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
//---------------------------------------------------------------------------

#ifndef UTreeH
#define UTreeH
//---------------------------------------------------------------------------

#define MAX_TREESIZE    65535
#define MAX_BRANCHES    16             // maximum number of branches from one node

#include "Environment.h"

class TREE : public Environment
{
private: 	// User declarations
        int     parentID[MAX_TREESIZE], displaypos[MAX_TREESIZE], targetID[MAX_TREESIZE];
        int     get_argument(int ptr, char *buf, const char *line, int maxlen) const;
        int     treesize;
public:		// User declarations
        TREE::TREE();
        TREE::~TREE();
        /*shidong starts*/
        int     LoadTree();
        FILE            *a ;
        bool    debug;
        //int     LoadTree(const int treeRow, const int treeCol);
        /*shidong ends*/
        int     DetermineTargetID(int parentID, int displaypos);
        bool    DoesLeadTo(int cur_parentID, int cur_targetID);
        bool    HasChildren(int cur_parentID);
};
#endif
