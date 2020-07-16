#include "GetP3Responses.h"
////////////////////////////////////////////////////////////////////////////////////
/// Get P300 Responses from the signal.
/// @param [in] signal        Signal is filtered and downsampled. Signal is overwritten. 
/// @param [in] trialnr       ste trialnr. trialnr is overwritten.
/// @param [in] windowlen     Parameter windlen. 
/// @param [in] stimulusCode  ste Stimulus code. stimulusCode is overwritten.
/// @param [in] stimulusType  state Stimulus type. stimulusType is overwritten.
/// @param [in] Flashing      ste Flashing.
/// @param [in] channels      Parameter channel_set. 
/// @param [in] MAfilter      Parameter Filter order.
/// @param [in] DecFact       Parameter Decimation_Frequency
/// \author Cristhian Potes
/// \date June 01, 2009

void GetP3Responses(ap::template_2d_array<float,true>& signal, 
                    ap::template_1d_array<short int,true>& trialnr,
                    ap::template_1d_array<double,true>& windowlen, 
                    ap::template_1d_array<unsigned short int, true>& stimulusCode,
                    ap::template_1d_array<unsigned short int, true>& stimulusType,
                    const ap::template_1d_array<unsigned char, true>& Flashing,
                    const ap::template_1d_array<double, true>& channels,
                    const int MAfilter,
                    const int DecFact)
{
////////////////////////////////////////////////////////////////////////////
// Section:  Define variables
int bound_min, bound_max, row, col, row_windowlen,
    row_channels, numchannels, lenflash, sig_ds_len, siglen;
double val_temp;
ap::template_1d_array<float, true> vect;
ap::template_1d_array<float, true> vect_r;
ap::template_1d_array<float, true> a; //variable to store filter coefficients
ap::template_1d_array<float, true> b; //Variable to store filter coefficients
ap::template_2d_array<float, true> sig; //filtered and downsampled signal
ap::template_1d_array<float, true> y_downsampled;
ap::template_1d_array<unsigned short int, true> Code;
ap::template_1d_array<unsigned short int, true> Type;
ap::template_1d_array<short int, true> trial;
vector<int> tmp;
////////////////////////////////////////////////////////////////////////////
// Section:  Get dimensions
row = signal.gethighbound(1)+1;
col = signal.gethighbound(0)+1;
row_windowlen = windowlen.gethighbound(1)+1;
row_channels = channels.gethighbound(1)+1;
numchannels = col;
lenflash = row;
////////////////////////////////////////////////////////////////////////////
// Section:  Check if windowlen has only one value (e.g. windlen = 800)
if (row_windowlen == 1)
{
  val_temp = windowlen(0);
  windowlen.setbounds(0, row_windowlen);
  windowlen(0) = 0;
  windowlen(1) = val_temp;
}
////////////////////////////////////////////////////////////////////////////
// Section:  Identify changes in Flashing 
for (int i=1; i<lenflash; i++)
{
  if (Flashing(i-1) == 0 && Flashing(i) ==1)
  {
    if((i+windowlen(1)-2)<lenflash)
      tmp.push_back(i);
  }
}
Code.setbounds(0,static_cast<int>(tmp.size())-1);
Type.setbounds(0,static_cast<int>(tmp.size())-1);
trial.setbounds(0, static_cast<int>(tmp.size())-1);
////////////////////////////////////////////////////////////////////////////
// Section:  Filter and downsample the signal 
sig_ds_len = ap::iceil((windowlen(1)-windowlen(0))/DecFact); 
siglen = sig_ds_len * numchannels; 
sig.setbounds(0,static_cast<int>(tmp.size())-1,0, siglen-1);
y_downsampled.setbounds(0, sig_ds_len-1);

// Define filter coefficients (Moving Average Filter) 
a.setbounds(0, MAfilter-1);
b.setbounds(0, MAfilter-1);
for (int i=0; i<MAfilter; i++)
{
  a(i) = (float) 1;
  b(i) = (float) 1/MAfilter;
}
// Extract the signal according to the specified window
// then downsample and filtering it.
for(int j=0; j<static_cast<int>(tmp.size()); j++)
{
  bound_min = static_cast<int>( tmp[j] + windowlen(0) - 1 );
  bound_max = static_cast<int>( tmp[j] + windowlen(1) - 2 );
  vect.setbounds(0, bound_max-bound_min);
  vect_r.setbounds(0, bound_max-bound_min);

  for (int i=0; i<col; i++)
  {
    ap::vmove(vect.getvector(0, bound_max-bound_min), signal.getcolumn(i, bound_min, bound_max));
    filter(MAfilter-1, a, b, bound_max-bound_min, vect, vect_r);
    downsampling(vect_r, DecFact, y_downsampled);
    ap::vmove(sig.getrow(j, i*sig_ds_len, ((i+1)*sig_ds_len)-1), y_downsampled.getvector(0, sig_ds_len-1));
  }
  Code(j) = stimulusCode(tmp[j]);
  Type(j) = stimulusType(tmp[j]);
  trial(j) = trialnr(tmp[j]);
}
// Overwrite signal and states
signal.setbounds(0,static_cast<int>(tmp.size())-1,0, siglen-1);
stimulusCode.setbounds(0, static_cast<int>(tmp.size())-1);
stimulusType.setbounds(0, static_cast<int>(tmp.size())-1);
trialnr.setbounds(0, static_cast<int>(tmp.size())-1);

signal = sig;
stimulusCode = Code;
stimulusType = Type;
trialnr = trial;
}

