/////////////////////////////////////////////////////////////////////////////
//
// File: PresSpellerModel.cpp
//
// Date: Oct 18, 2001
//
// Author: Juergen Mellinger
//
// Description:
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

#ifdef __BORLANDC__
#include "PCHIncludes.h"
#pragma hdrstop
#endif // __BORLANDC__

#include <string>
#include <sstream>
#include <cassert>

#include "PresSpellerModel.h"
#include "PresParams.h"
#include "Views/PresViews.h"
#include "SpellerTree.h"
#include "Views/TextFrame.h"
#include "ParamList.h"

using namespace std;

TPresSpellerModel::TPresSpellerModel(   ParamList        *inParamList,
                                        TPresBroadcaster */*inBroadcaster*/ )
: TPresModel( inParamList ),
  mode( 0 ),
  autoBackspace( 0 ),
  documentFrame( NULL ),
  textEntryFrame( NULL ),
  showingProposal( false ),
  failureReported( false )
{
    targetCode.AttachState( "TargetCode" );
    artifact.AttachOptionalState( "Artifact", 0 );

    PARAM_ENABLE( inParamList, PRSpellerMode );
    PARAM_ENABLE( inParamList, PRTreeFile );
    PARAM_ENABLE( inParamList, PRDictionaryFile );
    PARAM_ENABLE( inParamList, PRCurrentDocument );
    PARAM_ENABLE( inParamList, PRDocumentRect );
    PARAM_ENABLE( inParamList, PRTextEntryRect );
    PARAM_ENABLE( inParamList, PRFeedbackRect );
    PARAM_ENABLE( inParamList, PRDocumentFont );
    PARAM_ENABLE( inParamList, PRTextEntryFont );
}

TPresSpellerModel::~TPresSpellerModel()
{
    PARAM_DISABLE( curParamList, PRSpellerMode );
    PARAM_DISABLE( curParamList, PRTreeFile );
    PARAM_DISABLE( curParamList, PRDictionaryFile );
    PARAM_DISABLE( curParamList, PRDocumentRect );
    PARAM_DISABLE( curParamList, PRTextEntryRect );
    PARAM_DISABLE( curParamList, PRFeedbackRect );
    PARAM_DISABLE( curParamList, PRDocumentFont );
    PARAM_DISABLE( curParamList, PRTextEntryFont );

    DoCleanup();
}

TPresError
TPresSpellerModel::DoInitialize( ParamList        *inParamList,
                                 TPresBroadcaster *inBroadcaster )
{
    TPresError  err;
    int         numberOfTargets;

    PARAM_GET_NUM( inParamList, NumberTargets, numberOfTargets );
    PARAM_GET_NUM( inParamList, PRSpellerMode, mode );

    // create and initialize views
    TGUIRect    feedbackRect;
    Param       *feedbackRectPtr;

    PARAM_GET_PTR( inParamList, PRFeedbackRect, feedbackRectPtr );
    err = feedbackRect.ReadFromParam( feedbackRectPtr );
    if( err != presNoError )
    {
        gPresErrors.AddError( err, PARAM_NAME( PRFeedbackRect ) );
        return err;
    }

    TPresView   *view;
    view = new TFeedbackView( inParamList );
    views.push_back( view );
    err = view->Initialize( inParamList, feedbackRect );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( view );

    view = new TBackgroundView( inParamList );
    views.push_back( view );
    err = view->Initialize( inParamList, feedbackRect );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( view );

    for( int i = 0; i <= numberOfTargets; ++i ) // We have a view for the null target, too.
    {
        TTargetView *targetView = new TTargetView( inParamList, i );
        views.push_back( targetView );
        targetViews.push_back( targetView );
        err = targetView->Initialize( inParamList,  feedbackRect );
        if( err != presNoError )
            return err;
        inBroadcaster->AttachListener( targetView );
    }

    view = new TMarkerView( inParamList );
    views.push_back( view );
    err = view->Initialize( inParamList, feedbackRect );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( view );

    view = new TReinforcementView( inParamList );
    views.push_back( view );
    err = view->Initialize( inParamList, feedbackRect );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( view );

    view = new TArtifactView( inParamList );
    views.push_back( view );
    err = view->Initialize( inParamList, feedbackRect );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( view );

    // Add ourselves to the list of listeners.
    inBroadcaster->AttachListener( this );

    // initialize parameters
    if( mode == 1 || mode == 2 ) // speller or copy speller
    {
        // build the text frames
        Param   *paramPtr;
        documentFrame = new TTextFrame;
        PARAM_GET_PTR( inParamList, PRDocumentRect, paramPtr );
        err = documentFrame->SetRect( paramPtr );
        if( err != presNoError )
            gPresErrors.AddError( err, PARAM_NAME( PRDocumentRect ) );
        PARAM_GET_PTR( inParamList, PRDocumentFont, paramPtr );
        err = documentFrame->SetTextProperties( paramPtr );
        if( err != presNoError )
            gPresErrors.AddError( err, PARAM_NAME( PRDocumentFont ) );

        textEntryFrame = new TTextFrame;
        PARAM_GET_PTR( inParamList, PRTextEntryRect, paramPtr );
        err = textEntryFrame->SetRect( paramPtr );
        if( err != presNoError )
            gPresErrors.AddError( err, PARAM_NAME( PRTextEntryRect ) );
        PARAM_GET_PTR( inParamList, PRTextEntryFont, paramPtr );
        err = textEntryFrame->SetTextProperties( paramPtr );
        if( err != presNoError )
            gPresErrors.AddError( err, PARAM_NAME( PRTextEntryFont ) );

        // Initialize the speller tree.
        const char  *treeFile;
        PARAM_GET_STRING( inParamList, PRTreeFile, treeFile );
        err = spellerTree.ReadFromFile( treeFile );
        if( err == presNoError )
            for( std::list< TTargetView* >::iterator i = targetViews.begin();
                    i != targetViews.end(); ++i )
                        ( *i )->SetLabel( spellerTree.GetLabel( ( *i )->GetTargetCode() ) );

        // Initialize the dictionary.
        const char  *dictionaryFile;
        PARAM_GET_STRING( inParamList, PRDictionaryFile, dictionaryFile );
        if( *dictionaryFile != '\0' )
            spellerDict.ReadFromFile( dictionaryFile );
    }

    if( mode == 1 ) // speller
    {
        const char  *currentDocument;
        PARAM_ENABLE( inParamList, PRCurrentDocument );
        PARAM_GET_STRING( inParamList, PRCurrentDocument, currentDocument );
        documentFrame->LoadText( currentDocument );
        textEntryFrame->ShowCursor();
    }
    else if( mode == 2 ) // copy speller
    {
        const char  *textToCopy;
        PARAM_ENABLE( inParamList, PRTextToCopy );
        PARAM_GET_STRING( inParamList, PRTextToCopy, textToCopy );
        string  normalizedText( textToCopy );
        err = spellerTree.NormalizeText( normalizedText );
        if( err != presNoError )
            return err;
        documentFrame->SetText( normalizedText );
        textEntryFrame->ShowCursor();
    }

    return presNoError;
}

