////////////////////////////////////////////////////////////////////////////////
// $Id: GenericFilter.cpp 5634 2017-06-30 17:57:56Z mellinger $
// Description: This file declares a purely abstract GenericFilter interface
//   which all BCI2000 filters are supposed to implement.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "GenericFilter.h"
#include "ClassName.h"
#include "StopWatch.h"
#include "SubchainFilter.h"
#include "ThreadLocal.h"
#include "BCIStream.h"
#include "BCIException.h"
#include <limits>
#include <sstream>
#include <iomanip>

#undef AutoConfig_

using namespace std;
using namespace bci;

// GenericFilter class definitions.
size_t GenericFilter::Registrar::sCreatedInstances = 0;
static const string sEmptyPos = "";
static const char sSeparator = '/';

static std::string ConstructPrefix(const Directory::Node* pNode)
{
  std::string prefix;
  for( ; pNode && pNode->Parent() != pNode; pNode = pNode->Parent())
    if(dynamic_cast<const SubchainFilter*>(pNode))
      prefix = pNode->Name() + sSeparator + prefix;
  if(!prefix.empty())
    prefix.pop_back();
  return prefix;
}

struct GenericFilter::Private
{
  static ThreadLocal<void*> stAuxThreadData;
  bool mTimedCalls;

  struct PrefixFilter : ParamListFilter
  {
    std::string prefix;

    bool onApply(std::string& ioName) const override
    {
      if(ioName.find(sSeparator) == std::string::npos)
        ioName = prefix + sSeparator + ioName;
      return true;
    }
    Param* onInsert(const Param& inParam, float inHint) override
    {
      Param param = inParam;
      param.SetSection(prefix);
      return ParamListFilter::onInsert(param, inHint);
    }
    const Param* onGet(const std::string& inName) const override
    {
      const Param* p = ParamListFilter::onGet(inName);
      if(!p && parentView())
        p = parentView()->get(inName);
      return p;
    }
    Param* onGet(const std::string& inName) override
    {
      Param* p = ParamListFilter::onGet(inName);
      if(!p && parentView())
        p = parentView()->get(inName);
      return p;
    }
  } mParamFilter;

  Private() : mTimedCalls(false) {}
};
ThreadLocal<void*> GenericFilter::Private::stAuxThreadData;

GenericFilter::Chain&
GenericFilter::RootChain()
{
  static GenericFilter::Chain rootChain( Registrar::Registrars() );
  return rootChain;
}

GenericFilter::GenericFilter()
: p(new Private)
{
  void* data = Private::stAuxThreadData;
  Directory::Node* pNode = static_cast<Directory::Node*>(data);
  SetParent(pNode);
  p->mParamFilter.prefix = ConstructPrefix(pNode);
  AllFilters().push_back( this );
}

GenericFilter::~GenericFilter()
{
  AllFilters().remove( this );
  delete p;
}

string
GenericFilter::VisParamName() const
{
  return std::string( "Visualize" ) + ClassName( typeid( *this ) );
}

bool
GenericFilter::TimedCalls() const
{
  return p->mTimedCalls;
}

void
GenericFilter::SetAuxThreadData(void* data)
{
  Private::stAuxThreadData = data;
}

ParamListView*
GenericFilter::OnParamListAccess(ParamListView* pView)
{
  if(!p->mParamFilter.prefix.empty())
  {
    p->mParamFilter.setParentView(pView);
    return &p->mParamFilter;
  }
  return pView;
}

// Static members
GenericFilter::FiltersType&
GenericFilter::AllFilters()
{
  static FiltersType allFilters;
  return allFilters;
}

Directory::Node*
GenericFilter::Directory()
{
  static Directory::RootNode* root = new Directory::RootNode;
  return root;
}

// GenericFilter::Registrar definitions
GenericFilter::Registrar::Registrar()
: mInstance( sCreatedInstances++ )
{
  ostringstream oss;
  oss << hex << setfill( '0' ) << setw( 8 ) << mInstance;
  mPos = oss.str();
  AutoDeleteInstance().insert( this );
}

