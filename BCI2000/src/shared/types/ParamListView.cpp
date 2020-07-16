////////////////////////////////////////////////////////////////////////////////
// $Id: ParamList.cpp 5593 2017-01-13 17:22:15Z mellinger $
// Authors: gschalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
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
#include "ParamListView.h"
#include "ParamRef.h"

#include <sstream>
#include <fstream>
#include <set>

ParamListView&
ParamListView::assign(const ParamListView& other)
{
  clear();
  for(const auto& param : other)
    insert(param, 0.0);
  return *this;
}

MutableParamRef
ParamListView::operator()( const std::string& inName )
{
  return MutableParamRef( &ByName( inName ) );
}

ParamRef
ParamListView::operator()( const std::string& inName ) const
{
  return ParamRef( &ByName( inName ) );
}

const Param&
ParamListView::ByName( const std::string& inName ) const
{
  static Param defaultParam = Param("", "Default");
  const Param* result = get(inName);
  return result ? *result : defaultParam;
}

Param&
ParamListView::ByName( const std::string& inName )
{
  Param* result = get(inName);
  if(!result)
    result = insert(Param(inName,""), 0);
  return *result;
}

void
ParamListView::Add( const Param& inParam, float inSortingHint )
{
  insert(inParam, inSortingHint);
}

bool
ParamListView::Add( const std::string& inLine )
{
  std::istringstream linestream( inLine );
  Param param;
  if( linestream >> param )
    ByName( param.Name() ) = param;
  return !!linestream;
}

std::ostream&
ParamListView::InsertInto( std::ostream& os ) const
{
  for( int i = 0; i < Size(); ++i )
    os << ByIndex( i ) << '\n';
  return os;
}

std::istream&
ParamListView::ExtractFrom( std::istream& is )
{
  Clear();
  Param param;
  is >> std::ws;
  while( !is.eof() && is >> param >> std::ws )
    ByName( param.Name() ) = param;
  return is;
}

std::ostream&
ParamListView::Serialize( std::ostream& os ) const
{
  for( int i = 0; i < Size(); ++i )
    ByIndex( i ).Serialize( os );
  return os;
}

std::istream&
ParamListView::Unserialize( std::istream& is )
{
  return ExtractFrom( is );
}

bool
ParamListView::Save( const std::string& inFileName ) const
{
  std::ofstream file( inFileName.c_str() );
  if( !file.is_open() )
    return false;
  file << *this;
  return !file.fail();
}

bool
ParamListView::Load( const std::string& inFileName, bool inImportNonexisting )
{
  std::ifstream file( inFileName.c_str() );
  ParamList paramsFromFile;
  file >> paramsFromFile;
  if( file.fail() )
    return false;

  // If desired, exclude parameters missing from the main parameter list.
  typedef std::set<std::string> NameSet;
  NameSet unwantedParams;
  if( !inImportNonexisting )
    for( int i = 0; i < paramsFromFile.Size(); ++i )
      if( !Exists( paramsFromFile[i].Name() ) )
        unwantedParams.insert( paramsFromFile[i].Name() );

  for( NameSet::const_iterator i = unwantedParams.begin(); i != unwantedParams.end(); ++i )
    paramsFromFile.Delete( *i );

  for( int i = 0; i < paramsFromFile.Size(); ++i )
  {
    Param &p = paramsFromFile[i], &q = ByName( p.Name() );
    if( !q.Readonly() )
      q.AssignValues( p, true );
  }
  return true;
}

// ParamListFilter class
ParamListFilter::ParamListFilter()
: mpParentView(nullptr)
{
}

void
ParamListFilter::setParentView(ParamListView* pView)
{
  mpParentView = pView;
}

const ParamListView*
ParamListFilter::parentView() const
{
  return mpParentView;
}

ParamListView*
ParamListFilter::parentView()
{
  return mpParentView;
}

bool
ParamListFilter::onApply(std::string& ioName)
{
  return const_cast<const ParamListFilter*>(this)->onApply(ioName);
}

int ParamListFilter::onCount() const
{
  return mpParentView ? mpParentView->count() : 0;
}

Param*
ParamListFilter::onInsert(const Param& inParam, float inSortingHint)
{
  std::string name = inParam.Name();
  if(onApply(name) && mpParentView)
  {
    Param param = inParam;
    param.SetName(name);
    return mpParentView->insert(param, inSortingHint);
  }
  return nullptr;
}

bool
ParamListFilter::onRemove(const std::string& inName)
{
  std::string name = inName;
  if(onApply(name) && mpParentView)
    return mpParentView->remove(name);
  return false;
}

Param*
ParamListFilter::onGet(const std::string& inName)
{
  std::string name = inName;
  if(onApply(name) && mpParentView)
    return mpParentView->get(name);
  return nullptr;
}

const Param*
ParamListFilter::onGet(const std::string& inName) const
{
  std::string name = inName;
  if(onApply(name) && mpParentView)
    return mpParentView->get(name);
  return nullptr;
}

Param*
ParamListFilter::onGet(size_t idx)
{
  return mpParentView ? mpParentView->get(idx) : nullptr;
}

const Param*
ParamListFilter::onGet(size_t idx) const
{
  return mpParentView ? mpParentView->get(idx) : nullptr;
}

void
ParamListFilter::onClear()
{
  if(mpParentView)
    mpParentView->clear();
}

void
ParamListFilter::onSort()
{
  if(mpParentView)
    mpParentView->sort();
}

void
ParamListFilter::onSetUnchanged()
{
  if(mpParentView)
    mpParentView->setUnchanged();
}
