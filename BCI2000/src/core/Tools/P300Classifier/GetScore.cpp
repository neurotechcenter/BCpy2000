#include "GetScore.h"

#define TRUE 1
#define FALSE 0

///////////////////////////////////////////////////////////////////
/// Compute a score matrix. The number of rows indicates the number
/// of available choices (e.g., The P300 Speller 6 x 6 has 12 possible choices).
/// The number of columns is composed by the number of trials for each
/// letter to identify.
/// @param [in] Responses         Given data
/// @param [in] Code              ste Stimulus Code
/// @param [in] tMUD              A structure with the necessary parameters to 
///                               classify a BCI2000 data online
/// @param [in] trialnr           ste trialnr
/// @param [in] windowlen         parameter windowlen
/// @param [in] numchannels       Number of channels specified by the user in channel_set
///	@param [in] NumberOfSequences Number of sequences. Default = 15;
/// @param [in] NumberOfChoices   Number of choices. This values is computed by adding the 
///                               rows and columns given in the P300 Speller 
///	@param [out] pscore           Matrix containing the score values for each row and column
/// \author Cristhian Potes
/// \date June 29, 2009

bool GetScore(ap::template_2d_array<float,true>& Responses, 
              ap::template_1d_array<unsigned short int, true>& Code,
              parameters tMUD,
              ap::template_1d_array<short int,true>& trialnr,
              ap::template_1d_array<double,true>& windowlen, 
              int numchannels,
              int NumberOfSequences,
              int NumberOfChoices,
              int mode,
              ap::real_2d_array &pscore)

{
///////////////////////////////////////////////////////////////////////
// Section: Define variables
int row_Responses, col_Responses, row_MUD, col_MUD,
  dslen, count, max, NumberOfEpochs, numVariables;

bool flag = true;

ap::real_2d_array Responses_double;
ap::template_2d_array<float, true> Responses_copy;
ap::real_2d_array DATA;
ap::real_2d_array tmp_MUD;
ap::real_1d_array score;
ap::real_1d_array weights;

vector<short int> trial;
//vector<short int> trial_copy;
vector<int> range;
vector<int> code_indx;
vector<short int>::iterator it;

///////////////////////////////////////////////////////////////////////
// Section: Get Dimmensions 
row_Responses = Responses.gethighbound(1)+1;
col_Responses = Responses.gethighbound(0)+1;
row_MUD = tMUD.MUD.gethighbound(1)+1;
col_MUD = tMUD.MUD.gethighbound(0)+1;

///////////////////////////////////////////////////////////////////////
// Section: Extract from the signal only the channels containing the "in" variables
numVariables = static_cast<int>( col_Responses/static_cast<double>( numchannels ) );
Responses_copy.setbounds(0, row_Responses-1, 0, numVariables*(tMUD.channels.gethighbound(1)+1)-1);
Responses_double.setbounds(0, row_Responses-1, 0, numVariables*(tMUD.channels.gethighbound(1)+1)-1);

for (int i=0; i<row_Responses; i++)
{
  for (int j=0; j<tMUD.channels.gethighbound(1)+1; j++)
  {
    ap::vmove(Responses_copy.getrow(i, j*numVariables, ((j+1)*numVariables)-1), 
              Responses.getrow(i, static_cast<int>(tMUD.channels(j)-1)*numVariables, 
                                  static_cast<int>(tMUD.channels(j)*numVariables)-1));
  }
}

for (int i=0; i<row_Responses; i++)
{
    for (int j=0; j<numVariables*(tMUD.channels.gethighbound(1)+1); j++) 
      Responses_double(i,j) = static_cast<double>( Responses_copy(i,j) );
}

for (int i=0; i<row_Responses; i++)
   trial.push_back(trialnr(i)); 

///////////////////////////////////////////////////////////////////////
// Section: Downsampling the MUD  
dslen = ap::iceil((row_MUD-1)/tMUD.DF)+1;
tmp_MUD.setbounds(0, dslen, 0, col_MUD-1);
for (int j=0; j<col_MUD; j++)
{
  for (int i=0; i<dslen; i++)
  {
    if (j==0)
      tmp_MUD(i,0) = tMUD.MUD(i*tMUD.DF, 0)-1;

    if (j==1)
    {
      tmp_MUD(i,1) = tMUD.MUD(i*tMUD.DF, 1) - windowlen(0);
      tmp_MUD(i,1) = ap::ifloor(tmp_MUD(i,1)/tMUD.DF)+1;
      tmp_MUD(i,1) = tmp_MUD(i,1) + (tmp_MUD(i,0)*numVariables);
    }
    if (j==2)
      tmp_MUD(i,2) = tMUD.MUD(i*tMUD.DF, 2);
  }
}
///////////////////////////////////////////////////////////////////////
// Section: Computing the score 
DATA.setbounds(0, row_Responses-1, 0, dslen-1);
score.setbounds(0, row_Responses-1);
weights.setbounds(0, dslen-1);

double valor;
for (int i=0; i<dslen; i++)
{
  valor = tmp_MUD(i,1); 
  ap::vmove(DATA.getcolumn(i, 0, row_Responses-1), Responses_double.getcolumn(static_cast<int>( tmp_MUD(i,1) ), 0, row_Responses-1));
  valor = DATA(0,i); 
  weights(i) = tmp_MUD(i,2);
}

matrixvectormultiply(DATA, 0, row_Responses-1, 0, dslen-1, FALSE, weights, 0, dslen-1, 1, score, 0, row_Responses-1, 0);


///////////////////////////////////////////////////////////////////////
// Section: Make sure that the epochs are not outside of the boundaries 
#if 0 // jm Mar 18, 2011
trial_copy = trial;
it = unique(trial_copy.begin(), trial_copy.end());
trial_copy.resize(it-trial_copy.begin());

max = *max_element(trial_copy.begin(), trial_copy.end());
#else // jm
max = *max_element(trial.begin(), trial.end());
#endif // jm

count = 0;
for (size_t i=0; i<trial.size(); i++)
{
  if (trial[i] == max)
    count++;
}

if (count == NumberOfSequences*NumberOfChoices)
  NumberOfEpochs = *max_element(trial.begin(), trial.end());
else
  NumberOfEpochs = *max_element(trial.begin(), trial.end())-1;

///////////////////////////////////////////////////////////////////////
// Section: Create a matrix with the scores for each sequence 
pscore.setbounds(0, NumberOfChoices-1, 0, (NumberOfEpochs*NumberOfSequences)-1);
for (int i=0; i<NumberOfEpochs; i++)
{
  for (size_t j=0; j<trial.size(); j++)
  {
    if (trial[j] == i+1)
      range.push_back(static_cast<int>(j));
  }
  if ((range.size() != 0) && (range.size() == NumberOfSequences*NumberOfChoices))
  {
    for (int k=0; k<NumberOfChoices; k++)
    {
      for (size_t j=0; j<range.size(); j++)
      {
        if (Code(range[j]) == k+1)
          code_indx.push_back(range[j]);
      }
      for (size_t j=0; j<code_indx.size(); j++)
      {
        if (code_indx.size() == NumberOfSequences)
          pscore(k,static_cast<int>((i*NumberOfSequences)+j)) = score(code_indx[j]);
      }
      code_indx.clear();
    }
	flag = true;
  }
  else
  {
	  flag = false;
	  break;
  }
  range.clear(); 
}
return flag;
}
