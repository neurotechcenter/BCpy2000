//////////////////////////////////////////////////////////////////////
// $Id: Runnable.h 5268 2016-03-04 14:54:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An interface class for objects that provide code
//   to be called back from code that takes a Runnable as a parameter.
//   Specify the code that should be executed inside the OnRun()
//   event handler.
//
//   A function call including parameters may be wrapped up into a
//   runnable by using the FunctionCall() and MembersCall()
//   templates which take the function signature as a template argument:
//   * FunctionCall< int( char ) >
//     to wrap a call to a global/static function that takes a char
//     as an argument, and returns an int.
//     The constructor takes the function and its arguments as
//     arguments:
//     FunctionCall< int( char ) > call( myFunc, myChar );
//   * MemberCall< int( MyClass*, char ) >
//     to wrap a call to a member function that takes a char
//     as an argument, and returns an int.
//     The constructor takes the function, the object, and
//     function arguments as arguments:
//     MemberCall< int( MyClass*, char ) > call( myFunc, myObjPtr, myChar );
//   * The return value may be obtained using the Result() function.
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
#ifndef TINY_RUNNABLE_H
#define TINY_RUNNABLE_H

namespace Tiny
{

class Runnable
{
 public:
  virtual ~Runnable() {}
  void Run() { OnRun(); }

