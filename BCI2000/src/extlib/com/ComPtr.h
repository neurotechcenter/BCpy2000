////////////////////////////////////////////////////////////////////////////////
// $Id: ComPtr.h 3925 2012-04-10 12:17:35Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A smart pointer managing COM objects' AddRef() and Release()
//   calls. Differently from typical COM smart pointers, this smart pointer
//   has a "symmetric" interface, i.e. it does not require additional AddRef()
//   calls after assignment, and also calls Release() on any previously held object.
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
#ifndef COM_PTR_H
#define COM_PTR_H

namespace com {

template<class T>
class Ptr
{
 private:
  struct PtrToPtr
  {
    PtrToPtr( T** p ) : mp( p ) {}
    operator T**() { return mp; }
    operator void**() { return reinterpret_cast<void**>( mp ); }
    T** mp;
  };

 public:
  Ptr() : mp( NULL ) {}
  Ptr( T* pT ) : mp( pT ) { Inc(); }
  Ptr( const Ptr& c ) : mp( c.mp ) { Inc(); }
  ~Ptr() { Dec(); }
  Ptr& operator=( T* pT ) { Dec(); mp = pT; Inc(); return *this; }
  Ptr& operator=( const Ptr& c ) { Dec(); mp = c.mp; Inc(); return *this; }
  PtrToPtr Assignee() { Dec(); mp = NULL; return &mp; }
  T* operator->() { return mp; }
  const T* operator->() const { return mp; }
  operator T*() { return mp; }
  operator const T*() const { return mp; }

 private:
  void Inc() { if( mp ) mp->AddRef(); }
  void Dec() { if( mp ) mp->Release(); }
  T* mp;
};

} // namespace

#endif // COM_PTR_H
