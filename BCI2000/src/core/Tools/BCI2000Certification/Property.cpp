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
#include "Property.h"
#include "ThreadLocal.h"
#include <queue>
#include <stack>

namespace Tiny { namespace Property {

namespace {
  ThreadLocalObject<std::stack<BasicHolder*>> stHolderStack;
  ThreadLocalObject<std::stack<BasicProperty*>> stPropertyStack;

  ThreadLocal<const BasicProperty*> stValidand;
  ThreadLocalObject<std::ostringstream> stValidationError;
  ThreadLocalObject<std::list<BasicProperty*>> stRevalidateLater;
}

BasicProperty::BasicProperty(void* data, const Declaration* decl)
: mpDecl(decl), mpData(data), mEmpty(true), mValid(true),
  mpHolder(BasicHolder::currentlyConstructed())
{
  Assert(!stPropertyStack->empty());
  stPropertyStack->pop();
  stPropertyStack->push(this);
  Assert(mpHolder);
  mpHolder->registerProperty(this); 
}

BasicProperty::~BasicProperty()
{
  auto i = std::find(stRevalidateLater->begin(), stRevalidateLater->end(), this);
  if(i != stRevalidateLater->end())
    stRevalidateLater->erase(i);
}

void BasicProperty::clone(const BasicProperty& other)
{
  mEmpty = other.mEmpty;
  mValid = other.mValid;
  mError = other.mError;
}

int BasicProperty::state() const
{
  int s = 0;
  if(!mEmpty)
    s |= NotEmpty;
  if(!mValid)
    s |= Invalid;
  return s;
}

bool
BasicProperty::revalidate()
{
  return validate(mEmpty ? nullptr : mpData);
}

void
BasicProperty::revalidateLater()
{
  if(!stValidand)
    revalidate();
  else if(stValidand != this
    && std::find(stRevalidateLater->begin(), stRevalidateLater->end(), this) == stRevalidateLater->end() )
    stRevalidateLater->push_back(this);
}

void
BasicProperty::doSetValue(const void* p)
{
  validate(p);
  if(!p)
    onClear(mpData);
  else
    onAssign(mpData, p);
  mEmpty = !p;

  while(!stRevalidateLater->empty())
  {
    auto v = stRevalidateLater->front();
    stRevalidateLater->pop_front();
    v->revalidate();
  }
}

bool
BasicProperty::validate(const void* p)
{
  mValid = mpHolder->validate(this, p, mError);
  return mValid;
}

BasicProperty&
BasicProperty::setError(const String& err)
{
  mValid = false;
  mError = err;
  return *this;
}

BasicProperty&
BasicProperty::assign(const BasicProperty& other)
{
  if(&other != this)
    set(other.toString());
  return *this;
}

bool BasicProperty::equal(const BasicProperty& other) const
{
  if(empty() != other.empty())
    return false;
  if(empty() && other.empty())
    return true;
  return onEqual(mpData, other.mpData);
}

BasicProperty& BasicProperty::set(const String& newprop)
{
  if(newprop.empty())
  {
    doSetValue(nullptr);
  }
  else if(!onSet(newprop))
  {
    std::istringstream iss(newprop);
    mError.clear();
    void* tmp = onUnserialize(iss);
    if(!iss && iss.fail() || !(iss >> std::ws).eof())
    {
      mValid = false;
      if(mError.empty())
        mError = "unexpected character(s)";
    }
    else
    {
      doSetValue(tmp);
      onDispose(tmp);
    }
  }
  return *this;
}

String BasicProperty::toString() const
{
  if(empty())
    return "";
  std::ostringstream oss;
  onSerialize(oss, mpData);
  return oss.str();
}

BasicProperty& BasicProperty::clear()
{
  set("");
  return *this;
}

// BasicHolder
BasicHolder::~BasicHolder()
{
  Assert(mConstructionFinished);
  Assert(stHolderStack->empty() || stHolderStack->top() != this);
}

BasicHolder::BasicHolder()
: mConstructionFinished(false),
  mpParentProperty(nullptr)
{
  if(!stPropertyStack->empty())
    mpParentProperty = stPropertyStack->top();
  stHolderStack->push(this);
  stPropertyStack->push(nullptr);
}

BasicHolder* BasicHolder::currentlyConstructed()
{
  Assert(!stHolderStack->empty());
  return stHolderStack->top();
}

void BasicHolder::constructionEnd()
{
  auto this_ = currentlyConstructed();
  Assert(!this_->mConstructionFinished);
  this_->mConstructionFinished = true;
  this_->onConstructionEnd();
  stHolderStack->pop();
  Assert(!stPropertyStack->empty());
  stPropertyStack->pop();
}

void BasicHolder::registerProperty(BasicProperty* p)
{
  Assert(!mConstructionFinished);
  onRegisterProperty(p);
}

BasicProperty*
BasicHolder::parentProperty()
{
  return mpParentProperty;
}

void
BasicHolder::setParentProperty(BasicProperty* p)
{
  mpParentProperty = p;
}

BasicProperty*
BasicHolder::property(const Tiny::String& name)
{
  ptrdiff_t offset = -1;
  for(auto p : onProperties())
    if(name == p.first)
      offset = p.second;
  return offset < 0 ? nullptr : propertyAddress(offset);
}

const BasicProperty*
BasicHolder::property(const Tiny::String& name) const
{
  return const_cast<BasicHolder*>(this)->property(name);
}

std::vector<BasicProperty*>
BasicHolder::properties()
{
  std::vector<BasicProperty*> props;
  for(auto p : onProperties())
    props.push_back(propertyAddress(p.second));
  return props;
}

std::vector<const BasicProperty*>
BasicHolder::properties() const
{
  std::vector<const BasicProperty*> props;
  for(auto p : onProperties())
    props.push_back(propertyAddress(p.second));
  return props;
}

bool
BasicHolder::valid() const
{
  for(auto p : properties())
    if(!p->valid())
      return false;
  return true;
}

bool
BasicHolder::revalidate()
{
  bool valid = true;
  for(auto p : properties())
    if(!p->revalidate())
      valid = false;
  return valid;
}

BasicProperty*
BasicHolder::propertyAddress(ptrdiff_t offset) const
{
  union { const BasicHolder* h; char* c; BasicProperty* p; } addr = { this };
  addr.c += offset;
  return addr.p;
}

ptrdiff_t
BasicHolder::propertyOffset(const BasicProperty* p) const
{
  union { const BasicHolder* h; const char* c; } this_ = { this };
  union { const BasicProperty* p; const char* c; } prop_ = { p };
  return prop_.c - this_.c;
}

bool
BasicHolder::validate(const BasicProperty* p, const void* newval, std::string& err)
{
  stValidationError->str("");
  stValidationError->clear();
  stValidand = p;
  onValidate(p, newval);
  stValidand = nullptr;
  err = stValidationError->str();
  if(mpParentProperty)
  {
    std::string err2;
    mpParentProperty->holder()->validate(p, newval, err2);
    if(!err2.empty())
      err += (err.empty() ? "" : "\n") + err2;
  }
  return err.empty();
}

const BasicProperty*
BasicHolder::validand() const
{
  return stValidand;
}

std::ostream&
BasicHolder::validationError()
{
  return *stValidationError;
}

bool BasicHolder::equalSameType(const BasicHolder& other) const
{
  for(auto p : onProperties())
  {
    const auto p1 = propertyAddress(p.second), p2 = other.propertyAddress(p.second);
    if(!p1->equal(*p2))
      return false;
  }
  return true;
}

//
BasicContainer::BasicContainer()
: mpParentProperty(nullptr)
{
}

void BasicContainer::setParentProperty(BasicProperty* p)
{
  mpParentProperty = p;
}

BasicProperty*
BasicContainer::parentProperty()
{
  return mpParentProperty;
}

void BasicContainer::changed(bool empty)
{
  if(mpParentProperty)
  {
    mpParentProperty->mEmpty = empty;
    mpParentProperty->revalidate();
  }
}

} } // namespace

