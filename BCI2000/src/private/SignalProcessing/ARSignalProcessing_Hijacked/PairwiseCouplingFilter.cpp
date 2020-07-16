////////////////////////////////////////////////////////////////////////////////
// $Id: PairwiseCouplingFilter.cpp 3798 2012-02-01 18:07:06Z mellinger $
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
#include "PairwiseCouplingFilter.h"

#include <algorithm>
using namespace std;

RegisterFilter( PairwiseCoupling, 2.D0 );

PairwiseCoupling::PairwiseCoupling()
{
	mEnabled = false;

	BEGIN_PARAMETER_DEFINITIONS
		"Filtering:Pairwise%20Coupling int    EnablePairwiseCoupling= 0 0 0 1 // turn on pairwise coupling transformation (boolean) ",
		"Filtering:Pairwise%20Coupling matrix PairwiseCouplingMatrix= 3 "
			" { Input%20Channel     Group Negative%20Class Positive%20Class Shift Scaling Training%20Set%20Size } "
			"   IntentionVsMovement   P        Intention       Movement      0.0    1.0          100             "
			"   IntentionVsRest       P        Intention         Rest        0.0    1.0          100             "
			"   MovementVsRest        P         Movement         Rest        0.0    1.0          100             "
			" % % % // ",
		"Filtering:Pairwise%20Coupling matrix PairwiseCouplingTest= 0 0 0.5 % % // ",
	END_PARAMETER_DEFINITIONS
}

PairwiseCoupling::~PairwiseCoupling()
{
	Halt();
}

void
PairwiseCoupling::Halt()
{
	// Haende hoch!
}

void PairwiseCoupling::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
	Output = Input;
	bool enable = (int)Parameter( "EnablePairwiseCoupling" );
	if( !enable )
		return;
	map<string,Group> groups;
	vector<string> outNames;
	SetupGroups( Input, groups, outNames );
	Output.SetChannels( outNames.size() );
	for( unsigned int i = 0; i < outNames.size(); i++ )
		Output.ChannelLabels()[i] = outNames[i];
}

void
PairwiseCoupling::SetupGroups(const SignalProperties& Input, map<string,Group>& groups, vector<string>& outNames) const
{
	ParamRef matrix = Parameter( "PairwiseCouplingMatrix" );
	int nRows = matrix->NumRows();
	if( nRows == 0 )
	{
		bcierr << "PairwiseCouplingMatrix parameter must at least 1 row" << endl;
		return;
	}
	int nCols = 7;
	if( matrix->NumColumns() != nCols )
	{
		bcierr << "PairwiseCouplingMatrix parameter must have " << nCols << " columns" << endl;
		return;
	}
	for( int row = 0; row < nRows; row++ )
	{
		string chName      = matrix( row, 0 );
		string grpName     = matrix( row, 1 );
		string negName     = matrix( row, 2 );
		string posName     = matrix( row, 3 );
		double shift       = matrix( row, 4 );
		double scale       = matrix( row, 5 );
		double problemSize = matrix( row, 6 );

		double ch = Input.ChannelIndex( chName );
		int inputChannelIndex = (int) ( ch+0.5 );
		if( ::ceil( ch ) < 0 || inputChannelIndex >= Input.Channels() )
			bcierr << "entry \"" << chName << "\" at PairwiseCouplingMatrix(" << row+1 << ",1) is not a valid channel specification" << endl;
		if( problemSize < 2.0 || problemSize != floor( problemSize ) )
			bcierr << "invalid training-set size " << problemSize << " at PairwiseCouplingMatrix(" << row+1 << ",6): should be a whole number greater than 1" << endl;

		unsigned int negOutputChannelIndex;
		string extendedNegName = grpName + "." + negName;
		for( negOutputChannelIndex = 0; negOutputChannelIndex < outNames.size(); negOutputChannelIndex++ )
			if( outNames[negOutputChannelIndex] == extendedNegName ) break;
		if( negOutputChannelIndex == outNames.size() ) outNames.push_back( extendedNegName );

		unsigned int posOutputChannelIndex;
		string extendedPosName = grpName + "." + posName;
		for( posOutputChannelIndex = 0; posOutputChannelIndex < outNames.size(); posOutputChannelIndex++ )
			if( outNames[posOutputChannelIndex] == extendedPosName ) break;
		if( posOutputChannelIndex == outNames.size() ) outNames.push_back( extendedPosName );

		Group& g = groups[grpName];
		g.SetName( grpName );
		g.AddSubProblem( inputChannelIndex, negName, posName, shift, scale, (unsigned int)problemSize );
		g.AddOutput( negOutputChannelIndex, negName );
		g.AddOutput( posOutputChannelIndex, posName );

	}
	for( map<string,Group>::iterator i = groups.begin(); i != groups.end(); i++ )
		(*i).second.Finalize();

	ParamRef testVals = Parameter( "PairwiseCouplingTest" );
	if( testVals->NumValues() != 0 && testVals->NumRows() != nRows )
		bcierr << "if not empty, the PairwiseCouplingTest parameter must have " << nRows << "rows (to match the PairwiseCouplingMatrix parameter)" << endl;
}
void PairwiseCoupling::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
	mEnabled = (int)Parameter( "EnablePairwiseCoupling" );
	if( !mEnabled )
		return;

	mGroups.clear();
	vector<string> outNames;
	SetupGroups( Input, mGroups, outNames );

	ParamRef testVals = Parameter( "PairwiseCouplingTest" );
	if( testVals->NumValues() )
	{
		SignalProperties fakeInputProps = Input;
		fakeInputProps.SetElements( testVals->NumColumns() );
		GenericSignal fakeInputSig;
		fakeInputSig.SetProperties( fakeInputProps );
		SignalProperties fakeOutputProps = Output;
		fakeOutputProps.SetElements( testVals->NumColumns() );
		GenericSignal fakeOutputSig;
		fakeOutputSig.SetProperties( fakeOutputProps );
		
		for( int row = 0; row < testVals->NumRows(); row++)
			for( int col = 0; col < testVals->NumColumns(); col++)
				fakeInputSig( row, col ) = testVals( row, col );

		Process( fakeInputSig, fakeOutputSig );

		bciout << "PairwiseCouplingTest results: " << endl;
		for( int row = 0; row < fakeOutputSig.Channels(); row++)
		{
			bciout << fakeOutputProps.ChannelLabels()[row] << " = [";
			for( int col = 0; col < testVals->NumColumns(); col++)
				bciout << " " << fakeOutputSig( row, col ) << " ";
			bciout << "]" << endl;
		}
	}
}

