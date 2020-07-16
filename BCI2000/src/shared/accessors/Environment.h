////////////////////////////////////////////////////////////////////////////////
// $Id: Environment.h 5599 2017-01-17 20:04:11Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Environment is a mix-in base class
//   that channels access to enviroment-like
//   global objects of types ParamList, StateList,
//   StateVector, and provides convenient accessor functions which perform
//   checks for parameter/state existence, and bounds.
//
//   Inheriting from Environment, EnvironmentExtension is an interface class
//   for components that need to handle system-wide events such as Preflight,
//   Initialize, etc. Direct inheritance from Environment should be used
//   for components that need access to parameters and states without handling
//   global events.
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
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Param.h"
#include "ParamList.h"
#include "ParamRef.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"
#include "StateRef.h"
#include "BCIStream.h"
#include "ClassName.h"
#include "BCIRegistry.h"
#include <set>
#include <iostream>
#include <sstream>
#include <string>

namespace Tiny { class File; }

class RunManager;
class SignalProperties;
class EnvironmentExtension;

class CoreModule;
class FilterWrapper;

class ApplicationWindowClient;

// Some macros to improve code readability.
#define BEGIN_PARAMETER_DEFINITIONS \
{ std::string p_[] = {
#define END_PARAMETER_DEFINITIONS \
}; AddParameters( p_, sizeof( p_ ) / sizeof( *p_ ) ); }

#define BEGIN_STATE_DEFINITIONS \
{ std::string s_[] = {
#define END_STATE_DEFINITIONS \
}; AddStates( s_, sizeof( s_ ) / sizeof( *s_ ), State::StateKind ); }

#define BEGIN_EVENT_DEFINITIONS \
{ std::string e_[] = {
#if IS_FIRST_MODULE
# define END_EVENT_DEFINITIONS \
}; AddStates( e_, sizeof( e_ ) / sizeof( *e_ ), State::EventKind ); }
#else // IS_FIRST_MODULE
# define END_EVENT_DEFINITIONS \
}; bcierr << "Trying to define events outside a source module."; }
#endif // IS_FIRST_MODULE

#define BEGIN_STREAM_DEFINITIONS \
{ std::string e_[] = {
#if IS_FIRST_MODULE
# define END_STREAM_DEFINITIONS \
}; AddStates( e_, sizeof( e_ ) / sizeof( *e_ ), State::StreamKind ); }
#else // IS_FIRST_MODULE
# define END_STREAM_DEFINITIONS \
}; bcierr << "Trying to define a stream outside a source module."; }
#endif // IS_FIRST_MODULE

// This base class channels access to Parameter, State, and Communication
// related objects that used to be arguments of member functions.
// "GenericFilter" inherits this class' "passive" accessor functions;
// to use its "controlling" functions, a class must be listed as
// a friend. Only framework classes should be friends.

class EnvironmentBase
{
public:
  enum ExecutionPhase
  {
    nonaccess,
    construction,
    publication,
    preflight,
    initialization,
    startRun,
    processing,
    stopRun,
    resting,
    destruction
  };
  typedef std::set<std::string, Param::NameCmp> NameSet;
  typedef std::map<const EnvironmentBase*, NameSet> NameSetMap;

  class Context
  {
   public:
    Context();

    static Context* GlobalInstance();
    static Context* CurrentInstance();

    void EnterPhase( ExecutionPhase, ParamListView* = nullptr, StateList* = nullptr, StateVector* = nullptr );
    ExecutionPhase Phase() const { return mPhase; }
    ParamListView* Parameters() { return mpParameters; }
    StateList* States() { return mpStates; }
    StateVector* Statevector() { return mpStatevector; }

    void RangeCheckParams( const ParamListView*, const NameSet& );
    void RegisterExtension__( EnvironmentExtension* p ) { mExtensions.insert( p ); }
    void UnregisterExtension( EnvironmentExtension* p ) { mExtensions.erase( p ); }

  private:
    friend class EnvironmentBase;
    ExecutionPhase mPhase;

    ParamListView* mpParameters;
    StateList* mpStates;
    StateVector* mpStatevector;
    bool mGlobal;

