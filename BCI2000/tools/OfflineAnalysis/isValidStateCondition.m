%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: isValidStateCondition.m 2007-111-26 12:31:37EST fialkoff $ 
%% 
%% File: isValidStateCondition.m 
%% 
%% Author: Joshua Fialkoff <fialkj@rpi.edu>, Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This function checks to make sure that the syntax of a
%% given condition is valid
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
function [isValid] = isValidStateCondition(cond, stateVarName)

isValid = 0;

cond = regexprep(cond, '^\s*(.*?)\s*$', '$1');
if isempty(cond)
  return;
end

stateVarFields = {'Running', 'Active', 'SourceTime', 'RunActive', ...
  'Recording', 'IntCompute', 'TargetCode', 'ResultCode', ...
  'StimulusTime', 'Feedback', 'IntertrialInterval', 'RestPeriod'};
operators = {'==', '<=', '~=', '>=', '>', '<', '&', '|', '&&', '||'};
modifiers = {'~', '-'};

tokenDelims = '=<~>()-&|';
equalIdx = 1;
ltIdx = 2;
notIdx = 3;
gtIdx = 4;
openParenIdx = 5;
closeParenIdx = 6;
negIdx = 7;
andIdx = 8;
orIdx = 9;

parenDepth = 0;
curTok = '';
curOp = '';
tokType = [];
bDot = false;

lastTokType = 0;
% 1 - literal or variable
% 4 - open paren
% 5 - close paren
% 6 - operator
% 8 - modifier
acceptedLastToks{1} = [4 6 8 0];
acceptedLastToks{4} = [4 6 0];
acceptedLastToks{5} = [5 1];
acceptedLastToks{6} = [1 5];
acceptedLastToks{8} = [4 6 8];

for idxChar = 1:length(cond)
  %check to see if this is a token delimiter
  delimIdx = findstr(cond(idxChar), tokenDelims);
  if ~isempty(delimIdx) || ~isempty(curOp) || ...
      idxChar == length(cond)
    if idxChar == length(cond) && isempty(delimIdx)
      curTok = [curTok cond(idxChar)];
    end
    
    %first close out previous token - it should be either a state variable
    %(i.e., stateVar.fieldName), a number or empty

    %trim spaces from beginning and end
    curTok = regexprep(curTok, '^\s*(.*?)\s*$', '$1');
    if isempty(curTok)
      %do nothing
    elseif ~isempty(regexp(curTok, '\s*'))
      return;
    else
      if ~isnan(str2double(curTok))
        tokType = [tokType 1];
      elseif length(curTok) >= length(stateVarName)+1 && ...
          all(curTok(1:length(stateVarName)+1)==[stateVarName '.'])
        tokType = [tokType 1];
      else
        return;
%         for idxFields = 1:length(stateVarFields)
%           if strcmp(curTok, [stateVarName '.' stateVarFields{idxFields}])
%             tokType = [tokType 1];
%             break;
%           end
%         end
% 
%         if isempty(tokType)
%           return;
%         end
      end
    end
    curTok = '';
    
    if ~isempty(curOp)
      %check to see if this is an extension of the operator.  If this code
      %executes, any non-delimiter based tokens will already have been
      %taken care of
      bFoundOp = 0;
      tmpOp = [curOp cond(idxChar)];
      for idxOp = 1:length(operators)
        if strcmp(tmpOp, operators{idxOp})
          tokType = [tokType 6];
          bFoundOp = 1;
          %delimIdx referred to an extension of the previous operator, we
          %don't want to consider it separately, so:
          delimIdx = [];
          break;
        end
      end
    
      %not a 2-char operator
      if ~bFoundOp
        %is it a modifier?
        for idxMod = 1:length(modifiers)
          if curOp == modifiers{idxMod}
            tokType = [tokType 8];
            bFoundOp = 1;
            break;
          end
        end

        %is it an operator?
        if ~bFoundOp
          for idxOp = 1:length(operators)
            if strcmp(curOp, operators{idxOp})
              tokType = [tokType 6];
              bFoundOp = 1;          
              break;
            end
          end
          
        end
        
        if isempty(delimIdx)
          curTok = cond(idxChar);
        end
      end
      
      curOp = '';
    end
    
    if ~isempty(delimIdx)
      switch delimIdx
        case openParenIdx
          parenDepth = parenDepth + 1;
          tokType = [tokType 4];
        case closeParenIdx
          parenDepth = parenDepth - 1;
          tokType = [tokType 5];
        case negIdx
          tokType = [tokType 8];
        otherwise
          if idxChar == length(cond)
            return;
          else
            %this delimiter may be more than one character. Save and move on
            curOp = [curOp cond(idxChar)];
          end
      end
    end
    
    if ~isempty(tokType)
      %1 or more tokens found
      for idxTok = 1:length(tokType)
        %for each token confirm that the last token was of an acceptable
        %type
        if ~any(acceptedLastToks{tokType(idxTok)} == lastTokType)
          return;
        end
        lastTokType = tokType(idxTok);
      end
      tokType = [];
    end
  else %not a delimiter
    curTok = [curTok cond(idxChar)];
  end
end

%check to make sure that last token is valid
if ~any([1 5] == lastTokType)
  return;
end

isValid = (parenDepth == 0);
return;
