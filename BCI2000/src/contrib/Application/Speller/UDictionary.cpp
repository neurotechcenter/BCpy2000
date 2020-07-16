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
#include <string.h>

#include "UDictionary.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


// **************************************************************************
// Function:   DICTIONARY
// Purpose:    This is the constructor of the DICTIONARY class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
DICTIONARY::DICTIONARY()
{
 numwords=0;
}


// **************************************************************************
// Function:   DICTIONARY
// Purpose:    This is the constructor of the DICTIONARY class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
DICTIONARY::~DICTIONARY()
{
 DeleteWords();
}


// **************************************************************************
// Function:   LoadDictionary
// Purpose:    This function loads a dictionary file
// Parameters: dictionaryfile - filename of the dictionary file
//             eraseold - true  ... deletes all existing words in the dictionary
//                        false ... does not delete existing words in the dictionary
// Returns:    0 ... error
//             1 ... OK
// **************************************************************************
int DICTIONARY::LoadDictionary(const char *dictionaryfile, bool eraseold)
{
char    line[MAX_WORDLENGTH];
FILE    *fp;
int     i;

 if (eraseold) DeleteWords();

 fp=fopen(dictionaryfile, "rb");
 if (!fp) return(0);

 
 while (!feof(fp))
  {
  fgets(line, MAX_WORDLENGTH, fp);
  line[MAX_WORDLENGTH-1]=0;
  for (i=0; i<(int)strlen(line); i++)
   if ((line[i] == ' ') || (line[i] == '\r') || (line[i] == '\n'))
      line[i]=0;
  AddWord(line);
  }
 fclose(fp);

 return(1);
}


int DICTIONARY::GetNumWords()
{
 return(numwords);
}


void DICTIONARY::DeleteWords()
{
 numwords=0;
}


void DICTIONARY::AddWord(const char *word)
{
 if (numwords >= MAX_WORDS-2) return;

 strcpy(&words[numwords][0], word);
 numwords++;
}


const char *DICTIONARY::GetWord(int idx) const
{
 if (idx >= numwords) return(NULL);

 return(&words[idx][0]);
}


int DICTIONARY::GetNumMatching(const char *prefix, bool casesensitive)
{
int     i, count;

 if (numwords == 0) return(0);

 count=0;
 for (i=0; i<numwords; i++)
  {
  if (casesensitive)
     if (strncmp(GetWord(i), prefix, strlen(prefix)) == 0)
        count++;
  if (!casesensitive)
     if (strnicmp(GetWord(i), prefix, strlen(prefix)) == 0)
        count++;
  }

 return(count);
}


const char *DICTIONARY::GetMatchingWord(const char *prefix, bool casesensitive, int idx) const
{
int        i, count;
const char *ptr;

 count=-1;
 ptr=NULL;
 for (i=0; i<numwords; i++)
  {
  if (casesensitive)
     if (strncmp(GetWord(i), prefix, strlen(prefix)) == 0)
        count++;
  if (!casesensitive)
     if (strnicmp(GetWord(i), prefix, strlen(prefix)) == 0)
        count++;
  if (count == idx)
     {
     ptr=GetWord(i);
     break;
     }
  }

 return(ptr);
}