    NameSet mParamsRangeChecked;
    NameSetMap mOwnedParams;
    NameSetMap mParamsAccessedDuringPreflight;
    NameSetMap mOwnedStates;
    NameSetMap mStatesAccessedDuringPreflight;

    typedef std::set<EnvironmentExtension*> ExtensionsContainer;
    ExtensionsContainer mExtensions;
    ExtensionsContainer mEarlyPublished;
  };

  // Friends from framework classes.
  friend class GenericVisualization;

  friend class CoreModule;
  friend class StatusMessage;
  friend class FilterWrapper;

 // Protecting the constructor prevents instantiation of this class
 // outside its descendants.
 protected:
  EnvironmentBase( Context* = nullptr );
  EnvironmentBase( const EnvironmentBase& );
  EnvironmentBase& operator=( const EnvironmentBase& );
  virtual ~EnvironmentBase();
  int Instance() const
    { return mInstance; }
  Context* EnvironmentContext() const
    { return mpContext; }

 private:
  static Context* InitializeContext( Context* );
  Context* mpContext;

 protected:
  virtual ParamListView* OnParamListAccess(ParamListView* p) { return p; }
  virtual StateList* OnStateListAccess(StateList* s) { return s; }
  virtual StateVector* OnStateVectorAccess(StateVector* s) { return s; }

 // Opaque references to environment objects.
 // These symbols have the syntax of pointers but allow for intercepting
 // access to the underlying objects.
  template<class T, T* (Context::*P)(), T* (EnvironmentBase::*H)(T*)> class Accessor_ : Uncopyable
  {
  private:
    friend class EnvironmentBase;
    Accessor_* operator&(); // undefined
    Accessor_( EnvironmentBase* pParent, Context* pContext ) : mpParent(pParent), mpContext( pContext ) {}
  public:
    operator T*() const { return (mpParent->*H)((mpContext->*P)()); }
    T* operator->() const { return operator T*(); }
  private:
    EnvironmentBase* mpParent;
    Context* mpContext;
  };
  friend class Accessor_<ParamListView, &Context::Parameters, &EnvironmentBase::OnParamListAccess>;
  friend class Accessor_<StateList, &Context::States, &EnvironmentBase::OnStateListAccess>;
  friend class Accessor_<StateVector, &Context::Statevector, &EnvironmentBase::OnStateVectorAccess>;

 protected:
  Accessor_<ParamListView, &Context::Parameters, &EnvironmentBase::OnParamListAccess> Parameters;
  Accessor_<StateList, &Context::States, &EnvironmentBase::OnStateListAccess> States;
  Accessor_<StateVector, &Context::Statevector, &EnvironmentBase::OnStateVectorAccess> Statevector;

 protected:
  // Helper functions to construct and set an error context string.
  static void ErrorContext( const std::string&, const EnvironmentBase* = NULL );
  const EnvironmentBase* ObjectContext() const;

 // Convenient accessor functions. These are not static, so we can identify
 // the caller as an object.
 protected:
  // The Parameter()/OptionalParameter() functions allow access to parameters by name.
  MutableParamRef Parameter( const std::string& name );
  ParamRef Parameter( const std::string& name ) const;
  ParamRef ActualParameter( const std::string& name ) const;
  MutableParamRef OptionalParameter( const std::string& name,
                                     const std::string& defaultValue = "" );
  ParamRef OptionalParameter( const std::string& name,
                              const std::string& defaultValue = "" ) const;
  MutableParamRef OptionalParameter( const std::string& name,
                                     double defaultValue );
  ParamRef OptionalParameter( const std::string& name,
                              double defaultValue ) const;
  static std::string DescribeValue( const Param&, size_t index1, size_t index2 );

  bool AutoConfig_( bool );
  #define AutoConfig_

 private:
  enum ParamAccessFlags { none = 0, optional = 1, actual = 2 };
  Param* ParamAccess( const std::string& name, int flags = none ) const;
  virtual void OnParamAccess( const std::string& name ) const {}

  bool mAutoConfig;
  mutable NameSet mAutoConfigParams;
  mutable ParamList mTemporaryParams;

