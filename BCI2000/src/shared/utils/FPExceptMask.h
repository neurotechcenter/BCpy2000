//////////////////////////////////////////////////////////////////////
// $Id: FPExceptMask.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An object of this class disables FPU exceptions
//      while it exists, and restores the previous state on
//      destruction.
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
///////////////////////////////////////////////////////////////////////
#ifndef FP_EXCEPT_MASK_H
#define FP_EXCEPT_MASK_H

#include <float.h>
#include "Uncopyable.h"

#if __BORLANDC__
# define _MCW_EM        MCW_EM
# define _EM_INVALID    EM_INVALID
# define _EM_DENORMAL   EM_DENORMAL
# define _EM_ZERODIVIDE EM_ZERODIVIDE
# define _EM_OVERFLOW   EM_OVERFLOW
# define _EM_UNDERFLOW  EM_UNDERFLOW
# define _EM_INEXACT    EM_INEXACT
#endif // __BORLANDC__

class FPExceptMask : private Uncopyable
{
 public:
  FPExceptMask( int inExceptions = _MCW_EM )
    : mPrevState( 0 )
    {
      mPrevState = ::_controlfp( 0, 0 );
      ::_controlfp( inExceptions, _MCW_EM );
    }

  virtual ~FPExceptMask()
    {
      ::_controlfp( mPrevState, _MCW_EM );
    }

 private:
  int mPrevState;
};

#endif // FP_EXCEPT_MASK_H