GenericFilter::Registrar::Registrar( const char* inPos, int inPriority, bool inAutoDelete )
: mPos( inPos ),
  mPriority( inPriority ),
  mInstance( sCreatedInstances++ )
{
  int maxPriority = inPriority;
  RegistrarSet_::iterator i = Registrars().begin();
  while( i != Registrars().end() )
  {
    RegistrarSet_::iterator j = i++;
    // Determine max priority present.
    if( (*j)->mPriority > maxPriority )
      maxPriority = (*j)->mPriority;
    // Remove all registrars with lower priority.
    if( (*j)->mPriority < inPriority )
      Registrars().erase( j );
  }
  // Only insert if priority is high enough.
  if( inPriority >= maxPriority )
    Registrars().insert( this );

  if( inAutoDelete )
    AutoDeleteInstance().insert( this );
}

GenericFilter::Registrar::~Registrar()
{
  Registrars().erase( this );
}

bool
GenericFilter::Registrar::less::operator()( const Registrar* a, const Registrar* b ) const
{
  if( a->mPos != b->mPos )
    return ( a->mPos < b->mPos );
  return ( a->mInstance < b->mInstance );
}


GenericFilter::RegistrarSet&
GenericFilter::Registrar::Registrars()
{
  static RegistrarSet registrars;
  return registrars;
}

GenericFilter::Registrar::AutoDeleteSet::~AutoDeleteSet()
{
  while( !empty() )
  {
    delete *begin();
    erase( begin() );
  }
}

GenericFilter::Registrar::AutoDeleteSet&
GenericFilter::Registrar::AutoDeleteInstance()
{
  static AutoDeleteSet instance;
  return instance;
}

// GenericFilter definitions
const string&
GenericFilter::GetFirstFilterPosition()
{
  if( Registrar::Registrars().empty() )
    return sEmptyPos;
  return ( *Registrar::Registrars().begin() )->Position();
}

const string&
GenericFilter::GetLastFilterPosition()
{
  if( Registrar::Registrars().empty() )
    return sEmptyPos;
  return ( *Registrar::Registrars().rbegin() )->Position();
}

// GenericFilter::Chain definitions
GenericFilter::Chain::Chain( const Registrar::RegistrarSet_& r )
: mRegistrars( r )
{
}

GenericFilter::ChainInfo
GenericFilter::Chain::Info()
{
  ChainInfo result;
  for( Registrar::RegistrarSet_::const_iterator i = mRegistrars.begin(); i != mRegistrars.end(); ++i )
  {
    ChainEntry entry;
    entry.position = ( *i )->Position();
    entry.name = ClassName( ( *i )->Typeid() );
    result.push_back( entry );
  }
  return result;
}

ostream&
GenericFilter::ChainInfo::InsertInto( ostream& os )
{
  for( const_iterator i = begin(); i != end(); ++i )
    os << "{ " << i->position << " " << i->name << " } ";
  return os;
}

// Wrapper functions for "handler" calls.
#define CALL_BODY_( x, b ) \
  ErrorContext( #x, this ); \
  try { this->x b; } \
  catch( const BCIException& e ) { bcierr_ << e.What(); } \
  catch( BCIStream::OutStream::Ref& r ) { if( !r.RefersTo( bcierr__ ) ) throw; } \
  ErrorContext( "" );

#define TIMED_CALL_BODY_( x, b ) \
  if( TimedCalls() ) \
  { \
    StopWatch watch_; \
    { CALL_BODY_( x, b ) } \
    if( watch_.Lapse() > MeasurementUnits::SampleBlockDuration() * 1e3 \
        && bcierr__.Empty() ) \
          bciwarn_ << ClassName( typeid( *this ) ) << "::" #x ": " \
                   << "Execution required more than a sample block duration"; \
  } else { CALL_BODY_( x, b ) }

#define CALL_DEF_( x, a, b ) void GenericFilter::Call##x a { CALL_BODY_( x, b ) }

#define CALL_0( x ) CALL_DEF_( x, (), () )
#define CALL_1( x, T ) CALL_DEF_( x, (T t), (t) )
#define CALL_2( x, T, U ) CALL_DEF_( x, (T t, U u), (t, u) )

CALL_0( Publish )
void
GenericFilter::CallAutoConfig( const SignalProperties& Input )
{
  bool prev = AutoConfig_( true );
  CALL_BODY_( AutoConfig, (Input) );
  AutoConfig_( prev );
}

void
GenericFilter::CallPreflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  OptionalParameter( "EvaluateTiming" );
  CALL_BODY_( Preflight, (Input, Output) );
}

void
GenericFilter::CallInitialize( const SignalProperties& Input, const SignalProperties& Output )
{
#if !BCIDEBUG
  p->mTimedCalls = ( OptionalParameter( "EvaluateTiming", 1 ) != 0 );
#endif
  CALL_BODY_( Initialize, (Input, Output) );
}

