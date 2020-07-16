/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
/*/
#include "PCHIncludes.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include <stdio.h>

#include "UTarget.h"
#include "UTree.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


// **************************************************************************
// Function:   TREE
// Purpose:    This is the constructor for the TREE class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
TREE::TREE()
{
/*shidong starts*/
   debug = false;
   if(debug) a = fopen("tree.txt", "w");
       
/*shidong ends*/
 treesize=0;
}


// **************************************************************************
// Function:   ~TREE
// Purpose:    This is the destructor for the TREE class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
TREE::~TREE()
{
/*shidong starts*/
 if(debug)fclose(a);
 /*shidong ends*/
}


// **************************************************************************
// Function:   get_argument
// Purpose:    parses the parameter line
//             it returns the next token that is being delimited by either
//             a ' ' or '='
// Parameters: ptr - index into the line of where to start
//             buf - destination buffer for the token
//             line - the whole line
//             maxlen - maximum length of the line
// Returns:    the index into the line where the returned token ends
// **************************************************************************
int TREE::get_argument(int ptr, char *buf, const char *line, int maxlen) const
{
 // skip trailing spaces, if any
 while ((line[ptr] == '=') || (line[ptr] == ' ') && (ptr < maxlen))
  ptr++;

 // go through the string, until we either hit a space, a '=', or are at the end
 while ((line[ptr] != '=') && (line[ptr] != ' ') && (line[ptr] != '\n') && (line[ptr] != '\r') && (ptr < maxlen))
  {
  *buf=line[ptr];
  ptr++;
  buf++;
  }

 *buf=0;
 return(ptr);
}


// **************************************************************************
// Function:   LoadTree
// Purpose:    This function loads a tree from a specified file
// Parameters: filename - pointer to the filename
// Returns:    0 - error (e.g., file not found)
//             1 - no error
// **************************************************************************
int TREE::LoadTree()
{
/*shidong starts*/
int     ptr;
//clean tree value in these 3 arraay
for (int i=0; i<treesize; i++)
{
        parentID[treesize] = 0;
        displaypos[treesize] = 0;
        targetID[treesize] = 0;
}//for
treesize = 0;


 for (int i = 0; i < Parameter("TreeDefinitionMatrix")->NumRows(); i++)
 {
        parentID[treesize] = AnsiString(Parameter("TreeDefinitionMatrix")(i,0).c_str()).ToInt();
        displaypos[treesize] = AnsiString(Parameter("TreeDefinitionMatrix")(i,1).c_str()).ToInt()-1;
        targetID[treesize]= AnsiString(Parameter("TreeDefinitionMatrix")(i,2).c_str()).ToInt();
        if(debug)fprintf(a, "ParentID[%d] is %d \t displaypos[%d] is  %d \t target[%d] is %d\n", treesize, parentID[treesize]   , treesize, displaypos[treesize], treesize, targetID[treesize]);
        treesize++;
 }//for

/*
char    line[256], buf[256];
FILE    *fp;
 int     ptr;

 // read the tree definition file
 fp=fopen(filename, "rb");
 if (!fp) return(0);

 while (!feof(fp))
  {
  fgets(line, 255, fp);
  if (strlen(line) > 2)
     {
     ptr=0;
     // first column ... parent ID
     ptr=get_argument(ptr, buf, line, 255);
     parentID[treesize]=atoi(buf);
     // second column ... display position
     ptr=get_argument(ptr, buf, line, 255);
     displaypos[treesize]=atoi(buf)-1;
     // third column ... target ID
     ptr=get_argument(ptr, buf, line, 255);
     targetID[treesize]=atoi(buf);
     treesize++;
     }
  }
 fclose(fp);
  */
 return(1);
 /*shidong ends*/
}


// **************************************************************************
// Function:   DetermineTargetID
// Purpose:    Returns target ID for a given parent ID and display position
// Parameters: cur_parentID   - target ID of the parent of the subtree
//             cur_displaypos - screen position (or, branch number) of the child of cur_parentID (0..NUM_TARGETS-1)
// Returns:    TARGETID_NOID - there is no child for that branch and parent
//             all other - target ID of the target that is a child of ID cur_parentID and has the specified screen position
// **************************************************************************
int TREE::DetermineTargetID(int cur_parentID, int cur_displaypos)
{
int     i, ret;

 ret=TARGETID_NOID;
 for (i=0; i<treesize; i++)
  if (parentID[i] == cur_parentID)
     if (displaypos[i] == cur_displaypos)
        {
        ret=targetID[i];
        break;
        }

 return(ret);
}


// **************************************************************************
// Function:   HasChildren
// Purpose:    Determines whether or not a given parent ID has any children
// Parameters: cur_parentID   - target ID of the parent of the subtree
// Returns:    false - this target has no children
//             true - this target has at least one child
// **************************************************************************
bool TREE::HasChildren(int cur_parentID)
{
int     branch, targetID;

 for (branch=0; branch<MAX_BRANCHES; branch++)
  {
  targetID=DetermineTargetID(cur_parentID, branch);
  if ((targetID != TARGETID_NOID) && (targetID != TARGETID_BACKUP))
     return(true);
  }

 return(false);
}


// **************************************************************************
// Function:   DoesLeadTo
// Purpose:    determines whether the tree (given the current parent ID)
//             can lead to a specific target_ID
// Parameters: cur_parentID   - target ID of the parent of the subtree
//             cur_targetID   - ID of the sought after target
// Returns:    false  - the subtree specified by cur_parentID does not contain a child with ID cur_targetID
//             true   - ... does contain ...
// **************************************************************************
bool TREE::DoesLeadTo(int cur_parentID, int cur_targetID)
{
int     branch, targetID, count;

 count=0;
 for (branch=0; branch<MAX_BRANCHES; branch++)
  {
  targetID=DetermineTargetID(cur_parentID, branch);
  if (targetID == cur_targetID)
     return(true);
  else
     {
     if ((targetID != TARGETID_NOID) && (targetID != TARGETID_BACKUP))
        {
        if (DoesLeadTo(targetID, cur_targetID))
           count++;
        }
     }
  }

 // does any of the paths contain the correct targetID ?
 if (count > 0) return(true);

 return(false);
}



