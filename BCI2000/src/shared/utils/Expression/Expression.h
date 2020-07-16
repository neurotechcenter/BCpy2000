//////////////////////////////////////////////////////////////////////////////////////
// $Id: Expression.h 5719 2018-03-17 11:17:21Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A simple expression parser for use within BCI2000.
//   Usage:
//   ex1 = Expression( "(1+ResultCode)==Signal(TargetCode,3)" );
//   double val1 = ex.Evaluate( inputSignalPtr );
//   ex2 = Expression( "TargetCode*(ResultCode==TargetCode)" );
//   double val2 = ex.Evaluate();
//   double val3 = Expression( Parameter( "ControlExpression" ) ).Evaluate();
//
//   Expressions may involve operators, numbers, BCI2000 states, and a GenericSignal
//   if given as an argument to the Evaluate() member function.
//   States are referred to by name, a signal is referred to in the form
//   "Signal(<channel>,<element>)".
//
//   Arithmetic operators: ^ unary- * / + -
//                         a^b evaluates to ::pow(a,b).
//   Comparison operators:  < > <= >= == != ~=
//   Logical operators:    ! ~ && ||
//                         ! and ~ are synonymous.
//   Operator precedence follows the order of appearance in the above list.
//   As usual, braces ( ) may be used to override operator precedence.
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
//////////////////////////////////////////////////////////////////////////////////////
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ArithmeticExpression.h"
#include "Environment.h"
#include "GenericSignal.h"

#include "ExpressionParser.hpp"

class Expression : public ArithmeticExpression, private Environment
{
 public:
  using ArithmeticExpression::Context;
  using ArithmeticExpression::CompilationState;
  using ArithmeticExpression::VariableContainer;
  static const VariableContainer& Constants;

  Expression( const std::string& s = "" )
    : ArithmeticExpression( s ),
      mOptionalAccess( false ),
      mDefaultValue( 0 ),
      mAllowStateAssignment( false ),
      mpSignal( NULL ),
      mSample( 0 ),
      mRefersSignal( -1 ),
      mRefersStates( -1 )
    {}
  ~Expression()
    {}

  Expression& SetOptionalAccess( State::ValueType inDefault = 0 );
  Expression& ClearOptionalAccess();

  bool IsValid( const GenericSignal* = NULL, int sample = 0, const ArithmeticExpression::Context& = ArithmeticExpression::Context() );
  bool IsValid( const SignalProperties&, int sample = 0, const ArithmeticExpression::Context& = ArithmeticExpression::Context() );
  double Evaluate( const GenericSignal* = NULL, int sample = 0 ) const;
  double Execute( const GenericSignal* signal = NULL, int sample = 0 ) const
    { return Evaluate( signal, sample ); }

  virtual bool RefersStates() const;
  virtual bool RefersSignal() const;

 protected:
  Node* Variable( const std::string& name );
  Node* Signal( AddressNode*, AddressNode* );
  Node* State( const std::string& );
  Node* StateAssignment( const std::string&, Node* );

 private:
  Node* NewStateNode( const std::string& );

 private:
  bool                 mOptionalAccess;
  State::ValueType     mDefaultValue;
  mutable bool         mAllowStateAssignment;

  mutable const GenericSignal* mpSignal;
  mutable int                  mSample;
  mutable int  mRefersSignal, mRefersStates;

 // Additional node classes
 private:
  class SignalNode : public Node
  {
   public:
    typedef const GenericSignal* const SignalPointer;
    SignalNode( const SignalPointer&, AddressNode*, AddressNode* );

   protected:
    double OnEvaluate() override;

   private:
    const SignalPointer& mrpSignal;
    AddressNode* mpChannelAddress,
               * mpElementAddress;
    mutable int  mChannelIdx,
                 mElementIdx;
  };

  class StateNode : public Node
  {
   public:
    StateNode( const StateRef&, const int& sample );

   protected:
    double OnEvaluate() override;

   private:
    StateRef mStateRef;
    const int& mrSample;
  };

  class StateAssignmentNode : public Node
  {
   public:
    StateAssignmentNode( const StateRef&, Node*, const int& sample, const bool& allowed );

   protected:
    double OnEvaluate() override;

   private:
    StateRef mStateRef;
    const int& mrSample;
    const bool& mrAllowed;
  };
};

#endif // EXPRESSION_H