void
GenericFilter::CallProcess( const GenericSignal& Input, GenericSignal& Output )
{
  TIMED_CALL_BODY_( Process, (Input, Output) );
}

void
GenericFilter::CallResting( const GenericSignal& Input, GenericSignal& Output )
{
  TIMED_CALL_BODY_( Resting, (Input, Output) );
}

CALL_0( StartRun )
CALL_0( StopRun )
CALL_0( Resting )
CALL_0( Halt )

void
GenericFilter::Chain::Add( GenericFilter::Registrar* pReg )
{
  mRegistrars.insert( pReg );
}

// Instantiate all registered filters once.
// We iterate through the registrars in reverse order. This implies that
// sub-filters acquired via PassFilter<>() must have a position string greater
// than that of their master filter; this appears more intuitive because
// sub-filters may then share the first part of their position
// strings with their master filters.
void
GenericFilter::Chain::Instantiate()
{
  for( RegistrarSet::reverse_iterator i = mRegistrars.rbegin();
                                       i != mRegistrars.rend(); ++i )
  {
    string filterName = ClassName( ( *i )->Typeid() );
    const string& posString = ( *i )->Position();
    ErrorContext( filterName + "::Constructor" );
    GenericFilter* pFilter = ( *i )->NewInstance();
    mOwnedFilters.push_front( pFilter );
    if( pFilter->AllowsVisualization() )
    {
      mVisualizations[pFilter].SetSourceID( posString );
      // Convert the position string to a float value with identical sort order.
      float placeValue = 1,
            sortingHint = 0;
      for( string::const_iterator i = posString.begin(); i != posString.end(); ++i )
      {
        sortingHint += *i * placeValue;
        placeValue /= 1 << ( 8 * sizeof( *i ) );
      }
      std::string prefix = ConstructPrefix(pFilter);
      string paramDefinition = "Visualize:Processing%20Stages";
      if(!prefix.empty())
        paramDefinition = prefix + ":" + paramDefinition;
      paramDefinition += " int "
                         + pFilter->VisParamName()
                         + "= 0 0 0 1 // Visualize "
                         + filterName
                         + " output (boolean)";
      Param param = Param::fromDefinition( paramDefinition );
      pFilter->Parameters->Add( param, sortingHint );
      bcidbg( 10 ) << "Added visualization parameter " << param.Name()
                   << " with sorting hint " << sortingHint
                   << "/pos string " << posString
                   << endl;
    }
    ErrorContext( "" );
  }
}

void
GenericFilter::Chain::Dispose()
{
  for( FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i )
  {
    ErrorContext( "Destructor", *i );
    delete *i;
    ErrorContext( "" );
  }
  mOwnedFilters.clear();
  mVisualizations.clear();
}

void
GenericFilter::Chain::OnPublish()
{
  for( FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i )
    ( *i )->CallPublish();
}

void
GenericFilter::Chain::OnPreflight( const SignalProperties& Input,
                                         SignalProperties& Output )
{
  mOwnedSignals[ NULL ].SetProperties( Input );
  GenericFilter* currentFilter = NULL;
  const SignalProperties* currentInput = &Input;
  for( FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i )
  {
    currentFilter = *i;
    int f = bcierr__.Flushes();
    currentFilter->CallAutoConfig( *currentInput );
    if( bcierr__.Flushes() > f )
      return;
    SignalProperties currentOutput;
    currentFilter->CallPreflight( *currentInput, currentOutput );
    if( currentOutput.Name() == currentInput->Name() || currentOutput.Name().empty() )
    {
      std::string name = ClassName(typeid(*currentFilter));
      std::string prefix = ConstructPrefix(currentFilter);
      if(!prefix.empty())
        name = prefix + sSeparator + name;
      currentOutput.SetName(name);
    }
    // The output signal will be created here if it does not exist.
    mOwnedSignals[ currentFilter ].SetProperties( currentOutput );
    currentFilter->OptionalParameter( currentFilter->VisParamName() );
    currentInput = &mOwnedSignals[ currentFilter ].Properties();
  }
  Output = mOwnedSignals[ currentFilter ].Properties();
}

