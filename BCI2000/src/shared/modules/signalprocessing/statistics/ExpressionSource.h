////////////////////////////////////////////////////////////////////////////////
//  $Id: ExpressionSource.h 3798 2012-02-01 18:07:06Z mellinger $
//  Author: juergen.mellinger@uni-tuebingen.de
//  Description: A data source that computes its data from an expression.
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
#ifndef EXPRESSION_SOURCE_H
#define EXPRESSION_SOURCE_H

#include "DataSource.h"
#include "Expression.h"
#include <string>

class ExpressionSource : public DataSource, private Expression
{
 public:
  typedef DataSource::Context Context;
  typedef ExpressionParser::Node Node;
  typedef ExpressionParser::NodeList NodeList;

  ExpressionSource( const std::string& );

 // DataSource interface
 protected:
  void OnDepends( const Context& );
  void OnInitialize( const Context& );
  void OnProcess( const Context& );
  Value OnData( int );

 // ArithmeticExpression interface
 protected:
  Node* Variable( const std::string& );
  Node* Function( const std::string&, const NodeList& );
  Node* MemberFunction( const std::string&, const std::string&, const NodeList& );

 private:
  Node* FindObserver( const std::string& ) const;

 private:
  std::string mExpression;
  const Context* mpContext;
  int mDataIndex;
};

#endif // EXPRESSION_SOURCE_H