 protected:
  // A macro/function combination for convenient formulation of parameter checks.
  #define PreflightCondition( x )        (PreflightCondition_(#x,double(x)))
  bool PreflightCondition_( const char*, bool ) const;
  // Read/write access a state by its name.
  StateRef State( const std::string& name ) const;
  // Read-only access to states that are not required.
  // The second argument is a default value.
  StateRef OptionalState( const std::string& name, State::ValueType defaultValue = 0 ) const;

 private:
  const StateList* StateListAccess() const;
  void StateAccess( const std::string& name ) const;
  virtual void OnStateAccess( const std::string& name ) const {}

 // Controlling functions to be called from framework friends only.
 // In the future, these functions will be used to perform a number of
 // sanity checks / access control, e.g.
 // - Are these functions called in appropriate order?
 // - Are there any descendants instantiated outside the construction phase?
 // - Parameters may only be read/write accessed during the preflight and
 //   processing phases.
 // - Parameters accessed during the processing phase must have been accessed
 //   during the preflight phase, too -- this prevents a number of possible
 //   errors (typos in parameter names; newly introduced parameters not being
 //   checked during preflight; parameters being maintained in some class
 //   outside a GenericFilter).

 protected:
  static ExecutionPhase Phase();

 protected:
  void AddParameters( const std::string*, size_t ) const;
  void AddStates( const std::string*, size_t, int ) const;

  NameSet& ParamsRangeChecked() const;
  NameSetMap& OwnedParams() const;
  NameSetMap& ParamsAccessedDuringPreflight() const;
  NameSetMap& OwnedStates() const;
  NameSetMap& StatesAccessedDuringPreflight() const;

 private:
  int mInstance;
  static int sMaxInstanceID;
};

// Environment adds consistency checks for Preflight() vs.
// later access to Parameters/States.
class Environment : public EnvironmentBase
{
  // Friends from framework classes.
  friend class CoreModule;
  friend class FilterWrapper;
  friend class ApplicationWindowClient;

 protected:
  Environment() {}
  virtual ~Environment() {}

  using EnvironmentBase::OptionalParameter;

 protected:
  std::string CurrentRun() const;
  Tiny::File& CurrentRunFile() const;
  std::string CurrentSession() const;

 private:
  virtual void OnParamAccess( const std::string& name ) const;
  virtual void OnStateAccess( const std::string& name ) const;

  static RunManager sRunManager;
};

// A virtual interface for classes that provide global information and need
// to be notified of system phase transitions.
// Unlike GenericFilter descendants, descendants of EnvironmentExtension
// should not need any constructor code that publishes information; rather,
// they should use the Publish() virtual member function.
// That way we may have static instantiation without a separate registrar class,
// and avoid difficulties that arise from the fact that constructors are not
// ordinary member functions (constructors cannot be virtual; calling of virtual
// functions from base class constructors is impossible).

class EnvironmentExtension : protected Environment
{
 protected:
  EnvironmentExtension();
  virtual ~EnvironmentExtension();

 // The extension interface. Extension descendants implement these functions.
 // For an overview of the events handled by these functions, see the documentation
 // of the GenericFilter function.
 protected:
  virtual void Publish() = 0;
  virtual void AutoConfig() {}
  virtual void Preflight() const = 0;
  virtual void Initialize() = 0;
  virtual void PostInitialize() {}
  virtual void StartRun() {}
  virtual void PostStartRun() {}
  virtual void StopRun() {}
  virtual void PostStopRun() {}
  virtual void Process() {}
  virtual void PostProcess() {}
  virtual void Resting() {}
  virtual void Halt() {}

 // The public calling interface to virtual functions.
 // Each Call...() function sets up an error context, and calls its corresponding
 // virtual function.
 public:
  void CallPublish();
  void CallAutoConfig();
  void CallPreflight() const;
  void CallInitialize();
  void CallPostInitialize();
  void CallStartRun();
  void CallPostStartRun();
  void CallStopRun();
  void CallPostStopRun();
  void CallProcess();
  void CallPostProcess();
  void CallResting();
  void CallHalt();

 public:
  // Objects registered with AutoDelete() are deleted before a module exits.
  static EnvironmentExtension* AutoDelete( EnvironmentExtension* p );
  static void OnExit();
};

#endif // ENVIRONMENT_H





