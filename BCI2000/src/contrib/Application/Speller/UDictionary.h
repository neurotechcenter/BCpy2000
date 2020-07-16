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

#ifndef UDictionaryH
#define UDictionaryH
//---------------------------------------------------------------------------

#define MAX_WORDS       2000
#define MAX_WORDLENGTH  20

class DICTIONARY
{
private: 	// User declarations
        char    words[MAX_WORDS][MAX_WORDLENGTH];
        int     numwords;
public:		// User declarations
        DICTIONARY::DICTIONARY();
        DICTIONARY::~DICTIONARY();
        int     LoadDictionary(const char *dictionaryfile, bool eraseold);
        void    DeleteWords();
        void    AddWord(const char *word);
        const char    *GetWord(int idx) const;
        int     GetNumWords();
        int     GetNumMatching(const char *prefix, bool casesensitive);
        const char    *GetMatchingWord(const char *prefix, bool casesensitive, int idx) const;
};
#endif
