#include "StimulusPresentation_Classify.h"

void StimulusPresentation_Classify(ap::real_2d_array pscore, 
								   ap::template_1d_array<unsigned short int, true>& StimulusCode,
								   ap::template_1d_array<unsigned short int, true>& StimulusType,
								   ap::template_1d_array<short int, true>& trialnr,
								   int NumberOfSequences,
								   int NumberOfChoices,
								   vector<string> TargetDefinitions,
								   vector<double>& result,
								   vector<string>& predicted)
{
vector<int> range;
vector<int> codes;
vector<int> StimuliTarget;
vector<int>::iterator it;
double numletters, correct, val, max_value = -1e15;
int epoch;
ap::real_2d_array cflash;
ap::real_2d_array score;
ap::template_2d_array<int, true> PredictedStimuli;

numletters = (pscore.gethighbound(0)+1)/NumberOfSequences;
epoch = NumberOfSequences * NumberOfChoices;
cflash.setbounds(0, NumberOfChoices-1, 0, pscore.gethighbound(0));
score.setbounds(0, static_cast<int>(numletters-1), 0, NumberOfChoices-1);
PredictedStimuli.setbounds(0, static_cast<int>(numletters-1), 0, NumberOfSequences-1);

// 
for (int i=0; i<numletters; i++)
{
	for (int j=0; j<trialnr.gethighbound(1)+1; j++)
	{
		if (trialnr(j) == i+1)
			range.push_back(j);
	}
	if (range.size() != 0)
	{
		for (size_t j=0; j<range.size(); j++)
			codes.push_back(StimulusCode(range[j])*StimulusType(range[j]));

		sort(codes.begin(), codes.end());
		it = unique(codes.begin(), codes.end()); 
		codes.resize(it-codes.begin());
		StimuliTarget.push_back(codes[1]);
		codes.clear();
		range.clear();

		for (int j=0; j<NumberOfChoices; j++)
		{
			val = 0;
			for (int k=i*NumberOfSequences; k<(i+1)*NumberOfSequences; k++)
			{
				val += pscore(j,k);
				cflash(j,k) = val;
			}
			score(i,j) = val;
		}	
	}
}

for (int i=0; i<numletters; i++)
{
	int l = 0;
	for (int k=i*NumberOfSequences; k<(i+1)*NumberOfSequences; k++)
	{
		for (int j=0; j<NumberOfChoices; j++)
		{
			/* compare with max */
			if (cflash(j,k) > max_value)
			{
				max_value = cflash(j,k);
				PredictedStimuli(i,l) = j + 1;
			}
		}
		l++, max_value = -1e15;
	}
}

for (int j=0; j<NumberOfSequences; j++)
{
	correct= 0;
	for (int i=0; i<numletters; i++)
	{
		if (StimuliTarget[i] == PredictedStimuli(i,j)) 
		{
			predicted.push_back(TargetDefinitions[PredictedStimuli(i,j)-1]);
			correct++;
		}
		else
			predicted.push_back(TargetDefinitions[PredictedStimuli(i,j)-1]);
	}
	result.push_back((correct/numletters)*100);
}
}
