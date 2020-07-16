////////////////////////////////////////////////////////////////////////////////
// $Id: FunctionSource.h 3811 2012-02-21 15:27:09Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A data source that represents an observer function call.
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
#ifndef FUNCTION_SOURCE_H
#define FUNCTION_SOURCE_H

#include "DataSource.h"
#include "ExpressionNodes.h"
#include "StatisticalObserver.h"
#include <vector>

class ObserverSource;

class FunctionSource : public DataSource, public ExpressionParser::Node
{
 public:
  typedef ExpressionParser::Node Node;
  typedef ExpressionParser::NodeList NodeList;

  static FunctionSource* NewFunctionSource( const std::string&, const NodeList&, const int& );
  FunctionSource( const std::string& name, int functionID, const NodeList&, const int& );

  // DataSource interface
 protected:
  void OnInitialize( const Context& );
  void OnProcess( const Context& );
  Value OnData( int );

  // Node interface
 protected:
  double OnEvaluate();

 private:
  void Compute( int );
  void CheckSizeArg( int pos, int value, int size = -1 ) const;

  enum { TypeNone, TypeNumber, TypeVector, TypeSymMatrix, TypeMatrix };
  int mFunctionID,
      mResultSize,
      mIndexArgs,
      mReturnType;
  DataProperties mResultProperties;
  std::vector<ObserverSource*> mObservers;
  const int& mrDataIndex;

  IndexList mResultIdx;
  std::vector<double> mArgBuffer;
  bool mArgsChanged;
  int mPreviousIndex;
  StatisticalObserver::MemPool mMemPool;
  StatisticalObserver::Number mNumberResult;
  // MemPool must be listed before VectorPtr and MatrixPtr objects
  // because their destructors depend on the MemPool object.
  StatisticalObserver::VectorPtr mVectorResult;
  StatisticalObserver::MatrixPtr mMatrixResult;
};

#endif // _SOURCE_H
