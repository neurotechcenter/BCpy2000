////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: gschalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: ParamListView represents a (possibly filtered) view on a
//   collection of parameters.
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
#ifndef PARAM_LIST_VIEW_H
#define PARAM_LIST_VIEW_H

#include <string>
#include "Param.h"

class ParamRef;
class MutableParamRef;

class ParamListView
{
public:
  virtual ~ParamListView() {}

  ParamListView& operator=(const ParamListView& other)
    { return assign(other); }
  const Param& operator[](const std::string& name) const
    { return ByName(name); }
  Param& operator[](const std::string& name)
    { return ByName(name); }
  const Param& operator[](size_t index) const
    { return ByIndex(index); }
  Param&  operator[](size_t index)
    { return ByIndex(index); }

  MutableParamRef operator()(const std::string& name);
  ParamRef        operator()(const std::string& name) const;

  ParamListView& assign(const ParamListView& other);
  int count() const { return onCount(); }
  bool empty() const { return onCount() < 1; }
  void clear() { onClear(); }
  Param* get(const std::string& name) { return onGet(name); }
  const Param* get(const std::string& name) const { return onGet(name); }
  Param* get(size_t idx) { return onGet(idx); }
  const Param* get(size_t idx) const { return onGet(idx); }
  Param* insert(const Param& param, float hint) { return onInsert(param, hint); }
  bool remove(const std::string& name) { return onRemove(name); }
  void sort() { onSort(); }
  void setUnchanged() { onSetUnchanged(); }

  int Size() const { return count(); }
  bool Empty() const { return count() < 1; }
  void Clear() { clear(); }

  bool Exists(const std::string& name) const { return get(name); }

  const Param& ByName(const std::string& name) const;
        Param& ByName(const std::string& name);
  const Param& ByIndex( size_t index ) const { return *get(index); }
        Param& ByIndex( size_t index ) { return *get(index); }

  void Add( const Param& p, float sortingHint = 0.0 );
  bool Add( const std::string& paramDefinition );
  void Delete( const std::string& name ) { remove(name); }

  bool Save( const std::string& filename ) const;
  bool Load( const std::string& filename,
                bool importNonexisting = true );

  void Sort() { sort(); }
  void Unchanged() { setUnchanged(); }

  // These contain all formatted I/O functionality.
  std::ostream& InsertInto( std::ostream& ) const;
  std::istream& ExtractFrom( std::istream& );

  // These define binary I/O.
  std::ostream& Serialize( std::ostream& ) const;
  std::istream& Unserialize( std::istream& );

 private:
  // Range-based for loop support
  struct const_iterator
  {
    const_iterator( const ParamListView* p, int i ) : p(p), i(i) {}
    void operator++() { ++i; }
    const Param& operator*() const { return p->ByIndex(i); }
    bool operator!=( const_iterator other ) { return i != other.i; }
    const ParamListView* const p;
    int i;
  };
 public:
  const_iterator begin() const { return const_iterator(this, 0); }
  const_iterator end() const { return const_iterator(this, Size()); }

 protected:
  virtual int onCount() const = 0;
  virtual Param* onInsert(const Param&, float) = 0;
  virtual bool onRemove(const std::string&) = 0;
  virtual Param* onGet(const std::string&) = 0;
  virtual const Param* onGet(const std::string&) const = 0;
  virtual Param* onGet(size_t) = 0;
  virtual const Param* onGet(size_t) const = 0;
  virtual void onClear() = 0;
  virtual void onSort() = 0;
  virtual void onSetUnchanged() = 0;
};

inline
std::ostream& operator<<( std::ostream& s, const ParamListView& p )
{ return p.InsertInto( s ); }

inline
std::istream& operator>>( std::istream& s, ParamListView& p )
{ return p.ExtractFrom( s ); }


class ParamListFilter : public ParamListView
{
public:
  ParamListFilter();
  void setParentView(ParamListView*);
  const ParamListView* parentView() const;
  ParamListView* parentView();

 protected:
  virtual bool onApply(std::string& name);
  virtual bool onApply(std::string& name) const = 0;

  int onCount() const override;
  Param* onInsert(const Param&, float) override;
  bool onRemove(const std::string& name) override;
  Param* onGet(const std::string& name) override;
  const Param* onGet(const std::string& name) const override;
  Param* onGet(size_t) override;
  const Param* onGet(size_t) const override;
  void onClear() override;
  void onSort() override;
  void onSetUnchanged() override;

 private:
  ParamListView* mpParentView;
};

#endif // PARAM_LIST_VIEW_H

