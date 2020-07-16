////////////////////////////////////////////////////////////////////////////////
// $Id: StateTransform.h 3798 2012-02-01 18:07:06Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A filter that transforms state values according to rules.
//              Whenever a given state's value changes, it replaces the new
//              value by a user-defined expression.
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
#ifndef STATE_TRANSFORM_H
#define STATE_TRANSFORM_H

#include "GenericFilter.h"
#include "Expression/Expression.h"

class StateTransform : public GenericFilter
{
 public:
   StateTransform();
   ~StateTransform();

   void Preflight( const SignalProperties&, SignalProperties& ) const;
   void Initialize( const SignalProperties&, const SignalProperties& );
   void Process( const GenericSignal&, GenericSignal& );

 private:
   std::vector<std::string> mStateNames;
   std::vector<Expression>  mExpressions;
   std::vector<int>         mPreviousInputStateValues,
                            mPreviousOutputStateValues;
};
#endif // STATE_TRANSFORM_H
