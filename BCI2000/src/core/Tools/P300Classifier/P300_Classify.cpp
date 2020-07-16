#include "P300_Classify.h"
#include <cassert>
#include <set>
#include <limits>

///////////////////////////////////////////////////////////////////
/// Predict the intended letters according to the pscore matrix.
/// This function assumes that the data was recorded under the P300
/// Speller paradigm.  
/// @param [in] pscore				Matrix containing the score values for each row and column
/// @param [in] Code				ste Stimulus Code
/// @param [in] Type				ste Stimulus Type
/// @param [in] trialnr				ste trialnr
///	@param [in] NumberOfSequences	Number of sequences. Default = 15;
///	@param [in] NumberMatrixRows	Number of rows for the P300 Speller
///	@param [in] NumberMatrixColumns	Number of columns for the P300 Speller
///	@param [in] TargetDefinitions	Vector containing the characters from the P300 Speller									
///	@param [out] result				Vector containing the performances. The values are given in percentages
///	@param [out] predicted			Vecto containing the predicted character
/// \author Cristhian Potes
/// \date June 29, 2009

void P300_Classify(ap::real_2d_array pscore, 
				   ap::template_1d_array<unsigned short int, true>& StimulusCode,
				   ap::template_1d_array<unsigned short int, true>& StimulusType,
				   ap::template_1d_array<short int, true>& trialnr,
				   int NumberOfSequences,
				   int NumMatrixRows,
				   int NumMatrixColumns,
				   vector<string> TargetDefinitions,
				   vector<double>& result,
				   vector<string>& predicted)
{
// jm, Oct 24, 2012
if( NumMatrixRows < 2 ) NumMatrixRows = 0;
if( NumMatrixColumns < 2 ) NumMatrixColumns = 0;
////////////////////////////////////////////////////////////////////////
// Section: Define variables
int choice, epoch;
bool canpredict = true;
static const double inf = std::numeric_limits<double>::infinity();
double val, max_value_row= -inf, max_value_col= -inf, numletters, correct;
ap::real_2d_array cflash;
ap::template_2d_array<int, true> predictedcol;
ap::template_2d_array<int, true> predictedrow;
vector<int> range;
vector<int> codes;
vector<int> codecol;
vector<int> coderow;
vector<int>::iterator it;

numletters = (pscore.gethighbound(0)+1)/NumberOfSequences;
choice = pscore.gethighbound(1) + 1;
canpredict = ( choice == NumMatrixRows + NumMatrixColumns );  
epoch = NumberOfSequences * choice;
cflash.setbounds(0, choice-1, 0, pscore.gethighbound(0));
predictedrow.setbounds(0, static_cast<int>(numletters-1), 0, NumberOfSequences-1);
predictedcol.setbounds(0, static_cast<int>(numletters-1), 0, NumberOfSequences-1);

////////////////////////////////////////////////////////////////////////
// Predict letters according to pscore
for (int i=0; i<numletters; i++)
{
	for (int j=0; j<trialnr.gethighbound(1)+1; j++)
	{
		if (trialnr(j) == i+1)
			range.push_back(j);
	}
	if (range.size() != 0)
	{
#if 0
		for (size_t j=0; j<range.size(); j++)
			codes.push_back(StimulusCode(range[j])*StimulusType(range[j]));

		sort(codes.begin(), codes.end());
		it = unique(codes.begin(), codes.end()); 
		codes.resize(it-codes.begin());

		if (codes.size() >= 2)
			codecol.push_back(codes[1]);
		if (codes.size() > 2)
			coderow.push_back(codes[2]);
		
		codes.clear();
#else // jm Oct 24, 2012 NOTE: This functions seems to invert the meaning of Rows and Columns
    {
      set<int> codes;
	    if( NumMatrixRows < 2 )
	      codes.insert( NumMatrixColumns + 1 );
	    if( NumMatrixColumns < 2 )
	      codes.insert( 1 );
		  for (size_t j=0; j<range.size(); j++)
			  codes.insert(StimulusCode(range[j])*StimulusType(range[j]));

      assert( codes.size() == 3 );
	    set<int>::const_iterator i = codes.begin();
	    if( i != codes.end() )
	      ++i;
	    if( i != codes.end() )
	      codecol.push_back( *i++ );
	    if( i != codes.end() )
	      coderow.push_back( *i++ );
	  }
#endif
		range.clear();

		for (int j=0; j<choice; j++)
		{
			val = 0;
			for (int k=i*NumberOfSequences; k<(i+1)*NumberOfSequences; k++)
			{
				val += pscore(j,k);
				cflash(j,k) = val;
			}
		}		
	}
}
for (int i=0; i<numletters; i++)
{
	int s = 0;
  for (int k=i*NumberOfSequences; k<(i+1)*NumberOfSequences; k++)
	{
    max_value_col= -inf, max_value_row= -inf;
		for (int j=0; j<choice; j++)
		{
		  int code = j + 1;
		  if( NumMatrixColumns < 1 )
		    code += 1;
			if ((j<NumMatrixColumns))
			{
				/* compare with max */
				if (cflash(j,k) > max_value_col)
				{
					max_value_col = cflash(j,k);
					predictedcol(i,s) = code;
				}
			}
			if ((j>=NumMatrixColumns))
			{
				/* compare with max */
				if (cflash(j,k) > max_value_row)
				{
					max_value_row = cflash(j,k);
					predictedrow(i,s) = code;
				}
			}
		}
		// jm Oct 24, 2012
		if( NumMatrixColumns < 1 )
		  predictedcol( i, s ) = 1;
		if( NumMatrixRows < 1 )
		  predictedrow( i, s ) = NumMatrixColumns + 1;
		  
		s++;
	}
}

// jm
if( NumMatrixColumns < 1 )
  NumMatrixColumns = 1;

for (int j=0; j<NumberOfSequences; j++)
{
	correct= 0;
	for (int i=0; i<numletters; i++)
	{
		if ((coderow[i] == predictedrow(i,j)) && codecol[i] == predictedcol(i,j))
			correct++;
    if( canpredict )
      predicted.push_back(TargetDefinitions[NumMatrixColumns*(predictedrow(i,j)-1-NumMatrixColumns)+predictedcol(i,j)-1]);
	}
  if( canpredict )
  	result.push_back((correct/numletters)*100);
}
}