 private:
  virtual void OnRun() = 0;
};

struct Run
{ Run( Runnable& r ) { r.Run(); } };

template<typename R> class FunctionResult_
{
 public:
  const R& Result() const { return value; }
 protected:
  void AssignResult( R r ) { value = r; }
 private:
  R value;
};

template<typename T> class FunctionResult_<T&>
{
 public:
  T& Result() { return *p; }
 protected:
   void AssignResult( T& t ) { p = &t; }
 private:
  T* p;
};

template<typename T> class FunctionCall
{ // When compilation fails because of a private constructor,
  // you are probably using the FunctionCall template with a function
  // type for which no specialization is provided in this file.
 private:
  FunctionCall();
  template<typename U> FunctionCall( const U& );
};
// Static function, void return type
template<> class FunctionCall<void()> : public Runnable
{
 public:
  FunctionCall( void (*f)() ) : f(f) {}
 private:
  void OnRun() { f(); }
  void (*f)();
};

// With Borland C++ Builder 6, compilation will fail here because BCB6 does not
// accept function signatures as types. This could be fixed by replacing
// function signatures with function pointer signatures: void(T1) -> void(*)(T1).
template<typename T1> class FunctionCall< void(T1) > : public Runnable
{
 public:
  FunctionCall( void (*f)(T1), T1 t1 ) : f(f), t1(t1) {}
 private:
  void OnRun() { f(t1); }
  void (*f)(T1);
  T1 t1;
};
template<typename T1, typename T2> class FunctionCall< void(T1,T2) > : public Runnable
{
 public:
  FunctionCall( void (*f)(T1,T2), T1 t1, T2 t2 ) : f(f), t1(t1), t2(t2) {}
 private:
  void OnRun() { f(t1,t2); }
  void (*f)(T1,T2);
  T1 t1;
  T2 t2;
};
template<typename T1, typename T2, typename T3> class FunctionCall< void(T1,T2,T3) > : public Runnable
{
 public:
  FunctionCall( void (*f)(T1,T2,T3), T1 t1, T2 t2, T3 t3 ) : f(f), t1(t1), t2(t2), t3(t3) {}
 private:
  void OnRun() { f(t1,t2,t3); }
  void (*f)(T1,T2,T3);
  T1 t1;
  T2 t2;
  T3 t3;
};
template<typename T1, typename T2, typename T3, typename T4> class FunctionCall< void(T1,T2,T3,T4) > : public Runnable
{
 public:
  FunctionCall( void (*f)(T1,T2,T3,T4), T1 t1, T2 t2, T3 t3, T4 t4 ) : f(f), t1(t1), t2(t2), t3(t3), t4(t4) {}
 private:
  void OnRun() { f(t1,t2,t3,t4); }
  void (*f)(T1,T2,T3,T4);
  T1 t1;
  T2 t2;
  T3 t3;
  T4 t4;
};
// Static function, non-void return type
template<typename R> class FunctionCall< R() > : public Runnable, public FunctionResult_<R>
{
 public:
  FunctionCall( R (*f)() ) : f(f) {}
 private:
  void OnRun() { this->AssignResult( f() ); }
  R (*f)();
};
template<typename R, typename T1> class FunctionCall< R(T1) > : public Runnable, public FunctionResult_<R>
{
 public:
  FunctionCall( R (*f)(T1), T1 t1 ) : f(f), t1(t1) {}
 private:
  void OnRun() { this->AssignResult( f(t1) ); }
  R (*f)(T1);
  T1 t1;
};
template<typename R, typename T1, typename T2> class FunctionCall< R(T1,T2) > : public Runnable, public FunctionResult_<R>
{
 public:
  FunctionCall( R (*f)(T1,T2), T1 t1, T2 t2 ) : f(f), t1(t1), t2(t2) {}
 private:
  void OnRun() { this->AssignResult( f(t1,t2) ); }
  R (*f)(T1,T2);
  T1 t1;
  T2 t2;
};
template<typename R, typename T1, typename T2, typename T3> class FunctionCall< R(T1,T2,T3) > : public Runnable, public FunctionResult_<R>
{
 public:
  FunctionCall( R (*f)(T1,T2,T3), T1 t1, T2 t2, T3 t3 ) : f(f), t1(t1), t2(t2),t3(t3) {}
 private:
  void OnRun() { this->AssignResult( f(t1,t2,t3) ); }
  R (*f)(T1,T2,T3);
  T1 t1;
  T2 t2;
  T3 t3;
};

template<typename T> class MemberCall
{ // When compilation fails because of a private constructor,
  // you are probably using the MemberCall template with a function
  // type for which no specialization is provided in this file.
 private:
  MemberCall();
  template<typename U> MemberCall( const U& );
};
// Member function, void return type
template<class C> class MemberCall< void(C*) > : public Runnable
{
 public:
  MemberCall( void (C::*f)(), C* c ) : f(f), c(c) {}
 private:
  void OnRun() { (c->*f)(); }
  void (C::*f)();
  C* c;
};
template<class C, typename T1> class MemberCall< void(C*,T1) > : public Runnable
{
 public:
  MemberCall( void (C::*f)(T1), C* c, T1 t1 ) : f(f), c(c), t1(t1) {}
 private:
  void OnRun() { (c->*f)(t1); }
  void (C::*f)(T1);
  C* c;
  T1 t1;
};
template<class C, typename T1, typename T2> class MemberCall< void(C*,T1,T2) > : public Runnable
{
 public:
  MemberCall( void (C::*f)(T1,T2), C* c, T1 t1, T2 t2 ) : f(f), c(c), t1(t1), t2(t2) {}
 private:
  void OnRun() { (c->*f)(t1,t2); }
  void (C::*f)(T1,T2);
  C* c;
  T1 t1;
  T2 t2;
};
template<class C, typename T1, typename T2, typename T3> class MemberCall< void(C*,T1,T2,T3) > : public Runnable
{
 public:
  MemberCall( void (C::*f)(T1,T2,T3), C* c, T1 t1, T2 t2, T3 t3 ) : f(f), c(c), t1(t1), t2(t2), t3(t3) {}
 private:
  void OnRun() { (c->*f)(t1,t2,t3); }
  void (C::*f)(T1,T2,T3);
  C* c;
  T1 t1;
  T2 t2;
  T3 t3;
};
// Member function, non-void return type
template<typename R, class C> class MemberCall< R(C*) > : public Runnable, public FunctionResult_<R>
{
 public:
  MemberCall( R (C::*f)(), C* c ) : f(f), c(c) {}
 private:
  void OnRun() { this->AssignResult( (c->*f)() ); }
  R (C::*f)();
  C* c;
};
template<typename R, class C, typename T1> class MemberCall< R(C*,T1) > : public Runnable, public FunctionResult_<R>
{
 public:
  MemberCall( R (C::*f)(T1), C* c, T1 t1 ) : f(f), c(c), t1(t1) {}
 private:
  void OnRun() { this->AssignResult( (c->*f)(t1) ); }
  R (C::*f)(T1);
  C* c;
  T1 t1;
};
template<typename R, class C, typename T1, typename T2> class MemberCall< R(C*,T1,T2) > : public Runnable, public FunctionResult_<R>
{
 public:
  MemberCall( R (C::*f)(T1,T2), C* c, T1 t1, T2 t2 ) : f(f), c(c), t1(t1), t2(t2) {}
 private:
  void OnRun() { this->AssignResult( (c->*f)(t1,t2) ); }
  R (C::*f)(T1,T2);
  C* c;
  T1 t1;
  T2 t2;
};
template<typename R, class C, typename T1, typename T2, typename T3> class MemberCall< R(C*,T1,T2,T3) > : public Runnable, public FunctionResult_<R>
{
 public:
  MemberCall( R (C::*f)(T1,T2,T3), C* c, T1 t1, T2 t2, T3 t3 ) : f(f), c(c), t1(t1), t2(t2), t3(t3) {}
 private:
  void OnRun() { this->AssignResult( (c->*f)(t1,t2,t3) ); }
  R (C::*f)(T1,T2,T3);
  C* c;
  T1 t1;
  T2 t2;
  T3 t3;
};

} // namespace

using Tiny::Runnable;
using Tiny::FunctionCall;
using Tiny::MemberCall;

#endif // RUNNABLE_H

