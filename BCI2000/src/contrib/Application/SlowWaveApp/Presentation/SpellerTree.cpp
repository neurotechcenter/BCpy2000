/////////////////////////////////////////////////////////////////////////////
//
// File: SpellerTree.cpp
//
// Date: Dec 13, 2001
//
// Author: Juergen Mellinger
//
// Description: A class that reads and manages a speller tree
//              with arbitrary depth and an arbitrary number of
//              choices at each node.
//
// Changes: Feb 6, 2003, jm: ReadFromFile now checks whether all nodes
//                           can be reached from the root node;
//                           will not loop infinitely if the backspace
//                           node cannot be reached from the root node.
//          Jul 7, 2003, jm: Trainers as well as users are equally puzzled
//                           by the idea that entering a wrong letter and
//                           subsequently deleting it may actually be
//                           quicker than going back the tree. So it's
//                           considered infinitely costly unless someone
//                           #defines REALLY_SHORTEST_PATHS.
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

#ifdef __BORLANDC__
#include "PCHIncludes.h"
#pragma hdrstop
#endif // __BORLANDC__

#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <cassert>
#include <limits>

#include "SpellerTree.h"
#include "PresErrors.h"
#include "Utils/Util.h"

using namespace std;

TSpellerTree::TSpellerTree()
: failstate( good ),
  currentNode( begin() ),
  dictionaryProposalCode( 0 ),
  textMaxLength( 0 ),
  backspaceWeight( infiniteWeight )
{
}

TSpellerTree::~TSpellerTree()
{
}

void
TSpellerTree::Reset()
{
    currentNode = begin();
    failstate = good;
}

const string&
TSpellerTree::Choose( unsigned int inTargetCode )
{
    static string   emptyString = "";
    const string*   retString = &emptyString;
    unsigned int    index = targetMap[ inTargetCode ];

    // If the target code is in the target codes list,
    // do the associated choice.
    if( index > 0 && index <= currentNode->choices.size() )
    {
        retString = &currentNode->texts[ index - 1 ];
        currentNode = &at( currentNode->choices[ index - 1 ] );
    }

    return *retString;
}

const string&
TSpellerTree::GetLabel( unsigned int inTargetCode )
{
    static string   emptyString = "";
    const string*   retString = &emptyString;
    unsigned int    index = targetMap[ inTargetCode ];

    if( index > 0 && index <= currentNode->labels.size() )
        retString = &currentNode->labels[ index - 1 ];

    return *retString;
}

unsigned int
TSpellerTree::NextChoiceInShortestPath( const string& inCurrentText,
                                        const string& inTargetText )
{
    unsigned int returnChoice = 0;

    // Reduce the call to a call with an empty first argument, i.e.
    // determine the characters that remain to be entered.
    if( inCurrentText != "" )
    {
        // Determine how many characters are entered correctly and add backspaces from
        // there. We assume that entering backspaces is the only way to correct spelling
        // mistakes.
        unsigned int curLen = inCurrentText.length();
        while( curLen > 0 && inTargetText.find( inCurrentText.substr( 0, curLen ) ) != 0 )
            --curLen;
        string  newTargetText = inTargetText.substr( curLen );
        // Add the appropriate number of backspaces before newTargetText.
        for( unsigned int i = 0; i < inCurrentText.length() - curLen; ++i )
            newTargetText = '\b' + newTargetText;
        returnChoice = NextChoiceInShortestPath( "", newTargetText );
    }
    else if( inTargetText != "" )
    {
        // Find the shortest path from the current position in the tree to entering
        // the beginning of inTargetText.

        // First, find the node that will enter as much as possible of inTargetText.
        unsigned int    length = textMaxLength,
                        index = 0;
        string          entryText = inTargetText.substr( 0, length );
        while( nodesByText.find( entryText ) == nodesByText.end() && length > 0 )
            entryText = inTargetText.substr( 0, --length );
        if( length > 0 )
        {
            // We found a node that enters the beginning of the target text.
            index = nodesByText[ entryText ];
        }
        else if( textMaxLength > 1 )
        {
            // We didn't find a node that enters the text correctly. Find a node
            // that enters as much correct text as possible
            // (additional text is supposed to be deleted afterwards).
            length = textMaxLength - 1;
            bool    found = false;
            map< string, unsigned int >::iterator i;
            while( !found && length > 0 )
            {
                i = nodesByText.begin();
                while( i != nodesByText.end() && inTargetText.find( i->first.substr( 0, length ) ) != 0 )
                    ++i;
                if( i != nodesByText.end() )
                    found = true;
                --length;
            }
            if( found )
            {
                entryText = i->first;
                index = i->second;
            }
        }

        if( index > 0 )
        {
            iterator    targetNode = &at( index - 1 );
            if( targetNode == currentNode )
            // We are already at the node we want. Find the target code that enters
            // the text we need.
            {
                vector< string >::iterator text = currentNode->texts.begin();
                while( text != currentNode->texts.end() && *text != entryText )
                    ++text;
                assert( text != currentNode->texts.end() );
                returnChoice = targetCodes[ text - currentNode->texts.begin() ];
            }
            else
                returnChoice = NextChoiceInShortestPath( currentNode, targetNode );
        }
        else // There is no node that will enter the text needed.
        {
            // Is the failure due to a missing backspace node in the tree?
            if( ( backspaceWeight == infiniteWeight )
                    && ( inTargetText.length() > 0 )
                    && ( inTargetText[ 0 ] == '\b' ) )
                failstate = backspaceFail;
            else
                failstate = generalFail;
        }
    }

    return returnChoice;
}

