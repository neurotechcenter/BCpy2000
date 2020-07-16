////////////////////////////////////////////////////////////////////////////////
// $Id: TaskType.h 4820 2015-04-27 19:45:34Z mellinger $
// Author: juergen.mellinger@neurotechcenter.org
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
#ifndef TINY_PROPERTY_H
#define TINY_PROPERTY_H

#include <sstream>
#include <fstream>
#include <type_traits>
#include <vector>

#include "Synchronized.h"
#include "Lockable.h"
#include "StringUtils.h"
#include "Debugging.h"

#define PropertyMember(name, type) \
    private: static Property::Declaration* s##name##Declarator() \
           { static Property::Declaration d = { #name, nullptr }; return &d; } \
    public: Property::MemberProperty<type, s##name##Declarator> name;
#define PropertyMembersEnd() \
    private: Property::EndOfProperties mEndOfProperties;


namespace Tiny
{

namespace Property
{

typedef Tiny::String String;

enum State {
  EmptyValid = 0,
  NotEmpty = 1,
  Invalid = 2,
};

struct Declaration
{
  const char* const name;
  type_info* type;
};

class BasicHolder;
class BasicContainer;

class BasicProperty
{
  friend class BasicContainer;
private:
  BasicProperty();
  BasicProperty(const BasicProperty&);
protected:
  BasicProperty(void* data, const Declaration*);
  void clone(const BasicProperty&);
public:
  virtual ~BasicProperty();

  BasicProperty& operator=(const BasicProperty& other) { return assign(other); }
  BasicProperty& operator=(const String& s) { return set(s); }
  bool operator==(const BasicProperty& other) { return equal(other); }
  bool operator!=(const BasicProperty& other) { return !equal(other); }

  BasicHolder* holder() const { return mpHolder; }
  String name() const { return mpDecl->name; }
  const Declaration* declaration() const { return mpDecl; }
  int state() const;
  bool empty() const { return mEmpty; }
  bool valid() const { return mValid; }
  BasicProperty& setError(const String&);
  const String& error() const { return mError; }

  String toString() const;
  BasicProperty& set(const String& s);
  BasicProperty& clear();

  BasicProperty& assign(const BasicProperty&);
  bool equal(const BasicProperty& other) const;

  void* data() { return mpData; }
  const void* data() const { return mpData; }
  bool revalidate();
  void revalidateLater();

protected:
  void doSetValue(const void*);

  virtual void onClear(void*) = 0;
  virtual bool onSet(const String&) = 0;
  virtual void onAssign(void*, const void*) const = 0;
  virtual bool onEqual(const void*, const void*) const = 0;
  virtual void onSerialize(std::ostream&, const void*) const = 0;
  virtual void* onUnserialize(std::istream&) const = 0;
  virtual void onDispose(void*) const = 0;

private:
  bool validate(const void*);

  const Declaration* const mpDecl;
  void* const mpData;
  bool mValid, mEmpty;
  String mError;
  BasicHolder* mpHolder;
};

class BasicHolder
{
private:
  BasicHolder(const BasicHolder&);
  BasicHolder& operator=(const BasicHolder&);
protected:
  BasicHolder();
  virtual ~BasicHolder();
public:
  void setParentProperty(BasicProperty*);
  BasicProperty* parentProperty();

  BasicProperty* property(const String& name);
  const BasicProperty* property(const String& name) const;
  std::vector<BasicProperty*> properties();
  std::vector<const BasicProperty*> properties() const;
  bool valid() const;
  bool revalidate();
  bool validate(const BasicProperty*, const void*, std::string&);

protected:
  bool equalSameType(const BasicHolder&) const;

  static BasicHolder* currentlyConstructed();
  virtual void onRegisterProperty(BasicProperty*) = 0;
  virtual void onConstructionEnd() = 0;

  typedef std::vector<std::pair<const char*, ptrdiff_t>> Proplist;
  virtual const Proplist& onProperties() const = 0;

  virtual void onValidate(const BasicProperty*, const void* newval) = 0;
  const BasicProperty* validand() const;
  std::ostream& validationError();

  ptrdiff_t propertyOffset(const BasicProperty*) const;
  BasicProperty* propertyAddress(ptrdiff_t) const;

private:
  friend class BasicProperty;
  void registerProperty(BasicProperty*);
  friend class EndOfProperties;
  static void constructionEnd();

  bool mConstructionFinished;
  BasicProperty* mpParentProperty;
};

template <class T>
class IsEmptyAware
{
  typedef char one;
  typedef struct { one _[2]; } two;
  template <class C> static one test( decltype(&C::empty) );
  template <class C> static two test(...);
public:
  enum { value = sizeof(test<T>(0)) == sizeof(one) };
};

enum { PODType, PlainType, EmptyAwareType, ContainerType, StringType };

template <class T>
struct PropertyType
{
  enum {
    value = std::is_pod<T>::value ? PODType :
            std::is_base_of<std::string, T>::value ? StringType :
            std::is_base_of<BasicContainer, T>::value ? ContainerType :
            IsEmptyAware<T>::value ? EmptyAwareType :
            PlainType
  };
};

template<class T, int = PropertyType<T>::value>
class Property;

template<class T>
class Property<T, PlainType> : public BasicProperty
{
private:
  Property(const Property&);
protected:
  Property(const Declaration* d) : BasicProperty(&mData, d) { onClear(&mData); }
  void clone(const Property& other) { onAssign(&mData, &other.mData); BasicProperty::clone(other); }
public:
  Property& operator=(const Property& other) { setValue(other.mData); return *this; }
  Property& operator=(const BasicProperty& other) { assign(other); return *this; }
  Property& operator=(const T& t) { return setValue(t); }
  operator const T&() const { return value(); }

  const T& value() const { return mData; }
  Property& setValue(const T& t) { doSetValue(&t); return *this; }

  bool operator==(const Property& other) const { return onEqual(&mData, &other.mData); }
  bool operator!=(const Property& other) const { return !onEqual(&mData, &other.mData); }

protected:
  T& mutableValue() { return mData; }

  void onClear(void* p) override { mData = T(); }
  bool onSet(const String&) override { return false; }
  bool onEqual(const void* p1, const void* p2) const override { return *static_cast<const T*>(p1) == *static_cast<const T*>(p2); }
  void onAssign(void* p1, const void* p2) const override { *static_cast<T*>(p1) = *static_cast<const T*>(p2); }
  void onSerialize(std::ostream& os, const void* p) const override { os << *static_cast<const T*>(p); }
  void* onUnserialize(std::istream& is) const override { T* p = new T; is >> *p; return p; }
  void onDispose(void* p) const override { delete static_cast<T*>(p); }

private:
  T mData;
};

template<class T>
class Property<T, PODType> : public Property<T, PlainType>
{
  typedef Property<T, PlainType> Super;
private:
  Property(const Property&);
protected:
  Property(const Declaration* d) : Super(d) {}
public:
  Property& operator=(const Property& other) { Super::operator=(other); return *this; }
  Property& operator=(const BasicProperty& other) { Super::operator=(other); return *this; }
  Property& operator=(const T& t) { Super::operator=(t); return *this; }
  operator const T&() const { return value(); }
  bool operator==(const Property& other) const { return Super::operator==(other); }
  bool operator!=(const Property& other) const { return Super::operator!=(other); }
protected:
  void onClear(void* p) override { ::memset(p, 0, sizeof(T)); }
};

template<class T>
class Property<T, EmptyAwareType> : public Property<T, PlainType>
{
  typedef Property<T, PlainType> Super;
private:
  Property(const Property&);
protected:
  Property(const Declaration* d) : Super(d) {}
public:
  Property& operator=(const Property& other) { Super::operator=(other); return *this; }
  Property& operator=(const BasicProperty& other) { Super::operator=(other); return *this; }
  bool operator==(const Property& other) const { return Super::operator==(other); }
  bool operator!=(const Property& other) const { return Super::operator!=(other); }

  Property& operator=(const T& t) { return setValue(t); }
  Property& setValue(const T& t) { doSetValue(t.empty() ? nullptr : &t); return *this; }

protected:
  void onClear(void* p) override { return static_cast<T*>(p)->clear(); }
};

template<class T>
class Property<T, StringType> : public Property<T, EmptyAwareType>
{
  typedef Property<T, EmptyAwareType> Super;
private:
  Property(const Property&);
protected:
  Property(const Declaration* d) : Super(d) {}
public:
  Property& operator=(const Property& other) { Super::operator=(other); return *this; }
  Property& operator=(const BasicProperty& other) { Super::operator=(other); return *this; }
  Property& operator=(const T& t) { Super::operator=(t); return *this; }
  bool operator==(const Property& other) const { return Super::operator==(other); }
  bool operator!=(const Property& other) const { return Super::operator!=(other); }
protected:
  bool onSet(const String& s) override { doSetValue(s.empty() ? nullptr : &s); return true; }
};

template<class T>
class Property<T, ContainerType> : public Property<T, EmptyAwareType>
{
  typedef Property<T, EmptyAwareType> Super;
private:
  Property(const Property&);
protected:
  Property(const Declaration* d) : Super(d) { value().setParentProperty(this); }
public:
  Property& operator=(const Property& other) { Super::operator=(other); return *this; }
  Property& operator=(const BasicProperty& other) { Super::operator=(other); return *this; }
  Property& operator=(const T& t) { Super::operator=(t); return *this; }
  bool operator==(const Property& other) const { return Super::operator==(other); }
  bool operator!=(const Property& other) const { return Super::operator!=(other); }

  T& value() { return Super::mutableValue(); }
  const T& value() const { return Super::value(); }
};

typedef Declaration* (*Declarator)();
template<class T, Declarator> class MemberProperty;

template<class U> class Holder : public BasicHolder
{
private:
  Holder();
public:
  Holder(U* obj) : BasicHolder(), mClassInit(false) { onConstructionBegin(obj); }
  Holder(const Holder& other) : BasicHolder(), mClassInit(false), mValidators(other.mValidators) {}
  Holder& operator=(const Holder& other) { mValidators = other.mValidators; return *this; }
  bool operator==(const Holder& other) const { return equalSameType(other); }
  bool operator!=(const Holder& other) const { return !equalSameType(other); }

private:
  void setValidator(const Declaration* d, nullptr_t)
  {
    std::remove_if(
      mValidators.begin(), mValidators.end(),
      [d](std::pair<const Declaration*, Validator>& v)
      { return v.first == d; }
    );
  }

  template <class T>
  void setValidator(const Declaration* d, void(U::*validate)(const T*))
  {
    auto val = Validator(validate);
    if(!val)
      setValidator(d, nullptr);
    else
    {
      auto i = std::find_if(
        mValidators.begin(), mValidators.end(),
        [d](std::pair<const Declaration*, Validator>& v)
        { return v.first == d; }
      );
      if(i == mValidators.end())
        mValidators.push_back(std::make_pair(d, val));
      else
        i->second = val;
    }
  }

public:
  template <class T, class V, Declarator D>
  void setValidator(MemberProperty<T, D> V::*, void(U::*validate)(const T*))
  {
    setValidator<T>(D(), validate);
    BasicHolder::revalidate();
  }

  template <class T, class V, Declarator D>
  void setValidator(MemberProperty<T, D> V::*, nullptr_t)
  {
    setValidator(D(), nullptr);
    BasicHolder::revalidate();
  }

  template <class T, Declarator D>
  void setValidator(MemberProperty<T, D>& prop, void(U::*validate)(const T*))
  {
    setValidator<T>(D(), validate);
    prop.revalidate();
  }
  template<class T, Declarator D>
  void setValidator(MemberProperty<T, D>& prop, nullptr_t)
  {
    setValidator(D(), nullptr);
    prop.revalidate();
  }

protected:
  void onConstructionBegin(U* obj)
  {
    if(sClassState.Atomic().IfEqual(raw).Exchange(initializing) == raw)
    {
      sClassLock.Acquire();
      mClassInit = true;
      setHolderOffset(obj); 
    }
    else if(sClassState != initialized)
    {
      sClassLock.Acquire();
      sClassLock.Release();
    }
  }
  void onRegisterProperty(BasicProperty* p) override
  {
    if(mClassInit)
      sProperties().push_back(std::make_pair(p->declaration()->name, propertyOffset(p)));
  }
  void onConstructionEnd() override
  {
    if(mClassInit)
    {
      mClassInit = false;
      sClassState = initialized;
      sClassLock.Release();
    }
  }

  const Proplist& onProperties() const override { return sProperties(); }

  void onValidate(const BasicProperty* p, const void* newval) override
  {
    for(auto v : mValidators)
    {
      if(v.first == p->declaration())
      {
        if(v.second)
          (holderObject()->*(v.second))(newval);
        break;
      }
    }
  }

  void setHolderOffset(U* obj)
  {
    union { Holder* h; char* c; } this_ = { this };
    union { U* u; char* c; } obj_ = { obj };
    sHolderOffset = obj_.c - this_.c;
  }
  U* holderObject()
  {
    union { Holder* h; U* u; char* c; } ptr = { this };
    ptr.c += sHolderOffset;
    return ptr.u;
  }

private: // per-instance
  typedef void(U::*Validator)(const void*);
  std::vector<std::pair<const Declaration*, Validator>> mValidators;
  bool mClassInit;

private: // per-class
  static Proplist& sProperties()
  { static Proplist instance; SyncMemory<>(); return instance; }
  static Synchronized<ptrdiff_t> sHolderOffset;

  enum { raw = 0, initializing, initialized };
  static Synchronized<int> sClassState;
  static Mutex sClassLock;
};
template<class U> Mutex Holder<U>::sClassLock;
template<class U> Synchronized<int> Holder<U>::sClassState;
template<class U> Synchronized<ptrdiff_t> Holder<U>::sHolderOffset;

// MemberProperty must be instantiated with a unique declarator function
template<class T, Declarator D> class MemberProperty : public Property<T>
{
public:
  // default constructor implicitly called when Holder is default constructed
  MemberProperty() : Property<T>(D()) { clear(); }
  // copy constructor implicitly called when Holder is copy constructed from a Holder of same type,
  // and with same validators: keep property state, do not call any validators
  MemberProperty(const MemberProperty& other) : Property<T>(D()) { clone(other); }
  // assignment operator implicitly called when Holder is assigned from a Holder of same type:
  // and with same validators: keep property state, do not call any validators
  MemberProperty& operator=(const MemberProperty& other) { clone(other); return *this; }

  bool operator==(const MemberProperty& other) const { return Property<T>::operator==(other); }
  bool operator!=(const MemberProperty& other) const { return Property<T>::operator!=(other); }
};

class EndOfProperties
{
public:
  EndOfProperties() { BasicHolder::constructionEnd(); }
  EndOfProperties(const EndOfProperties&) { BasicHolder::constructionEnd(); }
  EndOfProperties& operator=(const EndOfProperties&) { return *this; }
};

class BasicContainer
{
private:
  BasicContainer(const BasicContainer&);
protected:
  BasicContainer();
  void changed(bool empty);
public:
  void setParentProperty(BasicProperty*);
  BasicProperty* parentProperty();
private:
  BasicProperty* mpParentProperty;
};

template<class T, bool = std::is_base_of<Holder<T>, T>::value> class Container;

template<class T>
class Container<T, true> : public BasicContainer
{
public:
  typedef T value_type;
  struct iterator
  {
    iterator(T** p) : p(p) {}
    T& operator*() const { return **p; }
    T* operator->() const { return *p; }
    iterator& operator++() { ++p; return *this; }
    iterator operator+(int i) const { return p + i; }
    bool operator!=(iterator i) const { return p != i.p; }
    private: T** p;
    friend class Container;
  };
  struct const_iterator
  {
    const_iterator(const T* const* p) : p(p) {}
    const T& operator*() const { return **p; }
    const T* operator->() const { return *p; }
    const_iterator& operator++() { ++p; return *this; }
    const_iterator operator+(int i) const { return p + i; }
    bool operator!=(const_iterator i) const { return p != i.p; }
    private: const T* const* p;
    friend class Container;
  };

  iterator begin() { return mData.data(); }
  iterator end() { return mData.data() + mData.size(); }
  const_iterator begin() const { return mData.data(); }
  const_iterator end() const { return mData.data() + mData.size(); }

  iterator insert(iterator i, const T& t)
  {
    auto it = mData.begin() + (i.p - mData.data());
    it = mData.insert(it, new T(t));
    (*it)->setParentProperty(this->parentProperty());
    changed(false);
    return mData.data() + (it - mData.begin());
  }
  iterator erase(iterator i)
  {
    auto it = mData.begin() + (i.p - mData.data());
    delete *it;
    it = mData.erase(it);
    changed(mData.empty());
    return mData.data() + (it - mData.begin());
  }

  T& front() { return *mData.front(); }
  const T& front() const { return *mData.front(); }
  void pop_front() { erase(begin()); }

  void push_back(const T& t) { insert(end(), t); }
  void pop_back() { erase(end() - 1); }
  T& back() { return *mData.back(); }
  const T& back() const { return *mData.back(); }

  T& operator[](size_t i) { return *mData[i]; }
  const T& operator[](size_t i) const { return *mData[i]; }
  void swap(size_t i, size_t j) { std::swap(mData[i], mData[j]); }

  bool empty() const { return mData.empty(); }
  size_t size() const { return mData.size(); }

  void clear() { do_clear(); changed(true); }
  void assign(const Container& c) { do_assign(c); changed(empty()); }

private:
  void do_clear() { for(auto p : mData) delete p; mData.clear(); }
  void do_assign(const Container& c) { do_clear(); for(auto p : c.mData) { mData.push_back(new T(*p)); mData.back()->setParentProperty(this->parentProperty()); } }

public:
  Container() {}
  Container(const Container& c) { assign(c); }
  ~Container() { do_clear(); }

  Container& operator=(const Container& c) { assign(c); return *this; }

  bool operator==(const Container& c) const
  {
    if(c.size() != size()) return false;
    for(auto i = begin(), j = c.begin(); i != end(); ++i, ++j)
      if(*i != *j) return false;
    return true;
  }
  bool operator!=(const Container& c) const { return !operator==(c); }

  std::ostream& serialize(std::ostream& os) const
  {
    os << "{\n";
    for(auto p : mData) os << *p << "\n";
    return os << "}\n";
  };
  std::istream& unserialize(std::istream& is)
  {
    clear();
    if((is >> std::ws).peek() == '{')
    {
      is.ignore();
      T t;
      while((is >> std::ws).peek() != '}' && is >> t)
        push_back(t);
      is.ignore();
    }
    else
      is.setstate(std::ios::failbit);
    return is;
  }

private:
  std::vector<T*> mData;
};

template <class T, bool H>
std::ostream& operator<<(std::ostream& os, const Container<T, H>& c) { return c.serialize(os); }
template <class T, bool H>
std::istream& operator>>(std::istream& is, Container<T, H>& c) { return c.unserialize(is); }

} // namespace Property

} // namespace Tiny

namespace Property = Tiny::Property;

#endif // TINY_PROPERTY_H