void
TPresSpellerModel::DoCleanup()
{
    if( mode == 1 ) // speller
    {
        const char  *currentDocument;
        PARAM_GET_STRING( curParamList, PRCurrentDocument, currentDocument );
        documentFrame->SaveText( currentDocument );
        PARAM_DISABLE( curParamList, PRCurrentDocument );
    }
    else if( mode == 2 ) // copy speller
        PARAM_DISABLE( curParamList, PRTextToCopy );

    delete documentFrame;
    delete textEntryFrame;
}

void
TPresSpellerModel::Reset()
{
  if( mode == 1 || mode == 2 )
  {
    spellerTree.Reset();
    failureReported = false;
  }
}

void
TPresSpellerModel::NextTarget()
{
  if( mode == 2 ) // copy speller
  {
    int curTargetCode = spellerTree.NextChoiceInShortestPath(
                            textEntryFrame->GetText(), documentFrame->GetText() );

    if( spellerTree.BackspaceFail() )
    {
      do
      {
        spellerTree.Reset();
        textEntryFrame->AddText( "\b" );
        curTargetCode = spellerTree.NextChoiceInShortestPath(
                            textEntryFrame->GetText(), documentFrame->GetText() );
      }
      while( spellerTree.BackspaceFail() );
    }
    else if( spellerTree.Fail() )
    {
      if( !failureReported )
      {
        gPresErrors << "The given text is impossible to spell "
                        "with the current speller tree." << endl;
        failureReported = true;
      }
      spellerTree.Reset();
    }
    else
      failureReported = false;

    targetCode.SetStateValue( curTargetCode );
  }

  if( showingProposal )
    spellerWavePlayer.SetFile( ( const char* )( NULL ) );
  else
    spellerWavePlayer.SetFile( spellerTree.GetAudioLabel().c_str() );
}

void
TPresSpellerModel::ProcessTaskBegin(    const TEventArgs& )
{
    spellerWavePlayer.Play();
}

void
TPresSpellerModel::ProcessEndOfClass(   const TEventArgs& inArgs )
{
    if( artifact.GetStateValue() == 1 )
      return;

    switch( mode )
    {
        case 0: // none
            break;
        case 1: // speller
            if( showingProposal )
            {
                if( spellerTree.DictionaryProposalCode() == inArgs.resultCode )
                {
                    textEntryFrame->SetText( lastProposal );
                    spellerTree.Reset();
                }
                showingProposal = false;
                for( std::list< TTargetView* >::iterator i = targetViews.begin();
                        i != targetViews.end(); ++i )
                            ( *i )->SetLabel( spellerTree.GetLabel( ( *i )->GetTargetCode() ) );
            }
            else
            {
                const std::string&  result = spellerTree.Choose( inArgs.resultCode );

                if( result.length() > 0 && result[ result.length() - 1 ] == ' ' )
                {
                    documentFrame->AddText( textEntryFrame->GetText() );
                    documentFrame->AddText( result );
                    textEntryFrame->SetText( "" );
                }
                else
                    textEntryFrame->AddText( result );

                // Look up in the dictionary and show the proposal if one is found.
                lastProposal = spellerDict.Lookup( textEntryFrame->GetText() );
                if( lastProposal != "" && lastProposal != textEntryFrame->GetText() )
                {
                    showingProposal = true;
                    for( std::list< TTargetView* >::iterator i = targetViews.begin();
                            i != targetViews.end(); ++i )
                                if( ( *i )->GetTargetCode() == spellerTree.DictionaryProposalCode() )
                                    ( *i )->SetLabel( lastProposal );
                                else
                                    ( *i )->SetLabel( "" );
                }
                else
                    for( std::list< TTargetView* >::iterator i = targetViews.begin();
                            i != targetViews.end(); ++i )
                                ( *i )->SetLabel( spellerTree.GetLabel( ( *i )->GetTargetCode() ) );
            }
            break;
        case 2: // copy speller
            {
                const std::string&  result = spellerTree.Choose( inArgs.resultCode );
                textEntryFrame->AddText( result );
                for( std::list< TTargetView* >::iterator i = targetViews.begin();
                        i != targetViews.end(); ++i )
                            ( *i )->SetLabel( spellerTree.GetLabel( ( *i )->GetTargetCode() ) );
            }
            break;
        default:
            assert( false );
    }
}


