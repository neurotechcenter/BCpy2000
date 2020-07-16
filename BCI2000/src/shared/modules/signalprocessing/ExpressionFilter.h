////////////////////////////////////////////////////////////////////////////////
//  $Id: ExpressionFilter.h 4656 2013-11-28 18:39:05Z mellinger $
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
#ifndef EXPRESSION_FILTER_H
#define EXPRESSION_FILTER_H

#include "GenericFilter.h"
#include "RandomGenerator.h"
#include "Expression/Expression.h"
#include <vector>

class ExpressionFilter : public GenericFilter
{
 public:
   ExpressionFilter();
   ~ExpressionFilter();

   void Preflight( const SignalProperties&, SignalProperties& ) const;
   void Initialize( const SignalProperties&, const SignalProperties& );
   void StartRun();
   void StopRun();
   void Process( const GenericSignal&, GenericSignal& );

 private:
   struct Expr : public Expression
   {
     Expr() : mpRandom( 0 ) {}
     Expr( LCRandomGenerator& r, const std::string& s = "" ) : Expression( s ), mpRandom( &r ) {}
     Node* Function( const std::string& name, const NodeList& );
     LCRandomGenerator* mpRandom;
   };
   typedef Expression::VariableContainer VariableContainer;
   typedef std::vector< std::vector<Expr> > ExpressionMatrix;

   static void LoadExpressions( const ParamRef&, ExpressionMatrix&, LCRandomGenerator& );
   static void CompileExpressions( ExpressionMatrix&, VariableContainer& );
   static void EvaluateExpressions( ExpressionMatrix&, const GenericSignal* = NULL, GenericSignal* = NULL );

   RandomGenerator mRandom;
   Expr mStartRunExpression,
        mStopRunExpression;
   ExpressionMatrix mExpressions;
   VariableContainer mVariables;
};
#endif // EXPRESSION_FILTER_H
