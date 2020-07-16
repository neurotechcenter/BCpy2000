//////////////////////////////////////////////////////////////////////////////////////
// $Id: Calculator.cpp 4447 2013-05-22 14:26:10Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A simple interactive calculator to test the ArithmeticExpression
//   class. The calculator supports arithmetic operators, and the functions from
//   math.h (cmath). You may create variables by assignment using the := assigment
//   operator.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "ArithmeticExpression.h"
#include <iostream>

using namespace std;

int
main( int argc, char** argv )
{
  if( argc > 1 )
  {
    cout << "Calculator: Enter arithmetic expressions or assignments on stdin, read results from stdout." << endl;
    return 0;
  }
  ArithmeticExpression::VariableContainer variables;
  bool success = true;
  string line;
  while( getline( cin, line ) )
  {
    ArithmeticExpression exp( line );
    success = exp.Compile( &variables );
    if( success )
      cout << "-> " << exp.Evaluate() << endl;
  }
  return success ? 0 : -1;
}