void
PairwiseCoupling::StartRun()
{
	// Well, go ahead and start already.
}

void
PairwiseCoupling::Process( const GenericSignal& Input, GenericSignal& Output )
{
	if( !mEnabled )
	{
		Output = Input;
		return;
	}
	for( map<string,Group>::iterator it = mGroups.begin(); it != mGroups.end(); it++ )
		(*it).second.Process( Input, Output );
}


void
Group::AddSubProblem( unsigned int inputChannelIndex, string& negName, string& posName, double shift, double scale, unsigned int problemSize )
{
	for( unsigned int i = 0; i < Inputs(); i++ )
	{
		if( mInputChannelIndices[i] == inputChannelIndex )
		{
			bcierr << "duplicate entries for input channel #" << inputChannelIndex+1 << " in PairwiseCouplingMatrix group \"" << mName << "\"" << endl;
			return;
		}
		if( ( negName == mNegNames[i] && posName == mPosNames[i] ) || ( negName == mPosNames[i] && posName == mNegNames[i] ) )
		{
			bcierr << "duplicate entries for the " << negName << "-vs-" << posName << " subproblem in PairwiseCouplingMatrix group \"" << mName << "\"" << endl;
			return;
		}
	}
	mInputChannelIndices.push_back( inputChannelIndex );
	mNegNames.push_back( negName );
	mPosNames.push_back( posName );
	mShifts.push_back( shift );
	mScales.push_back( scale );
	mProblemSizes.push_back( problemSize );
}

void
Group::AddOutput( unsigned int outputChannelIndex, string& outputName )
{
	for( unsigned int i = 0; i < Outputs(); i++ )
	{
		if( mOutputChannelIndices[i] == outputChannelIndex || mOutNames[i] == outputName )
		{
			if( mOutputChannelIndices[i] != outputChannelIndex )
				bcierr << "oops, this shouldn't happen: output \"" << outputName << "\" has been assigned conflicting channel numbers " << outputChannelIndex+1 << " and " << mOutputChannelIndices[i]+1 << " in PairwiseCouplingMatrix group \"" << mName << "\"" << endl;
			if( mOutNames[i] != outputName )
				bcierr << "oops, this shouldn't happen: outputs \"" << outputName << "\" and \"" << mOutNames[i] << "\" have been assigned the same channel number " << outputChannelIndex+1 << " in PairwiseCouplingMatrix group \"" << mName << "\"" << endl;
			return;
		}
	}
	mOutputChannelIndices.push_back( outputChannelIndex );
	mOutNames.push_back( outputName );
}

