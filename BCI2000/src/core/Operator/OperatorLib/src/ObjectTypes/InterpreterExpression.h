////////////////////////////////////////////////////////////////////////////////
// $Id: InterpreterExpression.h 5293 2016-03-21 17:22:22Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: An expression that is linked to a command interpreter object.
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
#ifndef INTERPRETER_EXPRESSION_H
#define INTERPRETER_EXPRESSION_H

#include "Expression.h"

class CommandInterpreter;

class InterpreterExpression : public Expression
{
 public:
  InterpreterExpression( CommandInterpreter&, const std::string& inExpr = "" );
  
  InterpreterExpression& AllowAssignment()
    { mAllowAssignment = true; return *this; }
  InterpreterExpression& ForbidAssignment()
    { mAllowAssignment = false; return *this; }
  
  double Evaluate( int sample = 0 ) const
    { return Execute( sample ); }
  double Execute( int sample = 0 ) const;

  bool RefersStates() const override
    { return mRefersStates; }

 protected:
  Node* Variable( const std::string& );
  Node* VariableAssignment( const std::string&, Node* );

  Node* State( const std::string& );
  Node* StateAssignment( const std::string&, Node* );
  
  void AssertAssignment() const;

  static bool StateExists( CommandInterpreter&, const std::string& );
  static void AssertState( CommandInterpreter&, const std::string& );

 private:
  CommandInterpreter& mrInterpreter;
  bool mAllowAssignment, mRefersStates;
  mutable int mSample;

  class StateNode : public Node
  {
   public:
    StateNode( CommandInterpreter& interpreter, const std::string& name, const int& sample )
    : mrInterpreter( interpreter ), mName( name ), mrSample( sample ) {}
   protected:
    double OnEvaluate();
   private:
    CommandInterpreter& mrInterpreter;
    const int& mrSample;
    std::string mName;
  };

  class StateAssignmentNode : public Node
  {
   public:
    StateAssignmentNode( CommandInterpreter& interpreter, const std::string& name, Node* rhs )
    : mrInterpreter( interpreter ), mName( name ) { Add( rhs ); }
   protected:
    double OnEvaluate();
   private:
    CommandInterpreter& mrInterpreter;
    std::string mName;
  };
};

#endif // INTERPRETER_EXPRESSION_H