TSpellerTree::TWeightType
TSpellerTree::Weight( const iterator node, size_t choice ) const
{
  TWeightType connectorWeight = 1;
  string textAssociatedWithChoice = node->texts[ choice ];
  if( textAssociatedWithChoice != "" )
  {
    // Never consider entering an unwanted backspace.
    if( textAssociatedWithChoice.find( "\b" ) != string::npos )
      connectorWeight = infiniteWeight;
    else
      connectorWeight = backspaceWeight;
  }
  return connectorWeight;
}

unsigned int
TSpellerTree::NextChoiceInShortestPath( iterator inStartNode, iterator inTargetNode )
{
  assert( inStartNode != inTargetNode );

  // Find the shortest path from the start node to the target node.
  // We consider the speller tree a directed graph. Each connector has a
  // weight of 1, except the connectors from text entering leaves back to the root node.
  // These connectors' weights equal the number of choices from root to
  // the backspace character.
  TPresError      err = presNoError;
  unsigned int    startNode = inStartNode - begin(),
                  targetNode = inTargetNode - begin();
  unsigned int    retVal = 0;

  typedef map< unsigned int, TPathInfo >      pathsMap;
  typedef map< unsigned int, unsigned int >   nodeSet;

  pathsMap    shortestPaths;
  shortestPaths[ startNode ].predecessorNode = startNode;
  shortestPaths[ startNode ].pathLength = 0;

  // While the target node is not in shortestPaths, consider nodes that can be
  // reached from the nodes in shortestPaths.
  while( shortestPaths.find( targetNode ) == shortestPaths.end() )
  {
    TWeightType shortestPath = infiniteWeight;
    nodeSet     minimumNodes;
    for( pathsMap::iterator pathFromLastSet = shortestPaths.begin();
            pathFromLastSet != shortestPaths.end(); ++pathFromLastSet )
    {
      unsigned int    nodeFromLastSet = pathFromLastSet->first;
      iterator        atNodeFromLastSet = &at( nodeFromLastSet );
      for( vector<unsigned int>::iterator choice = atNodeFromLastSet->choices.begin();
            choice != atNodeFromLastSet->choices.end(); ++choice )
      {
        // Ignore connectors to nodes that are already in the set.
        if( shortestPaths.find( *choice ) == shortestPaths.end() )
        {
          TWeightType connectorWeight =
             Weight( atNodeFromLastSet, choice - atNodeFromLastSet->choices.begin() );
          if( shortestPaths[ nodeFromLastSet ].pathLength + connectorWeight < shortestPath )
          {
            minimumNodes.clear();
            shortestPath = shortestPaths[ nodeFromLastSet ].pathLength + connectorWeight;
          }
          if( shortestPaths[ nodeFromLastSet ].pathLength + connectorWeight == shortestPath )
            minimumNodes[ *choice ] = nodeFromLastSet;
        }
      }
    }
    for( nodeSet::iterator i = minimumNodes.begin();
            i != minimumNodes.end(); ++i )
    {
      shortestPaths[ i->first ].pathLength = shortestPath;
      shortestPaths[ i->first ].predecessorNode = i->second;
    }
  }
  // Now we got the length minima and the predecessor nodes in the shortestPaths map.
  // Go back along the path to determine the next choice to make.
  if( err == presNoError )
  {
    // We need to consider the case of more that one connector between
    // two nodes, differring in weight.
    unsigned int nextNode = targetNode;
    while( shortestPaths[ nextNode ].predecessorNode != startNode )
      nextNode = shortestPaths[ nextNode ].predecessorNode;
    TWeightType minWeight = infiniteWeight;
    size_t minChoice = inStartNode->choices.size();
    for( size_t i = 0; i < inStartNode->choices.size(); ++i )
    {
      if( inStartNode->choices[ i ] == nextNode )
      {
        TWeightType weight = Weight( inStartNode, i );
        if( weight < minWeight )
        {
          minWeight = weight;
          minChoice = i;
        }
      }
    }
    if( minChoice == inStartNode->choices.size() )
      err = presIllegalSpellerTreeError;
    else
      retVal = targetCodes[ minChoice ];
  }

  if( err != presNoError )
  {
    gPresErrors << "Copy speller could not determine next speller choice." << endl;
    failstate = generalFail;
  }

  return retVal;
}