bool
Group::Finalize()
{
	bool good = true;
	mOrdering.clear();
	for( unsigned int negInd = 0; negInd < Outputs(); negInd++ )
	{
		string negName = mOutNames[negInd];
		for( unsigned int posInd = negInd+1; posInd < Outputs(); posInd++ )
		{
			string posName = mOutNames[posInd];
			unsigned int match;
			for( match = 0; match < Inputs(); match++ )
			{
				if( negName == mNegNames[match] && posName == mPosNames[match] )
					break;
				if( negName == mPosNames[match] && posName == mNegNames[match] )
				{
					string swap = mPosNames[match];
					mPosNames[match] = mNegNames[match];
					mNegNames[match] = swap;
					mScales[match] *= -1.0;
					break;
				}
			}
			if( match < Inputs() )
				mOrdering.push_back( match );
			else
			{
				bcierr << "PairwiseCouplingMatrix group \"" << mName << "\" is lacking information about the " << negName << "-vs-" << posName << " subproblem" << endl;
				good = false;
			}
		}
	}
	int nExpected = (Outputs() * (Outputs()-1))/2;
	if( Inputs() !=  nExpected )
	{
		bcierr << "for the " << Outputs() << "-class problem of PairwiseCouplingMatrix group \"" << mName << "\", " << nExpected << " inputs were expected, but " << Inputs() << " were specified" << endl;
		good = false;
	}

	vector<double> zIn( Inputs() );
	vector<double> zOut( Outputs() );

	nTrain = zIn;
	pIn = zIn;
	pOut = zOut;
	mNumerator = zOut;
	mDenominator = zOut;

	return good;
}

void
Group::PWC( )
{
	unsigned int nSubproblems = pIn.size();
	unsigned int nClasses = pOut.size();
	
	bool converged;
	vector<double> pPrev;

	for( unsigned int i = 0; i < nClasses; i++ )
		pOut[i] = 1.0 / nClasses;

	do {
		converged = true;
		pPrev = pOut;
		for( unsigned int i = 0; i < nClasses; i++ )
			mNumerator[i] = mDenominator[i] = 0.0;
		unsigned int k = 0;
		for( unsigned int i = 0; i < nClasses; i++ )
		{
			for( unsigned int j = i+1; j < nClasses; j++ )
			{
				double r_ij = 1.0 - pIn[k]; // in H&T notation r[i,j] = Pr(i | i or j), whereas our convention is to mention the negative class first and quote the probability of the positive class: neg=i,pos=j gives Pr(j | i or j), i.e. 1-r[i,j]
				double r_ji = pIn[k];
				double n_ij = nTrain[k];
				double n_ji = nTrain[k];
				double mu_ij = pOut[i] / (pOut[i] + pOut[j]);
				double mu_ji = 1.0 - mu_ij;

				mNumerator[i] += n_ij * r_ij;
				mNumerator[j] += n_ji * r_ji;
				mDenominator[i] += n_ij * mu_ij;
				mDenominator[j] += n_ji * mu_ji;
				k++;
			}
		}
		double total = 0.0;
		for( unsigned int i = 0; i < nClasses; i++ )
		{
			if( mNumerator[i] == 0.0) mDenominator[i] = 1.0;
			pOut[i] *= mNumerator[i] / mDenominator[i];
			total += pOut[i];
		}
		for( unsigned int i = 0; i < nClasses; i++ )
		{
			pOut[i] /= total;
			if( fabs(pOut[i] - pPrev[i]) >= 1e-5 )
				converged = false;
		}
	} while( !converged );
}

void
Group::Process( const GenericSignal& InputSignal, GenericSignal& OutputSignal )
{
	//bciout << InputSignal.Channels() << "x" << InputSignal.Elements() << " -> " << OutputSignal.Channels() << "x" << OutputSignal.Elements() << endl;
	unsigned int nEls = InputSignal.Elements();
	unsigned int nSubproblems = Inputs();
	unsigned int nClasses = Outputs();

	for( unsigned int el = 0; el < nEls; el++ )
	{
		for( unsigned int iSubproblem = 0; iSubproblem < nSubproblems; iSubproblem++ )
		{
			unsigned int reordered = mOrdering[iSubproblem];
			unsigned int ch = mInputChannelIndices[reordered];
			double shift = mShifts[reordered];
			double scale = mScales[reordered];
			nTrain[iSubproblem] = mProblemSizes[reordered];

			double dv = InputSignal( ch, el );
			dv -= shift;
			dv *= scale;
			pIn[iSubproblem] = 1.0 / ( 1.0 + exp( -dv ) );
		}
		if( nClasses == 2 && nSubproblems == 1 )
		{
			pOut[0] = pIn[0];
			pOut[1] = 1.0 - pIn[0];
		}
		else
		{
			PWC();
		}
		for( unsigned int iClass = 0; iClass < nClasses; iClass++ )
			OutputSignal( mOutputChannelIndices[iClass], el ) = pOut[iClass];
	}
}