void
GenericFilter::Chain::OnInitialize()
{
  const GenericSignal::ValueType NaN = numeric_limits<GenericSignal::ValueType>::quiet_NaN();
  const SignalProperties* currentInput = &mOwnedSignals[ NULL ].Properties();
  GenericFilter* currentFilter = NULL;
  for( FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i )
  {
    currentFilter = *i;
    const SignalProperties& currentOutput = mOwnedSignals[ currentFilter ].Properties();
    // This will implicitly create the output signal if it does not exist.
    currentFilter->CallInitialize( *currentInput, currentOutput );
    // Output signal visualization.
    bool visEnabled = false;
    if( currentFilter->AllowsVisualization() )
    {
      visEnabled = int( currentFilter->Parameter( currentFilter->VisParamName() ) ) != 0;
      if( visEnabled )
        mVisualizations[ currentFilter ].Send( currentOutput )
                                         .Send( GenericSignal( currentOutput, NaN ) );
      mVisualizations[ currentFilter ].Send( CfgID::Visible, visEnabled );
    }
    mVisualizations[ currentFilter ].SetEnabled( visEnabled );

    currentInput = &mOwnedSignals[ currentFilter ].Properties();
  }
}

void
GenericFilter::Chain::OnStartRun()
{
  for( FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i )
    ( *i )->CallStartRun();
}

void
GenericFilter::Chain::OnProcess( const GenericSignal& Input,
                                       GenericSignal& Output, bool inResting )
{
  const GenericSignal* currentInput = &Input;
  GenericFilter* currentFilter = NULL;
  for( FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i )
  {
    currentFilter = *i;
    // This will implicitly create the output signal if it does not exist.
    GenericSignal& currentOutput = mOwnedSignals[ currentFilter ];
    if( inResting )
      currentFilter->CallResting( *currentInput, currentOutput );
    else
      currentFilter->CallProcess( *currentInput, currentOutput );
    if( mVisualizations[ currentFilter ].Enabled() )
      mVisualizations[ currentFilter ].Send( currentOutput );

    currentInput = &mOwnedSignals[ currentFilter ];
  }
  if( currentFilter )
    Output.AssignValues( mOwnedSignals[currentFilter] );
  else
    Output.AssignValues( Input );
}

void
GenericFilter::Chain::OnStopRun()
{
  for( FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i )
    ( *i )->CallStopRun();
}

void
GenericFilter::Chain::OnResting()
{
  for( FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i )
    ( *i )->CallResting();
}

void
GenericFilter::Chain::OnHalt()
{
  for( FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i )
    ( *i )->CallHalt();
}

// Create an instance of the same type as a given one.
GenericFilter*
GenericFilter::NewInstance( const GenericFilter* existingInstance )
{
  Registrar* registrarFound = NULL;
  RegistrarSet::iterator i = Registrar::Registrars().begin();
  while( i != Registrar::Registrars().end() && registrarFound == NULL )
  {
    if( typeid( *existingInstance ) == ( *i )->Typeid() )
      registrarFound = *i;
    ++i;
  }
  GenericFilter* newInstance = NULL;
  if( registrarFound )
    newInstance = registrarFound->NewInstance();
  return newInstance;
}

// Directory
Directory::Node::~Node()
{
  SetParent( 0 );
  for( Container::iterator i = mChildren.begin(); i != mChildren.end(); ++i )
    ( *i )->SetParent( 0 );
}

void
Directory::Node::SetParent( Node* p )
{
  OnSetParent( p );
  if( mpParent )
    mpParent->Children().remove( this );
  mpParent = p;
  if( p )
    p->Children().push_front( this );
}

string
Directory::Node::OnName() const
{
  return ClassName( typeid( *this ) );
}

const string&
Directory::Node::Name() const
{
  if( mName.empty() )
  {
    int idx = 1;
    mName = OnName();
    Node* pParent = mpParent;
    while( pParent )
    {
      bool found = false;
      Container::const_iterator i = pParent->Children().begin();
      while( !found && i != pParent->Children().end() )
      {
        if( *i != this )
          found = ( mName == ( *i )->Name() );
        ++i;
      }
      if( found )
      {
        ostringstream oss;
        oss << OnName() << ':' << ++idx;
        mName = oss.str();
      }
      else
        pParent = 0;
    }
  }
  return mName;
}

string
Directory::Node::Path() const
{
  const char sep = '/';
  string path;
  if( mpParent )
    path = mpParent->Path();
  if( path.empty() || *path.rbegin() != sep )
    path += sep;
  return path + Name();
}

