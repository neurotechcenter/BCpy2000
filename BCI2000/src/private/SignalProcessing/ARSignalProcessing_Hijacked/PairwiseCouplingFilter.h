////////////////////////////////////////////////////////////////////////////////
// $Id: PairwiseCouplingFilter.h 3798 2012-02-01 18:07:06Z mellinger $
// Authors: jezhill@gmail.com
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
#ifndef PAIRWISECOUPLING_H
#define PAIRWISECOUPLING_H

#include "GenericFilter.h"
#include <vector>
#include <string>
#include <map>

class Group
{
public:
	Group() {}
	~Group() {}

	void SetName( std::string& groupName ) { mName = groupName; }
	unsigned int Inputs() { return mInputChannelIndices.size(); }
	unsigned int Outputs() { return mOutputChannelIndices.size(); }

	void AddSubProblem( unsigned int inputChannelIndex, std::string& neg, std::string& pos, double shift, double scale, unsigned int problemSize );
	void AddOutput( unsigned int outputChannelIndex, std::string& outputName );
	bool Finalize();
	void Process( const GenericSignal& Input, GenericSignal& Output );
	void PWC(); 

private:
	std::string mName;
	std::vector<unsigned int> mOrdering;
	std::vector<unsigned int> mInputChannelIndices;
	std::vector<double>       mShifts;
	std::vector<double>       mScales;
	std::vector<unsigned int> mProblemSizes;
	std::vector<std::string>  mNegNames;
	std::vector<std::string>  mPosNames;
	std::vector<std::string>  mOutNames;
	std::vector<unsigned int> mOutputChannelIndices;

	std::vector<double> pIn;
	std::vector<double >pOut;
	std::vector<double >nTrain;
	std::vector<double >mNumerator;
	std::vector<double >mDenominator;
};

class PairwiseCoupling : public GenericFilter
{
 public:
  PairwiseCoupling();
  virtual ~PairwiseCoupling();
  virtual void Preflight( const SignalProperties& Input, SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input, const SignalProperties& Output );
  virtual void SetupGroups( const SignalProperties& Input, std::map<std::string,Group>& groups, std::vector<std::string>& outNames ) const;
  virtual void StartRun();
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual void Halt();

private:
  bool                         mEnabled;
  std::map<std::string,Group>  mGroups;
};


#endif // PAIRWISECOUPLING_H
