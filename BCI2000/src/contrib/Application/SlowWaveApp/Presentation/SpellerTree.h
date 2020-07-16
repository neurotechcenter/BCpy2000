/////////////////////////////////////////////////////////////////////////////
//
// File: SpellerTree.h
//
// Date: Dec 13, 2001
//
// Author: Juergen Mellinger
//
// Description: A class that reads and manages a speller tree
//              with arbitrary depth and an arbitrary number of
//              choices at each node.
//
// Changes:
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
//////////////////////////////////////////////////////////////////////////////

#ifndef SPELLERTREEH
#define SPELLERTREEH

#include <vector>
#include <string>
#include <map>

#include "PresErrors.h"

class TSpellerTreeNode
{
  public:
    std::vector< std::string >  labels;
    std::vector< std::string >  texts;
    std::vector< unsigned int > choices;
    std::string                 audioLabel;
};

class TSpellerTree : private std::vector< TSpellerTreeNode >
{
  public:
                    TSpellerTree();
                    ~TSpellerTree();

    void            Reset();
    bool            Fail() const;
    bool            BackspaceFail() const;

    // From the current node, choose the option associated with the
    // target code given. Returns a reference to a string that is
    // associated with that choice.
    const std::string&  Choose( unsigned int inTargetCode );

    // Get the current node's user label for the target code given.
    const std::string&  GetLabel( unsigned int inTargetCode );

    // Get the current node's audio label file name.
    const std::string&  GetAudioLabel() const;

    // Return the target code for the choice that has to be chosen for the shortest
    // way to enter the text given in the argument.
    unsigned int        NextChoiceInShortestPath(   const std::string&  inCurrentText,
                                                    const std::string&  inTargetText );
    unsigned int        NextChoiceInShortestPath(   const iterator      inStartNode,
                                                    const iterator      inTargetNode );

    unsigned short      DictionaryProposalCode() const;

    TPresError          ReadFromFile( const char* inTreeFileName );

    // Adapt a given text to the capabilities of the current speller tree.
    TPresError          NormalizeText( std::string& ioText );

  private:
    enum TFailstate
    {
        good = 0,
        generalFail = 1 << 0,
        backspaceFail = 1 << 1
    };
    
    typedef unsigned long           TWeightType;
    static const unsigned long      infiniteWeight = 32000;
    TWeightType  Weight( const iterator, size_t ) const;

    struct TPathInfo
    {
        unsigned int    predecessorNode;
        TWeightType     pathLength;
    };
    
    std::map< unsigned int, unsigned int >  targetMap;
    std::vector< unsigned int >             targetCodes;
    std::map< std::string, unsigned int >   nodesByText;
    iterator                                currentNode;
    unsigned short                          dictionaryProposalCode;
    unsigned int                            textMaxLength;
    TWeightType                             backspaceWeight;
    TFailstate                              failstate;
};

inline
unsigned short
TSpellerTree::DictionaryProposalCode() const
{
    return dictionaryProposalCode;
}

inline
bool
TSpellerTree::Fail() const
{
    return failstate != good;
}

inline
bool
TSpellerTree::BackspaceFail() const
{
    return ( failstate & backspaceFail ) != 0;
}

inline
const std::string&
TSpellerTree::GetAudioLabel() const
{
    return currentNode != end() ? currentNode->audioLabel : std::string( "" );
}

#endif // SPELLERTREEH
