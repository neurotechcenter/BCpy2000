////////////////////////////////////////////////////////////////////////////////
// $Id: ParamList.cpp 5665 2017-07-22 19:10:58Z mellinger $
// Authors: gschalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: ParamList is a BCI2000 type that represents a collection of
//   parameters.
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
#include "ParamList.h"
#include "Debugging.h"

#include <vector>
#include <map>
#include <algorithm>

struct ParamList::Private
{
  struct ParamEntry
  {
    ParamEntry() : SortingHint(0.0) {}
    class Param Param;
    float SortingHint;
    static bool Compare( const ParamEntry* p, const ParamEntry* q )
      { return p->SortingHint < q->SortingHint; }
  };
  typedef std::map<std::string, ParamEntry, Param::NameCmp> ParamContainer;
  ParamContainer mParams;
  typedef std::vector<ParamEntry*> Index;
  Index mIndex;

  void BuildIndex()
  {
    mIndex.clear();
    for( ParamContainer::iterator i = mParams.begin(); i != mParams.end(); ++i )
      mIndex.push_back( &i->second );
  }

  void AssignFrom(const Private* other)
  {
    mParams = other->mParams;
    BuildIndex();
  }
};

ParamList::ParamList()
: p(new Private)
{}

ParamList::~ParamList()
{
  delete p;
}

ParamList::ParamList( const ParamList& other )
: p(new Private)
{
  p->AssignFrom(other.p);
}

ParamList::ParamList(const ParamListView& view)
: p(new Private)
{
  ParamListView::assign(view);
}

ParamList&
ParamList::operator=( const ParamList& other )
{
  p->AssignFrom(other.p);
  return *this;
}

ParamList&
ParamList::operator=(const ParamListView& view)
{
  ParamListView::assign(view);
  return *this;
}

int
ParamList::onCount() const
{
  return p->mParams.size();
}

Param*
ParamList::onInsert(const Param& inParam, float inSortingHint)
{
  Private::ParamEntry* pEntry = nullptr;
  auto i = p->mParams.find(inParam.Name());
  if(i == p->mParams.end())
  {
    pEntry = &p->mParams[inParam.Name()];
    p->mIndex.push_back(pEntry);
  }
  else
  {
    pEntry = &i->second;
  }
  pEntry->Param = inParam;
  pEntry->SortingHint = inSortingHint;
  Assert(p->mIndex.size() == p->mParams.size());
  return &pEntry->Param;
}

const Param*
ParamList::onGet( size_t index ) const
{
  return &p->mIndex.at( index )->Param;
}

Param*
ParamList::onGet( size_t index )
{
  return &p->mIndex.at( index )->Param;
}

Param*
ParamList::onGet( const std::string& inName )
{
  Private::ParamContainer::iterator i = p->mParams.find( inName );
  return i != p->mParams.end() ? &i->second.Param : nullptr;
}

const Param*
ParamList::onGet( const std::string& inName ) const
{
  Private::ParamContainer::const_iterator i = p->mParams.find( inName );
  return i != p->mParams.end() ? &i->second.Param : nullptr;
}

bool
ParamList::onRemove(const std::string& inName)
{
  Private::ParamContainer::iterator i = p->mParams.find( inName );
  if( i != p->mParams.end() )
  {
    Private::Index::iterator j = p->mIndex.begin();
    while( j != p->mIndex.end() && *j != &i->second )
      ++j;
    Assert( j != p->mIndex.end() );
    p->mIndex.erase( j );
    p->mParams.erase( i );
    return true;
  }
  return false;
}

void
ParamList::onClear()
{
  p->mIndex.clear();
  p->mParams.clear();
}

void
ParamList::onSort()
{
  // stable_sort will retain the relative order of parameters with identical
  // sections.
  stable_sort( p->mIndex.begin(), p->mIndex.end(), Private::ParamEntry::Compare );
}

void
ParamList::onSetUnchanged()
{
  for( Private::Index::const_iterator i = p->mIndex.begin(); i != p->mIndex.end(); ++i )
    (*i)->Param.Unchanged();
}

