////////////////////////////////////////////////////////////////////////////////
// $Id
// Author: juergen.mellinger@uni-tuebingen.de
//  Description: A data source that computes its data from an expression.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "ExpressionSource.h"

#include "FunctionSource.h"
#include "ObserverSource.h"
#include "BCIException.h"

using namespace std;
using namespace ExpressionParser;

ExpressionSource::ExpressionSource( const std::string& s )
: DataSource( s ),
  Expression( s ),
  mExpression( s ),
  mpContext( NULL ),
  mDataIndex( 0 )
{
}

void
ExpressionSource::OnDepends( const Context& inContext )
{
  if( CompilationState() == ArithmeticExpression::none )
  {
    mpContext = &inContext;
    Expression::Compile( inContext.variables );
    mpContext = NULL;
  }
}

void
ExpressionSource::OnInitialize( const Context& inContext )
{
  if( Sources().empty() )
  { // Assume a time course of state values between 0 and 1.
    mProperties = DataProperties();
    mProperties.Unit().SetOffset( 0 ).SetGain( 1 )
                      .SetRawMin( 0 ).SetRawMax( 1 );
    mProperties.Dimensions().resize( 2 );
    Dimension& ch = mProperties.Dimensions()[0];
    ch.labels.Resize( 1 );
    ch.labels[0] = Name();
    Dimension& el = mProperties.Dimensions()[1];
    el.labels.Resize( MeasurementUnits::SampleBlockSize() );
    el.unit.SetGain( 1.0 / MeasurementUnits::SamplingRate() )
           .SetSymbol( "s" );
    el.continuous = true;
    el.streaming = true;
  }
  else
  {
    mProperties = Sources()[0]->Properties();
    for( size_t i = 1; i < Sources().size(); ++i )
      if( !mProperties.Compatible( Sources()[i]->Properties() ) )
        bcierr << "While processing \"" << mExpression << "\": "
               << "Data dimensions must be compatible across sub-expressions" << endl;
  }
  mProperties.Name() = Name();
}

void
ExpressionSource::OnProcess( const Context& inContext )
{
  mpContext = &inContext;
}

DataSource::Value
ExpressionSource::OnData( int inIdx )
{
  mDataIndex = inIdx;
  Value result = 0;
  if( Sources().empty() )
    result = Expression::Evaluate( mpContext->signal, inIdx );
  else
    result = Expression::Evaluate( mpContext->signal );
  return result;
}

Node*
ExpressionSource::Variable( const std::string& inName )
{
  Node* pResult = FindObserver( inName );
  if( !pResult )
    pResult = Expression::Variable( inName );
  return pResult;
}

Node*
ExpressionSource::Function( const std::string& inName, const NodeList& inArgs )
{
  Node* pResult = NULL;
  FunctionSource* pFunction = FunctionSource::NewFunctionSource( inName, inArgs, mDataIndex );
  if( pFunction )
  {
    DataSource::Add( pFunction );
    pResult = pFunction;
  }
  else
    pResult = Expression::Function( inName, inArgs );
  return pResult;
}

Node*
ExpressionSource::MemberFunction( const std::string& inObject, const std::string& inName, const NodeList& inArgs )
{
  Node* pObserverNode = FindObserver( inObject ),
      * pResult = NULL;
  if( pObserverNode )
  {
    FunctionSource* pFunction = NULL;
    try
    {
      NodeList args = inArgs;
      args.Add( pObserverNode, 0 );
      pFunction = FunctionSource::NewFunctionSource( inName, args, mDataIndex );
    }
    catch( const bci::Exception& e )
    {
      Errors() << e.What() << endl;
    }
    if( pFunction )
    {
      DataSource::Add( pFunction );
      pResult = pFunction;
    }
    else
    {
      delete pObserverNode;
    }
  }
  if( !pResult )
    pResult = Expression::MemberFunction( inObject, inName, inArgs );
  return pResult;
}

Node*
ExpressionSource::FindObserver( const std::string& inName ) const
{
  Node* pResult = NULL;
  if( mpContext )
  {
    ObserverSource* pObserver = dynamic_cast<ObserverSource*>( mpContext->sources->Find( inName ) );
    if( pObserver )
      pResult = new ObserverNode( inName, pObserver );
  }
  return pResult;
}
