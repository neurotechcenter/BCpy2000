////////////////////////////////////////////////////////////////////////////////
//  $Id: DataSource.h 3798 2012-02-01 18:07:06Z mellinger $
//  Author: juergen.mellinger@uni-tuebingen.de
//  Description: A base class for objects that provide signal data to other
//    objects, and that depend on such data.
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
#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include "GenericSignal.h"
#include "Expression.h"
#include <string>
#include <vector>

class DataSource
{
 public:
  typedef std::vector<int> IndexList;
  typedef GenericSignal::ValueType Value;

  class SourceList : public std::vector<DataSource*>
  {
   public:
    DataSource* Find( const std::string& name ) const;
    bool Contains( DataSource* );
    SourceList& Add( DataSource* );
    SourceList& AddUnique( DataSource* );
  };

  struct Dimension
  {
    Dimension() : streaming( false ), continuous( false ) {}
    int Size() const { return labels.Size(); }
    bool operator==( const Dimension& ) const;
    std::string name;
    PhysicalUnit unit;
    LabelIndex labels;
    bool streaming,
         continuous;
  };

  typedef std::vector<Dimension> DimensionList;

  class DataProperties
  {
   public:
    DataProperties() {}
    DataProperties( const SignalProperties& );
    operator SignalProperties() const;

    const std::string& Name() const { return mName; }
    std::string& Name() { return mName; }
    const DimensionList& Dimensions() const { return mDimensions; }
    DimensionList& Dimensions() { return mDimensions; }
    const PhysicalUnit& Unit() const { return mUnit; }
    PhysicalUnit& Unit() { return mUnit; }

    DataProperties& OuterProduct( const DataProperties& );

    int DataSize() const;
    int ToSingleIndex( const IndexList& ) const;
    IndexList ToIndexList( int ) const;

    bool Compatible( const DataProperties& ) const;

   private:
    std::string mName;
    DimensionList mDimensions;
    PhysicalUnit mUnit;
  };

  struct Context
  {
    enum { preflight, process } type;
    const GenericSignal* signal;
    SourceList* sources;
    Expression::VariableContainer* variables;
  };

  DataSource( const std::string& inName ) : mName( inName ) {}
  virtual ~DataSource() {}

  // Properties
  const std::string& Name() const { return mName; }
  const SourceList& Sources() const { return mSources; }
  const DataProperties& Properties() const { return mProperties; }
  // Two-step initialization:
  // Depends determines dependencies starting from a single DataSource
  // object, and adding its dependencies to a list of sources.
  // From that list, Initialize() and Process() may then be called in order to
  // fulfil dependencies.
  void Depends( const Context&, SourceList& outList, SourceList inList = SourceList() );
  void Initialize( const Context& );
  void Process( const Context& );
  Value Data( const IndexList& );
  Value Data( int );
  // The abort flag is tested frequently, and synchronization is not critical.
  // It is thus not protected by a mutex.
  static void SetAbortFlag( bool inAbort = true ) { sAbortFlag = inAbort; }
  static bool AbortFlag() { return sAbortFlag; }

 protected:
  // Interface to descendants.
  void Add( DataSource* p ) { mSources.AddUnique( p ); }

  virtual void OnDepends( const Context& ) {}
  virtual void OnInitialize( const Context& ) = 0;
  virtual void OnProcess( const Context& ) = 0;
  // Access to data takes a single index, which may be obtained from multiple
  // indices using DataProperties::CalculateIndex().
  virtual Value OnData( int ) = 0;

 protected:
  DataProperties mProperties;
  static bool sAbortFlag;
 private:
  std::string mName;
  SourceList mSources;
};

#endif // DATA_SOURCE_H
