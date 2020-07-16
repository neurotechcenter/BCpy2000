//////////////////////////////////////////////////////////////////////////////////////
// $Id: ExpressionNodes.cpp 5719 2018-03-17 11:17:21Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A collection of classes that represent nodes of parsed expressions.
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

#include "ExpressionNodes.h"
#include "BCIException.h"
#include "Debugging.h"
#include "Numeric.h"
#include "ClassName.h"
#include <sstream>

using namespace std;
using namespace ExpressionParser;

RefObj::~RefObj()
{
  if( mRefs )
    throw std_logic_error << "Deleting object with " << mRefs << " references";
}

// NodeList
NodeList& 
NodeList::Add( Node* node )
{
  if( node )
    mList.push_back( node );
  return *this;
}

NodeList& 
NodeList::Add( Node* node, size_t idx )
{
  if( node )
    mList.insert( mList.begin() + idx, node );
  return *this;
}

NodeList& 
NodeList::Append( const NodeList& other )
{
  mList.insert( mList.end(), other.mList.begin(), other.mList.end() );
  return *this;
}

Node*
NodeList::operator[]( size_t idx ) const
{
  const Node* p = mList[idx];
  return const_cast<Node*>( p );
}


// Node
Node::Node( bool isConst )
: mIsConst( isConst )
{
  mLocation.end = 0;
  mLocation.begin = mLocation.end;
}

Node::Node( const NodeList& inList )
: mIsConst( false )
{
  mLocation.end = 0;
  mLocation.begin = mLocation.end;
  AddChildren( inList );
}

Node::~Node()
{
}

Node*
Node::Simplify()
{
  for( size_t i = 0; i < mChildren.size(); ++i )
    mChildren[i] = mChildren[i]->Simplify();
  return OnSimplify();
}

void
Node::AddChild( Node* p )
{
  if( p )
    mChildren.push_back( p );
}

void
Node::AddChildren( const NodeList& inList )
{
  for( int i = 0; i < inList.Size(); ++i )
    AddChild( inList[i] );
}

void
Node::SetLocation(size_t begin, size_t end)
{
  mLocation.begin = begin;
  mLocation.end = end;
}

// ExpressionNodes::ConstPropagatingNode
Node*
ConstPropagatingNode::OnSimplify()
{
  if( mPropagate )
  {
    bool allChildrenConst = true;
    for( size_t i = 0; i < mChildren.size(); ++i )
      allChildrenConst &= mChildren[i]->IsConst();
    if( allChildrenConst )
      return new ConstantNode( this->Evaluate() );
  }
  return this;
}

// ExpressionNodes::StringNode
double
StringNode::OnEvaluate()
{
  return NaN<double>();
}

// ExpressionNodes::AddressNode
AddressNode::AddressNode( Node* n1, Node* n2 )
: StringNode( "" )
{
  AddChild( n1 );
  AddChild( n2 );
}

double
AddressNode::OnEvaluate()
{
  if( !mChildren.empty() )
  {
    mString.clear();
    for( size_t i = 0; i < mChildren.size(); ++i )
    {
      Node* p = mChildren[i];
      StringNode* s = dynamic_cast<StringNode*>( p );
      if( s )
        mString += s->Evaluate();
      else
      {
        ostringstream oss;
        oss << p->Evaluate();
        mString += oss.str();
      }
    }
  }
  return NaN<double>();
}

Node*
AddressNode::OnSimplify()
{
  mIsConst = true;
  for( size_t i = 0; i < mChildren.size(); ++i )
    mIsConst &= mChildren[i]->IsConst();
  if( mIsConst )
  {
    Evaluate();
    mChildren.clear();
  }
  return this;
}