TPresError
TSpellerTree::NormalizeText( string& ioText )
{
#if 0
    // Adapt a given text to the capabilities of the current speller tree.
    // For now, only a simple conversion to uppercase is performed and
    // the '-' character is replaced by a space character.
    // For the future, the speller tree definition might contain a conversion
    // table.

    for( string::iterator i = ioText.begin(); i != ioText.end(); ++i )
    {
        *i = toupper( *i );
        switch( *i )
        {
            case 'ä':
                *i = 'Ä';
                break;
            case 'ö':
                *i = 'Ö';
                break;
            case 'ü':
                *i = 'Ü';
                break;
            case 'ß':
                *i = 'S';
                ioText.insert( i, 'S' );
                break;
            case '-':
                *i = ' ';
                break;
        }
    }

    return presNoError;
#else
 // just replace '_' characters by space characters.
    for( string::iterator i = ioText.begin(); i != ioText.end(); ++i )
    {
        switch( *i )
        {
            case '_':
                *i = ' ';
                break;
        }
    }

    return presNoError;
#endif
}

TPresError
TSpellerTree::ReadFromFile( const char  *inTreeFileName )
{
    TPresError  err = presNoError;

    size_t lineNo = 0;
    clear();
    targetMap.clear();
    targetCodes.clear();
    nodesByText.clear();
    textMaxLength = 0;

    Util::TPath         curPath;
    string              inputFileName = curPath + inTreeFileName;
    ifstream            inputFile( inputFileName.c_str() );
    bool                gotTargetCodes = false;

    if( !inputFile.is_open() )
        err = presFileOpeningError;

    while( err == presNoError && !inputFile.eof() )
    {
        ++lineNo;
        if( inputFile.peek() == '#' ) // ignore comment lines
            inputFile.ignore( numeric_limits<streamsize>::max(), '\n' );
        else
        {
            string              inputLine;

            getline( inputFile, inputLine );
            if( !inputFile.fail() )
            {
                istringstream   lineStream( inputLine );
                unsigned int    nodeID;

                lineStream >> nodeID;

                if( lineStream.fail() )
                {
                    string  optionIdentifier;

                    lineStream.clear();
                    lineStream >> optionIdentifier;
                    if( optionIdentifier == "TargetCodes" || optionIdentifier == "targetCodes" )
                    {
                        unsigned int    curIndex = 1,
                                        targetCode;

                        lineStream >> targetCode;
                        while( !lineStream.fail() )
                        {
                            targetMap[ targetCode ] = curIndex++;
                            targetCodes.push_back( targetCode );
                            lineStream >> targetCode;
                        }

                        // Check for double target code entries.
                        if( targetMap.size() != curIndex - 1 )
                            err = presIllegalSpellerTreeError;
                        gotTargetCodes = true;
                    }
                    else if( optionIdentifier == "DictionaryProposal" || optionIdentifier == "dictionaryProposal" )
                    {
                        lineStream >> dictionaryProposalCode;
                        if( lineStream.fail() )
                        {
                            dictionaryProposalCode = 0;
                            err = presIllegalSpellerTreeError;
                        }
                    }
                    else if( optionIdentifier == "Audio" || optionIdentifier == "audio" )
                    {
                        if( size() < 1 )
                            err = presIllegalSpellerTreeError;
                        else
                        {
                            string  audioLabel;
                            lineStream >> audioLabel;
                            at( size() - 1 ).audioLabel = curPath + audioLabel;
                        }
                    }
                    else if( optionIdentifier == "Path" || optionIdentifier == "path" )
                    {
                        string  pathRead;
                        char    c;

                        lineStream >> c;
                        if( !lineStream.fail() )
                        {
                            if( c == '"' )
                                getline( lineStream, pathRead, '"' );
                            else
                            {
                                lineStream >> pathRead;
                                pathRead = c + pathRead;
                            }
                            curPath = Util::TPath( pathRead );
                        }
                    }
                    else
                        err = presIllegalSpellerTreeError;
                }
                else if( gotTargetCodes )
                {
                    unsigned int    choiceNode;

                    if( nodeID == size() )
                    // We need to create a new node entry.
                        resize( size() + 1 );
                    // Node entries must be ordered by node ID.
                    if( nodeID != size() - 1 )
                        err = presIllegalSpellerTreeError;
                    else
                    {
                        TSpellerTreeNode&   curNode = at( nodeID );
                        char                c;

                        lineStream >> c;
                        if( c == '"' )
                        {
                            string  labelPart,
                                    label;
                            getline( lineStream, labelPart, '"' );
                            label += labelPart;
                            // The while loop is for handling the \" literal.
                            while( label.length() > 0 && label[ label.length() - 1 ] == '\\' )
                            {
                                label += '"';
                                getline( lineStream, labelPart, '"' );
                                label += labelPart;
                            }
                            curNode.labels.push_back( Util::ConvertLiterals( label ) );

                            lineStream >> c;
                            if( c == '"' )
                            {
                                string  textPart,
                                        text;
                                getline( lineStream, textPart, '"' );
                                text += textPart;
                                // The while loop is for handling the \" literal.
                                while( text.length() > 0 && text[ text.length() - 1 ] == '\\' )
                                {
                                    text += '"';
                                    getline( lineStream, textPart, '"' );
                                    text += textPart;
                                }
                                text = Util::ConvertLiterals( text );
                                if( text.length() > textMaxLength )
                                    textMaxLength = text.length();
                                curNode.texts.push_back( text );
                            }
                        }

                        lineStream >> choiceNode;
                        curNode.choices.push_back( choiceNode );

                        if( lineStream.fail() )
                            err = presIllegalSpellerTreeError;
                    }
                }
                else
                    err = presIllegalSpellerTreeError;
            }
        }
    }

    if( err == presNoError )
    {
        unsigned int numNodes = size();

        if( numNodes < 1 ) // We need at least one node in the tree.
            err = presIllegalSpellerTreeError;

        // Check if all choice entries point to existing nodes.
        for( iterator i = begin(); i != end() && err == presNoError; ++i )
            for( vector< unsigned int >::iterator j = i->choices.begin();
                                        j != i->choices.end() && err == presNoError; ++j )
                if( *j >= numNodes )
                    err = presIllegalSpellerTreeError;
    }

    if( err != presNoError )
        gPresErrors.AddError( err, inputFileName.c_str(), lineNo );

    currentNode = begin();

    // We determine shortest paths for all nodes to check whether all
    // can be reached from the root node.
    if( err == presNoError )
    {
        // We consider the speller tree a directed graph. Each connector has a
        // weight of 1, except the connectors from text entering leaves back to the root node.
        // These connectors' weights equal the number of choices from root to
        // the backspace character.

        // Determine the length of the shortest path from the root node (0) to
        // each of the other nodes if reachable from the root node.
        // All connectors have a weight of 1.
        typedef map< unsigned int, TWeightType >    pathsMap;
        typedef set< unsigned int >                 nodeSet;
        pathsMap    shortestPaths;
        size_t shortestPathsLastSize = 0;
        shortestPaths[ 0 ] = 1;
        // Repeat until all nodes reachable from the root node have been entered
        // into shortestPaths.
        while( shortestPaths.size() > shortestPathsLastSize )
        {
            shortestPathsLastSize = shortestPaths.size();
            TWeightType shortestPath = infiniteWeight;
            nodeSet     minimumNodes;
            for( pathsMap::iterator pathFromLastSet = shortestPaths.begin();
                    pathFromLastSet != shortestPaths.end(); ++pathFromLastSet )
            {
                unsigned int    nodeFromLastSet = pathFromLastSet->first;
                iterator        atNodeFromLastSet = &at( nodeFromLastSet );
                for( vector< unsigned int >::iterator choice = atNodeFromLastSet->choices.begin();
                        choice != atNodeFromLastSet->choices.end(); ++choice )
                {
                    if( shortestPaths.find( *choice ) == shortestPaths.end() )
                    {
                        if( shortestPaths[ nodeFromLastSet ] + 1 < shortestPath )
                        {
                            minimumNodes.clear();
                            shortestPath = shortestPaths[ nodeFromLastSet ] + 1;
                        }
                        if( shortestPaths[ nodeFromLastSet ] + 1 == shortestPath )
                            minimumNodes.insert( *choice );
                    }
                }
            }
            for( nodeSet::iterator i = minimumNodes.begin();
                    i != minimumNodes.end(); ++i )
                        shortestPaths[ *i ] = shortestPath;
        }
        nodeSet orphans;
        for( size_t i = 0; i < size(); ++i )
          if( shortestPaths.find( i ) != shortestPaths.end() )
          // If the node is reachable, put its entry texts into the text->node map.
          {
            TSpellerTreeNode& curNode = at( i );
            for( vector<string>::iterator j = curNode.texts.begin(); j != curNode.texts.end(); ++j )
              if( *j != "" )
                nodesByText[ *j ] = i + 1;
          }
          else
          // If the node is unreachable, put it into the "orphans" set.
            orphans.insert( i );

        if( orphans.size() > 0 )
        // There are nodes that cannot be reached from the root node.
        // Issue a warning.
        {
           gPresErrors << "Warning: Speller tree " << inputFileName.c_str()
                       << " contains ";
           if( orphans.size() == 1 )
             gPresErrors << "an orphan node: ";
           else
             gPresErrors << orphans.size() << " orphan nodes:\n";
           gPresErrors << *orphans.begin();
           for( nodeSet::iterator i = ++orphans.begin(); i != orphans.end(); ++i )
             gPresErrors << ", " << *i;
           gPresErrors << endl;
        }

        if( err == presNoError && nodesByText.find( "\b" ) != nodesByText.end() )
        {
          size_t backspaceNode = nodesByText[ "\b" ] - 1;
#ifdef REALLY_SHORTEST_PATHS
// As of July 2003, trainers and users are equally puzzled by the idea that
// entering a wrong letter and subsequently deleting it may actually be
// quicker than going back the tree. So it's considered infinitely costly
// unless someone #defines REALLY_SHORTEST_PATHS.
          backspaceWeight = shortestPaths[ backspaceNode ];
#else
          backspaceWeight = infiniteWeight;
#endif // REALLY_SHORTEST_PATHS
          assert( backspaceWeight > 0 );
        }
    }

    return err;
}

