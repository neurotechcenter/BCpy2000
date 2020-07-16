////////////////////////////////////////////////////////////////////////////////
//  $Id: ExpressionFilter.cpp 5162 2015-12-09 13:25:56Z mellinger $
//  Author:      juergen.mellinger@uni-tuebingen.de
//  Description: A filter that uses arithmetic expressions to compute its
//    output.
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
#include "ExpressionFilter.h"
#include "StringUtils.h"
#include "BCIException.h"

using namespace std;

RegisterFilter( ExpressionFilter, 2.D2 );

ExpressionFilter::ExpressionFilter()
: mStartRunExpression( mRandom ),
  mStopRunExpression( mRandom )
{
  BEGIN_PARAMETER_DEFINITIONS
    "Filtering string StartRunExpression= % "
      " % % % // expression executed on StartRun",
    "Filtering string StopRunExpression= % "
      " % % % // expression executed on StopRun",
    "Filtering matrix Expressions= 0 1 "
      " % % % // expressions used to compute the output of the ExpressionFilter"
      " (rows are channels; empty matrix for none)",
  END_PARAMETER_DEFINITIONS
}


ExpressionFilter::~ExpressionFilter()
{
}


void
ExpressionFilter::Preflight( const SignalProperties& Input,
                                   SignalProperties& Output ) const
{
  // Test whether configuration can be loaded.
  LCRandomGenerator rg;
  Expr startRunExpression( rg, Parameter( "StartRunExpression" ) ),
       stopRunExpression( rg, Parameter( "StopRunExpression" ) );
  ExpressionMatrix expressions;
  LoadExpressions( Parameter( "Expressions" ), expressions, rg );
  VariableContainer variables;
  // Request output signal properties:
  Output = Input;
  size_t numRows = expressions.size(),
         numCols = numRows ? expressions[0].size() : 0;
  if( numCols != 0 )
    Output.SetChannels( numRows )
          .SetElements( numCols )
          .ElementUnit().SetGain( 1.0 ).SetOffset( 0.0 ).SetSymbol( "" );
  // Try evaluating expressions.
  startRunExpression.Compile( variables );
  startRunExpression.Evaluate();
  GenericSignal preflightInput( Input ), preflightOutput( Output );
  CompileExpressions( expressions, variables );
  EvaluateExpressions( expressions, &preflightInput, &preflightOutput );
  stopRunExpression.Compile( variables );
  stopRunExpression.Evaluate();
}


void
ExpressionFilter::Initialize( const SignalProperties&,
                              const SignalProperties& )
{
  mStartRunExpression = Expr( mRandom, Parameter( "StartRunExpression" ) );
  mStopRunExpression = Expr( mRandom, Parameter( "StopRunExpression" ) );
  LoadExpressions( Parameter( "Expressions" ), mExpressions, mRandom );
  mVariables.clear();
  mStartRunExpression.Compile( mVariables );
  CompileExpressions( mExpressions, mVariables );
  mStopRunExpression.Compile( mVariables );
}


void
ExpressionFilter::StartRun()
{
  mStartRunExpression.Evaluate();
}

void
ExpressionFilter::StopRun()
{
  mStopRunExpression.Evaluate();
}

void
ExpressionFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  if( mExpressions.empty() )
    Output = Input;
  else
    EvaluateExpressions( mExpressions, &Input, &Output );
}

void
ExpressionFilter::LoadExpressions( const ParamRef& inParam, ExpressionMatrix& outMatrix, LCRandomGenerator& rg )
{
  // Read an expression parameter into an expression matrix.
  outMatrix.clear();
  int numRows = inParam->NumRows(),
      numCols = inParam->NumColumns();
  if( numRows != 0 && numCols != 0 )
  {
    outMatrix.resize( numRows, vector<Expr>( numCols ) );
    for( int row = 0; row < numRows; ++row )
      for( int col = 0; col < numCols; ++col )
        outMatrix[ row ][ col ] = Expr( rg, inParam( row, col ) );
  }
}


void
ExpressionFilter::CompileExpressions(
  ExpressionMatrix& inExpressions,
  VariableContainer& ioVariables )
{
  for( size_t i = 0; i < inExpressions.size(); ++i )
    for( size_t j = 0; j < inExpressions[i].size(); ++j )
      inExpressions[i][j].Compile( &ioVariables );
}

void
ExpressionFilter::EvaluateExpressions(
  ExpressionMatrix& inExpressions,
  const GenericSignal* inpSignal,
  GenericSignal* outpSignal )
{
  for( size_t i = 0; i < inExpressions.size(); ++i )
  {
    for( size_t j = 0; j < inExpressions[i].size(); ++j )
    {
      double result = inExpressions[i][j].Evaluate( inpSignal );
      if( outpSignal )
        ( *outpSignal )( i, j ) = result;
    }
  }
}

namespace
{
  struct RandomValueNode : ExpressionParser::Node
  {
    typedef double (LCRandomGenerator::*Func)();
    RandomValueNode( LCRandomGenerator* r, Func f ) : mpRandom( r ), mpFunc( f ) {}
    double OnEvaluate() { return (mpRandom->*mpFunc)(); }
    LCRandomGenerator* mpRandom;
    Func mpFunc;
  };
  struct RandomIntNode : ExpressionParser::Node
  {
    RandomIntNode( LCRandomGenerator* r, Node* arg ) : mpRandom( r ) { Add( arg ); }
    double OnEvaluate()
    { return (*mpRandom)( static_cast<int>( mChildren[0]->Evaluate() ) ) + 1; }
    LCRandomGenerator* mpRandom;
  };
} // namespace


ExpressionParser::Node*
ExpressionFilter::Expr::Function( const std::string& inName, const NodeList& inArgs )
{
  using ExpressionParser::StringNode;
  if( Ci( inName ) == "random" )
  {
    if( inArgs.Size() != 1 )
      throw bciexception << "The random() function takes exactly one argument";
    StringNode* pArg = dynamic_cast<StringNode*>( inArgs[0] );
    if( !pArg )
      return new RandomIntNode( mpRandom, inArgs[0] );
    else
    {
      static const struct { const char* name; RandomValueNode::Func func; }
      dists[] =
      {
        { "uniform01", &LCRandomGenerator::RandomValue<LCRandomGenerator::Uniform01> },
        { "normal", &LCRandomGenerator::RandomValue<LCRandomGenerator::Normal> },
      };
      string name = pArg->Evaluate();
      RandomValueNode::Func f = 0;
      for( size_t i = 0; !f && i < sizeof( dists ) / sizeof( *dists ); ++i )
        if( Ci( name ) == dists[i].name )
          f = dists[i].func;
      if( f )
        return new RandomValueNode( mpRandom, f );
      ostringstream oss;
      for( size_t i = 0; i < sizeof( dists ) / sizeof( *dists ); ++i )
        oss << ", " << dists[i].name;
      throw bciexception
        << "Unknown distribution name: " << name
        << ", known distributions are: " << oss.str().substr( 2 );
      return 0;
    }
  }
  return Expression::Function( inName, inArgs );
}


