%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: calc_rsqu.m 2007-111-26 12:31:37EST schalk $ 
%% 
%% File: calc_rsqu.m 
%% 
%% Author: Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This function calclulates the r^2 values for two 3-D
%% variables.
%%
%% $BEGIN_BCI2000_LICENSE$
%% 
%% This file is part of BCI2000, a platform for real-time bio-signal research.
%% [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
%% 
%% BCI2000 is free software: you can redistribute it and/or modify it under the
%% terms of the GNU General Public License as published by the Free Software
%% Foundation, either version 3 of the License, or (at your option) any later
%% version.
%% 
%% BCI2000 is distributed in the hope that it will be useful, but
%%                         WITHOUT ANY WARRANTY
%% - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
%% A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
%% 
%% You should have received a copy of the GNU General Public License along with
%% this program.  If not, see <http://www.gnu.org/licenses/>.
%% 
%% $END_BCI2000_LICENSE$
%% http:%%www.bci2000.org 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [ressq, amp1, amp2] = calc_rsqu(data1, data2, rorrsqu)
%RSQU   [ressq, avgamp1, avgamp2]  = calc_rsqu(data1, data2, rorrsqu) calculates the r2-value for
%       two three-dimensional variables (dim1 by dim2 by trial) data1 and data2
%       the result is ressq (dim1, dim2); each element represents the r2 
%       for this particular combination of dim1, dim2 across trials
%       in addition to r2 values, this function also calculates average amplitudes 
%       for each sample and channel, for both data variables (i.e., conditions), and
%       returns these in amp1 and amp2
%       rorrsqu == 1 ... rsqu values
%                  2 ... r values

if (rorrsqu == 1)
   for ch=1:size(data1, 2)
    for samp=1:size(data1, 1)
       ressq(samp, ch)=rsqu(data1(samp, ch, :), data2(samp, ch, :));
       amp1(samp, ch)=mean(data1(samp, ch, :));
       amp2(samp, ch)=mean(data2(samp, ch, :));
    end
   end
else
   for ch=1:size(data1, 2)
    for samp=1:size(data1, 1)
       ressq(samp, ch)=rvalue(data1(samp, ch, :), data2(samp, ch, :));
       amp1(samp, ch)=mean(data1(samp, ch, :));
       amp2(samp, ch)=mean(data2(samp, ch, :));
    end
   end
end
