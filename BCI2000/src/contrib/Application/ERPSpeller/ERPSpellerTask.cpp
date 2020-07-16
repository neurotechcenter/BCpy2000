////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors:
// Description: ERPSpellerTask implementation
//
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
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "ERPSpellerTask.h"

#include "SpellerCommand.h"
#include "TextGroupStimulus.h"
#include "ImageStimulus.h"
#include "SoundStimulus.h"
#include "SpeechStimulus.h"
#include "AudioSpellerTarget.h"

#include "Localization.h"
#include "FileUtils.h"
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace GUI;

RegisterFilter( ERPSpellerTask, 3 );

ERPSpellerTask::ERPSpellerTask()
: mNumberOfSequences( 0 ),
  mInterpretMode_( InterpretModes::None ),
  mDisplayResults( false ),
  mTestMode( false ),
  mCurMenu( 0 ),
  mNumMatrixRows( 0 ),
  mNumMatrixCols( 0 ),
  mSequenceCount( 0 ),
  mSequencePos( mSequence.begin() ),
  mAvoidStimulusRepetition( false ),
  mSleepMode( 0 ),
  mPaused( false ),
  mpStatusBar( NULL ),
  mpTextWindow( NULL ),
  mSummaryFile( SummaryFileExtension().c_str() ),
  mRunCount( 0 ),
  mNumSelections( 0 ),
  mSleepDuration( 0 ),
  mConnection( &mConnectionBuffer )
{
  mConnectionBuffer.SetIO( &mSocket );

  BEGIN_PARAMETER_DEFINITIONS
   "Application:Sequencing int NumberOfSequences= 15 15 1 % // "
      "number of sequences in a set of intensifications",

   "Application:Speller%20Targets matrix TargetDefinitions= "
   "7 { Display } "
   "{ matrix 6 { Display Enter Display%20Size Icon%20File Sound%20File } A A<GTO7> 1 % % B B<GTO7> 1 % % C C<GTO7> 1 % % D D<GTO7> 1 % % E E<GTO7> 1 % % F F<GTO7> 1 % % } "
   "{ matrix 6 { Display Enter Display%20Size Icon%20File Sound%20File } G G<GTO7> 1 % % H H<GTO7> 1 % % I I<GTO7> 1 % % J J<GTO7> 1 % % K K<GTO7> 1 % % L L<GTO7> 1 % % } "
   "{ matrix 6 { Display Enter Display%20Size Icon%20File Sound%20File } M M<GTO7> 1 % % N N<GTO7> 1 % % O O<GTO7> 1 % % P P<GTO7> 1 % % Q Q<GTO7> 1 % % R R<GTO7> 1 % % } "
   "{ matrix 6 { Display Enter Display%20Size Icon%20File Sound%20File } S S<GTO7> 1 % % T T<GTO7> 1 % % U U<GTO7> 1 % % V V<GTO7> 1 % % W W<GTO7> 1 % % X X<GTO7> 1 % % } "
   "{ matrix 6 { Display Enter Display%20Size Icon%20File Sound%20File } Y Y<GTO7> 1 % % Z Z<GTO7> 1 % % 1 1<GTO7> 1 % % 2 2<GTO7> 1 % % 3 3<GTO7> 1 % % 4 4<GTO7> 1 % % } "
   "{ matrix 6 { Display Enter Display%20Size Icon%20File Sound%20File } 5 5<GTO7> 1 % % 6 6<GTO7> 1 % % 7 7<GTO7> 1 % % 8 8<GTO7> 1 % % 9 9<GTO7> 1 % % _ _<GTO7> 1 % % } "
   "{ matrix 6 { Display Enter Display%20Size Icon%20File Sound%20File } A-F <GTO1> 0.5 % % G-L <GTO2> 0.5 % % M-R <GTO3> 0.5 % % S-X <GTO4> 0.5 % % Y-4 <GTO5> 0.5 % % 5-_ <GTO6> 0.5 % % } "
   "// speller target properties",
   
   "Application:Speller%20Targets intlist NumMatrixColumns= 7 3 3 3 3 3 3 3 "
      "3 3 % // display matrices' column number(s)",
   "Application:Speller%20Targets intlist NumMatrixRows= 7 2 2 2 2 2 2 2 "
      "2 2 % // display matrices' row number(s)",

   "Application:Audio%20Stimuli int AudioStimuliOn= 0 "
      "0 0 1 // Audio Stimuli Mode (0=no, 1=yes) (boolean)",
   "Application:Audio%20Stimuli matrix AudioStimuliRowsFiles= "
      "{ 1 2 3 4 5 6 } " // row labels
      "{ filename } " // filename
      "./voice/1.wav "
      "./voice/2.wav "
      "./voice/3.wav "
      "./voice/4.wav "
      "./voice/5.wav "
      "./voice/6.wav "
      " // audio stimuli rows files ",
   "Application:Audio%20Stimuli matrix AudioStimuliColsFiles= "
      "{ 1 2 3 4 5 6 } " // column labels
      "{ filename } " // filename
      "./voice/a.wav "
      "./voice/b.wav "
      "./voice/c.wav "
      "./voice/d.wav "
      "./voice/e.wav "
      "./voice/f.wav "
      " // audio stimuli column files ",

   "Application:Speller%20Targets floatlist TargetWidth= 1 16 0 0 100 // "
      "target width in percent of screen width",
   "Application:Speller%20Targets floatlist TargetHeight= 1 14 0 0 100 // "
      "target height in percent of screen height",
   "Application:Speller%20Targets floatlist TargetTextHeight= 1 12 0 0 100 // "
      "height of target labels in percent of screen height",
   "Application:Speller%20Targets stringlist BackgroundColor= 1 0x000000 "
      "0x505050 % % // target background color (color)",
   "Application:Speller%20Targets stringlist TextColor= 1 0x00555555 "
      "0x505050 % % // text color (color)",
   "Application:Speller%20Targets stringlist TextColorIntensified= 1 0x00FFFFFF "
      "0x505050 % % // intensified text color (color)",
   "Application:Speller%20Targets intlist IconHighlightMode= 1 1 "
      "1 0 4 // icon highlight method "
        "0: Show/Hide, "
        "1: Intensify, "
        "2: Grayscale, "
        "3: Invert, "
        "4: Dim "
        "   (enumeration)",
   "Application:Speller%20Targets floatlist IconHighlightFactor= 1 0.5 "
      "0.5 0 % // scale factor for highlighted icon pixel values",

   "Application:Speller int FirstActiveMenu= 7 "
      "1 1 % // Index of first active menu",
   "Application:Speller float StatusBarSize= 10 0 0 100 // "
      "size of status bar in percent of screen height",
   "Application:Speller float StatusBarTextHeight= 4 0 0 100 // "
      "size of status bar text in percent of screen height",
   "Application:Speller string TextToSpell= % % % % // "
    " character or string to spell in offline copy mode",
   "Application:Speller string TextResult= % % % % // "
      "user spelling result",
   "Application:Speller int TestMode= 0 0 0 1 // "
      "select targets by clicking on their associated stimuli (0=no, 1=yes) (boolean)",
   "Application:Speller string DestinationAddress= % % % % // "
      "network address for speller output in IP:port format",

   "Application:Text%20Window int TextWindowEnabled= 0 "
      "0 0 1 // Show Text Window (0=no, 1=yes) (boolean)",
   "Application:Text%20Window int TextWindowLeft= 640 0 0 % // "
      "Text Window X location",
   "Application:Text%20Window int TextWindowTop= 0 0 0 % // "
      "Text Window Y location",
   "Application:Text%20Window int TextWindowWidth= 512 512 0 % // "
      "Text Window Width",
   "Application:Text%20Window int TextWindowHeight= 512 512 0 % // "
      "Text Window Height",
   "Application:Text%20Window string TextWindowFontName= Courier % % % // "
      "Text Window Font Name",
   "Application:Text%20Window int TextWindowFontSize= 10 4 1 % // "
      "Text Window Font Size",
   "Application:Text%20Window string TextWindowFilePath= % % % % // "
      "Path for Saved Text File (directory)",
 END_PARAMETER_DEFINITIONS

 BEGIN_STATE_DEFINITIONS
   "SelectedTarget 16 0 0 0",
   "SelectedRow     8 0 0 0",
   "SelectedColumn  8 0 0 0",
   "SpellerMenu     8 0 0 0",
 END_STATE_DEFINITIONS

 LANGUAGES "German",
 BEGIN_LOCALIZED_STRINGS
  "TIME OUT !!!",
           "Zeit abgelaufen!",
  "Waiting to start ...",
           "Warte ...",
  "Sleeping--Select SLEEP twice to resume",
           "Angehalten: Zweimal SLEEP fur Weiter",
  "Select SLEEP once more to resume",
           "Angehalten: Noch einmal SLEEP fur Weiter",
  "Paused--Select PAUSE again to resume",
           "Angehalten: Noch einmal PAUSE fur Weiter",
 END_LOCALIZED_STRINGS
}

ERPSpellerTask::~ERPSpellerTask()
{
  mStimuli.DeleteObjects();
  Speller::DeleteObjects();
  delete mpTextWindow;
}

void
ERPSpellerTask::OnPreflight( const SignalProperties& /*Input*/ ) const
{
  Parameter( "TextResult" );
  Parameter( "DisplayResults" );
  Parameter( "DestinationAddress" );
  PreflightCondition( Parameter( "FirstActiveMenu" ) <= Parameter( "TargetDefinitions" )->NumRows() );

  // Try loading all the menus to detect configuration errors.
  GUI::Rect rect;
  GraphDisplay preflightDisplay;
  SetOfStimuli preflightStimuli;
  AssociationMap preflightAssociations;
  struct : public Speller
  { void OnEnter( const std::string& ) {} } preflightSpeller;

  int numMenus = NumMenus(),
      interpretMode = Parameter( "InterpretMode" );
  for( int i = 0; i < numMenus; ++i )
  {
    LoadMenu(
      i,
      rect,
      preflightDisplay,
      preflightStimuli,
      preflightAssociations,
      preflightSpeller
    );
    if( interpretMode == InterpretModes::Copy && i + 1 == Parameter( "FirstActiveMenu" ) )
    {
      string commands;
      for( SetOfSpellerTargets::const_iterator j = preflightSpeller.Targets().begin(); j != preflightSpeller.Targets().end(); ++j )
      {
        istringstream iss( ( *j )->EntryText() );
        SpellerCommand command;
        while( iss >> command )
          if( !command.Code().empty() )
            commands += string( " " ) + command.Code();
      }
      if( !commands.empty() )
        bciout << "Speller commands are not supported in copy spelling "
               << "mode, and may result in inconsistent target suggestions. "
               << "Menu " << i + 1
               << " contains the following speller commands: "
               << commands
               << "."
               << endl;

      if( !preflightSpeller.TrySpelling( Parameter( "TextToSpell" ) ) )
        bcierr << "TextToSpell cannot be spelled using TargetDefinitions "
               << "in menu " << i + 1
               << endl;

      if( !preflightSpeller.TrySpelling( Parameter( "TextResult" ) ) )
        bcierr << "TextResult cannot be spelled using TargetDefinitions "
               << "in menu " << i + 1
               << endl;
    }
  }
  preflightStimuli.DeleteObjects();
  preflightSpeller.DeleteObjects();

  if( interpretMode == InterpretModes::Copy )
  {
    if( Parameter( "TextToSpell" ) == "" )
      bciout << "Empty TextToSpell parameter in copy spelling mode" << endl;
  }
  if( interpretMode != InterpretModes::None )
  {
    int numberOfSequences = Parameter( "NumberOfSequences" );
    if( OptionalParameter( "EpochsToAverage", numberOfSequences ) > numberOfSequences )
      bciout << "EpochsToAverage is larger than NumberOfSequences."
             << " This implies that multiple sequences enter into"
             << " a single classification."
             << endl;
  }

  State( "Running" );
  State( "Recording" );

  // Parameters of text window
  Parameter( "TextWindowFontName" );
  Parameter( "TextWindowFontSize" );
  Parameter( "TextWindowFilePath" );

  // Parameters accessed for the summary file only
  OptionalParameter( "ID_Montage" );
  OptionalParameter( "ID_Amp" );
  OptionalParameter( "ID_System" );
  OptionalParameter( "OperatorVersion" );
  OptionalParameter( "EEGSourceVersion" );
  OptionalParameter( "SignalProcessingVersion" );
  OptionalParameter( "ApplicationVersion" );
  OptionalParameter( "Classifier" );
}

void
ERPSpellerTask::OnInitialize( const SignalProperties& /*Input*/ )
{
  mStimuli.DeleteObjects();
  Speller::DeleteObjects();
  Associations().clear();

  if( mpStatusBar == NULL )
    mpStatusBar = new StatusBar( Display() );
  GUI::Rect statusBarRect =
  { 0, 0, 1.0, Parameter( "StatusBarSize" ) / 100 };
  mpStatusBar->SetTextHeight( Parameter( "StatusBarTextHeight" ) / Parameter( "StatusBarSize" ) )
              .SetColor( RGBColor::Gray )
              .SetTextColor( RGBColor::Yellow )
              .SetObjectRect( statusBarRect );

  ClearTextHistory();

  mCurMenu = static_cast<int>( Parameter( "FirstActiveMenu" ) - 1 );
  while( !mMenuHistory.empty() )
    mMenuHistory.pop();
  mMenuHistory.push( mCurMenu );

  mMatrixRect.left = 0;
  mMatrixRect.top = statusBarRect.bottom;
  mMatrixRect.right = 1.0;
  mMatrixRect.bottom = 1.0;
  LoadMenu(
    mCurMenu,
    mMatrixRect,
    Display(),
    mStimuli,
    Associations(),
    *this
  );
  mNumMatrixRows = MenuRows( mCurMenu );
  mNumMatrixCols = MenuCols( mCurMenu );
  InitSequence();

  mNumberOfSequences = Parameter( "NumberOfSequences" );
  mDisplayResults = ( Parameter( "DisplayResults" ) != 0 );
  mTestMode = ( Parameter( "TestMode" ) != 0 );
  mInterpretMode_ = Parameter( "InterpretMode" );
  switch( mInterpretMode_ )
  {
    case InterpretModes::None:
    case InterpretModes::Free:
      mTextToSpell = "";
      break;
    case InterpretModes::Copy:
      mTextToSpell = ( string )Parameter( "TextToSpell" );
      break;
  }

  mGoalText = mTextToSpell;
  mpStatusBar->SetGoalText( mGoalText );

  delete mpTextWindow;
  mpTextWindow = NULL;
  if( Parameter( "TextWindowEnabled" ) == 1 )
  {
    mpTextWindow = new TextWindow;
    mpTextWindow->SetLeft( Parameter( "TextWindowLeft" ) )
                 .SetTop( Parameter( "TextWindowTop" ) )
                 .SetWidth( Parameter( "TextWindowWidth" ) )
                 .SetHeight( Parameter( "TextWindowHeight" ) )
                 .SetFontName( Parameter( "TextWindowFontName" ) )
                 .SetFontSize( Parameter( "TextWindowFontSize" ) )
                 .Show();
  }

  // UDP connection
  mSocket.Close();
  mConnection.clear();
  string destinationAddress = Parameter( "DestinationAddress" );
  if( destinationAddress != "" )
  {
    mSocket.Open( destinationAddress );
    if( !mSocket.IsOpen() )
      bciout << "Could not open " << destinationAddress << endl;
  }
}


void
ERPSpellerTask::OnStartRun()
{
  // Non-summary file
  Display().ClearClicks();
  if( mInterpretMode_ == InterpretModes::Copy )
    ClearTextHistory();
  InitSequence();
  DetermineAttendedTarget();
  DisplayMessage( LocalizableString( "Waiting to start ..." ) );

  mNumSelections = 0;
  mSleepDuration = 0;
  mSleepMode = dontSleep;
  mPaused = false;
  State( "SpellerMenu" ) = mCurMenu + 1;

  // Summary file
  mSummaryFile << "System ID = "  << OptionalParameter( "ID_System", "N/A" )  << '\t'
               << "Amp ID = "     << OptionalParameter( "ID_Amp", "N/A" )     << '\t'
               << "Montage ID = " << OptionalParameter( "ID_Montage", "N/A" ) << '\n'
               << "\nSW Versions:\n";

  if( Parameters->Exists( "OperatorVersion" ) )
    mSummaryFile << "Operator:\n\t"
                 << Parameter( "OperatorVersion" )( "Revision" )
                 << '\n';
  if( Parameters->Exists( "EEGSourceVersion" ) )
    mSummaryFile << "EEGSource:\n\t"
                 << Parameter( "EEGSourceVersion" )( "Revision" )
                 << '\n';
  if( Parameters->Exists( "SignalProcessingVersion" ) )
    mSummaryFile << "Signal Processing:\n\t"
                 << Parameter( "SignalProcessingVersion" )( "Revision" )
                 << '\n';
  if( Parameters->Exists( "ApplicationVersion" ) )
    mSummaryFile << "Application:\n\t"
                 << Parameter( "ApplicationVersion" )( "Revision" )
                 << '\n';

  mSummaryFile << "\n---------------------------------------------------"
               << endl;

  if( Parameters->Exists( "Classifier" ) )
  {
    mSummaryFile << "Classifier Matrix:\n";
    ParamRef Classifier = Parameter( "Classifier" );
    for( int row = 0; row < Classifier->NumRows(); ++row )
    {
      for( int col = 0; col < Classifier->NumColumns(); ++col )
        mSummaryFile << Classifier( row, col ) << ' ';
      mSummaryFile << '\n';
    }
    mSummaryFile << flush;
  }

  ++mRunCount;
  if( mInterpretMode_ == InterpretModes::Free )
  {
    AppLog << "Start of run " << mRunCount << " in online (free) mode\n";
    mSummaryFile << "*** START OF RUN " << mRunCount << " IN ONLINE MODE ***\n";
  }
  else
  {
    AppLog << "Start of run " << mRunCount << " in offline (copy) mode\n";
    mSummaryFile << "*** START OF RUN " << mRunCount << " IN OFFLINE MODE ***\n";
  }
  AppLog << flush;

  mSummaryFile << "Date = " << StringDate() << "\t\t"
               << "Time = " << StringTime() << "\n"
               << "Num of Sequences = " << mNumberOfSequences
               << "\nMATRIX SIZE(s)\n";

  int numMenus = NumMenus();
  for( int i = 0; i < numMenus; ++i )
    mSummaryFile << MenuRows( i ) << " x "
                 << MenuCols( i ) << '\n';

  mSelectionSummary.str() = "Selections in this run:\n";
}

void
ERPSpellerTask::OnStopRun()
{
  if( mInterpretMode_ == InterpretModes::Free )
    Parameter( "TextResult" ) = mTextHistory.top();

  DisplayMessage( LocalizableString( "TIME OUT !!!" ) );

  // App log
  AppLog << "******************************" << endl;

  // Summary file
  mSummaryFile << "*** RUN SUMMARY ***\n"
               << "System Pause Duration (in seconds): " << mSleepDuration << '\n'
               << "Number of Selections = " << mNumSelections << '\n';
  if( mInterpretMode_ == InterpretModes::Copy ) // in copy spelling
    mSummaryFile << "Expected Copy Spelling Characters: "
                 << mTextToSpell
                 << '\n';
  mSummaryFile << mSelectionSummary.str() << endl;
  mSelectionSummary.clear();
  mSelectionSummary.str( "" );
}

void
ERPSpellerTask::OnPreSequence()
{
  DisplayMessage( "" );
}

void
ERPSpellerTask::DoPreSequence( const GenericSignal&, bool& /*doProgress*/ )
{
  CheckSwitchMenu();
}

void
ERPSpellerTask::OnSequenceBegin()
{
  State( "SelectedRow" ) = 0;
  State( "SelectedColumn" ) = 0;
  State( "SelectedTarget" ) = 0;
}

void
ERPSpellerTask::OnPostRun()
{
  State( "SelectedRow" ) = 0;
  State( "SelectedColumn" ) = 0;
  State( "SelectedTarget" ) = 0;
}

int
ERPSpellerTask::OnNextStimulusCode()
{
  // Return values of this function determine sequencing in the following way:
  //  A zero stimulus code ends the current sequence of stimuli.
  //  A null sequence (no nonzero stimulus codes between two zero codes) ends
  //  the run.
  int result = 0;
  if( !mSequence.empty() )
  {
    if( mSequencePos == mSequence.end() )
    { // During a run, we always use the same sequence object and re-shuffle it.
      //
      // Sequences should fulfil the constraint that no stimulus
      // presented on the previous sequence's last stimulus presentation
      // may be presented on the next sequence's first stimulus presentation
      // (unless this is impossible by the way stimuli are grouped).

      int prevStimulusCode = *mSequence.rbegin();
      do
      {
        random_shuffle( mSequence.begin(), mSequence.end(), RandomNumberGenerator );
      } while( mAvoidStimulusRepetition
               && Associations().StimuliIntersect( *mSequence.begin(), prevStimulusCode ) );

      mSequencePos = mSequence.begin();
      if( ++mSequenceCount == mNumberOfSequences )
      {
        result = 0;
        mSequenceCount = 0;
      }
      else
      {
        result = *mSequencePos++;
      }
    }
    else
    {
      result = *mSequencePos++;
    }
  }
  return result;
}

void
ERPSpellerTask::DoPostSequence( const GenericSignal&, bool& /*doProgress*/ )
{
  CheckSwitchMenu();
}

Target*
ERPSpellerTask::OnClassResult( const ClassResult& inResult )
{
  // We override the standard ClassResult handler
  // - to additionally provide the SelectedTarget, SelectedRow, SelectedColumn
  //   states,
  // - to handle user clicks on visual stimuli,
  // - to log the classification result.

  // Clear the display's queue of clicked objects, and store a pointer to the
  // most recently clicked stimulus.
  Stimulus* pClickedStimulus = NULL;
  GraphObject* pObject = 0;
  while( pObject = Display().ConsumeClick() )
  {
    Stimulus* pStimulus = dynamic_cast<Stimulus*>( pObject );
    if( pStimulus )
      pClickedStimulus = pStimulus;
  }

  Target* pTarget = NULL;
  if( mTestMode && pClickedStimulus != NULL )
  { // Fake an ideal ERP result, i.e. a binary response to the clicked stimulus.
    // This allows for testing result processing as well.
    ClassResult fakeResult;
    GenericSignal fakeSignal( 1, 1 );
    for( AssociationMap::const_iterator i = Associations().begin();
                                        i != Associations().end(); ++i )
    {
      fakeSignal( 0, 0 ) =  i->second.Contains( pClickedStimulus );
      fakeResult[ i->first ].push_back( fakeSignal );
    }
    pTarget = StimulusTask::OnClassResult( fakeResult );
  }
  else
    pTarget = StimulusTask::OnClassResult( inResult );

  // Compute the "Selected*" states from the result.
  // These are for documentation purposes only, and may lose their meaning
  // when targets are not grouped into rows and columns.
  int targetID = pTarget ? pTarget->Tag() : 0;
  State( "SelectedTarget" ) = targetID;
  State( "SelectedRow" )    = targetID ? ( targetID - 1 ) / mNumMatrixCols + 1 : 0;
  State( "SelectedColumn" ) = targetID ? ( targetID - 1 ) % mNumMatrixCols + 1 : 0;

  // Write classification signal details into the application log.
  size_t numAverages = 0;
  for( ClassResult::const_iterator i = inResult.begin(); i != inResult.end(); ++i )
    numAverages += i->second.size();
  AppLog << "This is the end of this sequence: "
         << numAverages * mNumberOfSequences << " total intensifications"
         << endl;
  // Report mean responses to log file but not to screen log.
  AppLog.File << "Mean responses for each stimulus:\n";
  for( ClassResult::const_iterator i = inResult.begin(); i != inResult.end(); ++i )
  {
    float mean = 0.0;
    for( size_t j = 0; j < i->second.size(); ++j )
      mean += i->second[ j ]( 0, 0 );
    mean /= i->second.size();
    AppLog.File << "Response for Stimulus Code " << i->first << ": "
                << setprecision( 2 ) << fixed << mean
                << "\n";
  }
  return pTarget;
}

// Speller events.
void
ERPSpellerTask::OnEnter( const std::string& inText )
{
  AppLog << "Selected command: " << inText << endl;
  mConnection << "P3Speller_Output " << inText << endl;

  istringstream iss( inText );
  SpellerCommand command;
  while( iss >> command )
  { // Interpret input as a sequence of commands interspersed with plain text.
    if( mDisplayResults )
    {
      if( command.Code() == "SLEEP" )
      {
        if( !mPaused )
          OnSleep();
      }
      else if( command.Code() == "PAUSE" )
      {
        if( mSleepMode == dontSleep )
          OnPause();
      }
      else if( !mPaused && mSleepMode == dontSleep )
      {
        if( command.Code() == "" )
          OnText( command.Value() );
        else if( command.Code() == "BS" )
          OnBackspace();
        else if( command.Code() == "DW" )
          OnDeleteWord();
        else if( command.Code() == "UNDO" )
          OnUndo();
        else if( command.Code() == "END" )
          OnEnd();
        else if( command.Code() == "GTO" || command.Code() == "GOTO" )
          OnGoto( ::atoi( command.Value().c_str() ) - 1 );
        else if( command.Code() == "BK"  || command.Code() == "BACK" )
          OnBack();
        else if( command.Code() == "SAVE" )
          OnSave();
        else if( command.Code() == "RETR" )
          OnRetrieve();
        else
          bcierr << "Unknown command: " << command << endl;
      }

      if( mSleepMode == sleep2 && command.Code() != "SLEEP" )
      { // Reset to sleeping.
        mSleepMode = dontSleep;
        OnSleep();
      }
    }
    else if( command.Code() == "" )
    {
      OnText( command.Value() );
    }
  }
  if( mDisplayResults )
  {
    mpStatusBar->SetResultText( mTextHistory.top() );
    if( mpTextWindow != NULL )
      mpTextWindow->SetText( mTextHistory.top() );
  }
  DetermineAttendedTarget();

  mSelectionSummary << inText << ' ';
  if( ++mNumSelections % 10 == 0 )
    mSelectionSummary << '\n';
}

// Event handlers associated with individual speller commands.
void
ERPSpellerTask::OnText( const std::string& inText )
{
  // add plain string input to the spelled text
  mTextHistory.push( mTextHistory.top() + inText );
}

void
ERPSpellerTask::OnBackspace()
{
  if( !mTextHistory.top().empty() )
    mTextHistory.push( mTextHistory.top().substr( 0, mTextHistory.top().length() - 1 ) );
}

void
ERPSpellerTask::OnDeleteWord()
{
  // delete last word and space characters following it
  if( !mTextHistory.top().empty() )
  {
    string curText = mTextHistory.top();
    size_t spacePos = curText.length();
    while( spacePos != 0 && ::isspace( curText[ --spacePos ] ) )
      ;
    while( spacePos != 0 && !::isspace( curText[ --spacePos ] ) )
      ;
    mTextHistory.push( curText.substr( 0, spacePos ) );
  }
}

void
ERPSpellerTask::OnUndo()
{
  // undo the last change
  if( mTextHistory.size() > 1 )
    mTextHistory.pop();
}

void
ERPSpellerTask::OnEnd()
{
  // end of run
  State( "Running" ) = 0;
}

void
ERPSpellerTask::OnSleep()
{
  // stop writing to file, and continue after two additional SLEEP commands
  ++mSleepMode %= unsleepAfter;
  switch( mSleepMode )
  {
    case dontSleep:
      State( "Recording" ) = 1;
      mpStatusBar->SetGoalText( mGoalText );
      mSleepDuration += static_cast<int>( ::difftime( ::time( NULL ), mStartPause ) );
      break;

    case sleep1:
      State( "Recording" ) = 0;
      mpStatusBar->SetGoalText( LocalizableString( "Sleeping--Select SLEEP twice to resume" ) );
      mStartPause = ::time( NULL );
      break;

    case sleep2:
      mpStatusBar->SetGoalText( LocalizableString( "Select SLEEP once more to resume" ) );
      break;
  }
}

void
ERPSpellerTask::OnPause()
{
  // stop writing to file, and continue after one additional PAUSE command
  mPaused = !mPaused;
  if( !mPaused )
  {
    State( "Recording" ) = 1;
    mpStatusBar->SetGoalText( mGoalText );
    mSleepDuration += static_cast<int>( ::difftime( ::time( NULL ), mStartPause ) );
  }
  else
  {
     mpStatusBar->SetGoalText( LocalizableString( "Paused--Select PAUSE again to resume" ) );
     mStartPause = ::time( NULL );
  }
}

void
ERPSpellerTask::OnGoto( int inMenu )
{
  if( inMenu >= 0 )
    State( "SpellerMenu" ) = inMenu + 1;
  else
    bcierr << "Invalid menu number in GOTO command" << endl;
}

void
ERPSpellerTask::OnBack()
{
  if( mMenuHistory.size() > 1 )
  {
    mMenuHistory.pop();
    State( "SpellerMenu" ) = mMenuHistory.top() + 1;
  }
}

void
ERPSpellerTask::OnSave()
{ // Save the text window's content into a file, and save the file name to
  // a directory file.
  if( mpTextWindow != NULL )
  {
    string path = Parameter( "TextWindowFilePath" );
    if( !path.empty() )
    {
      path = FileUtils::AbsolutePath( path );
      path += string( "/Text" ) + TimeStamp() + ".txt";
      ofstream textFile( path.c_str() );
      if( !textFile.is_open() )
        bciout << "Could not open text file for writing - "
               << path
               << endl;
      else
      {
        textFile << mpTextWindow->Text();
        mpTextWindow->SetText( "" );
        mTextHistory.push( "" );
        ofstream dirFile( DirectoryFileName().c_str() );
        dirFile << path;
      }
    }
  }
}


void
ERPSpellerTask::OnRetrieve()
{ // Retrieve the name of the file last saved, and load its content into the
  // text window.
  if( mpTextWindow != NULL )
  {
    string path;
    ifstream dirFile( DirectoryFileName().c_str() );
    getline( dirFile, path, '\0' );
    ifstream textFile( path.c_str() );
    if( !textFile.is_open() )
      AppLog.Screen << "No saved file to retrieve!" << endl;
    else
    {
      string text;
      getline( textFile, text, '\0' );
      if( text.empty() )
        AppLog.Screen << "Retrieved file is empty" << endl;
      else
      {
        mpTextWindow->SetText( text );
        mTextHistory.push( text );
      }
    }
  }
}

// Internally used functions which are not event handlers.
void
ERPSpellerTask::CheckSwitchMenu()
{
  if( State( "SpellerMenu" ) != mCurMenu + 1 )
  { // One of the commands modified the current menu -- we need to load a
    // different menu.
    // We cannot switch menus from the OnEnter() handler because it originates
    // from a target member function, and switching menus will delete target
    // objects.
    mCurMenu = State( "SpellerMenu" ) - 1;
    mMenuHistory.push( mCurMenu );
    LoadMenu(
      mCurMenu,
      mMatrixRect,
      Display(),
      mStimuli,
      Associations(),
      *this
    );
    mNumMatrixRows = MenuRows( mCurMenu );
    mNumMatrixCols = MenuCols( mCurMenu );
    InitSequence();
    DetermineAttendedTarget();
  }
}

void
ERPSpellerTask::DetermineAttendedTarget()
{ // Determine the attended target for copy spelling mode.
  SpellerTarget* pSuggestedTarget = NULL;
  if( mInterpretMode_ == InterpretModes::Copy )
  {
    SequenceOfSpellerTargets currentlySpelled,
                             toBeSpelled;
    Speller::TrySpelling( mTextHistory.top(), &currentlySpelled );
    Speller::TrySpelling( mTextToSpell, &toBeSpelled );
    if( toBeSpelled.size() > currentlySpelled.size() )
      pSuggestedTarget = toBeSpelled[ currentlySpelled.size() ];

    if( pSuggestedTarget == NULL )
    {
      mGoalText = mTextToSpell;
      ClearSequence();
    }
    else
    {
      mGoalText = mTextToSpell + " (";
      SetOfStimuli targetStimuli = Associations().TargetToStimuli( pSuggestedTarget );
      set<string> captions;
      for( SetOfStimuli::const_iterator i = targetStimuli.begin(); i != targetStimuli.end(); ++i )
      {
        TextGroupStimulus* p = dynamic_cast<TextGroupStimulus*>( *i );
        if( p != NULL )
          captions.insert( p->Text() );
      }
      for( set<string>::const_iterator i = captions.begin(); i != captions.end(); ++i )
        mGoalText += *i;
      mGoalText += ")";
    }
    mpStatusBar->SetGoalText( mGoalText );
  }
  SetAttendedTarget( pSuggestedTarget );
}


void
ERPSpellerTask::InitSequence()
{
  AssociationMap::iterator i = Associations().find( 0 );
  if( i != Associations().end() )
    Associations().erase( i );

  mSequence.clear();
  for( AssociationMap::const_iterator i = Associations().begin();
                                      i != Associations().end(); ++i )
    mSequence.push_back( i->first );

  // Check whether it is possible to construct stimulus code sequences that avoid
  // stimulus repetitions.
  // If any stimulus code intersects with all other stimulus codes, avoiding
  // repetitions is impossible.
  mAvoidStimulusRepetition = true;
  for( size_t i = 0; mAvoidStimulusRepetition && i < mSequence.size(); ++i )
  {
    bool fullRowIntersects = true;
    for( size_t j = 0; fullRowIntersects && j < mSequence.size(); ++j )
      fullRowIntersects &= Associations().StimuliIntersect( mSequence[ i ], mSequence[ j ] );
    mAvoidStimulusRepetition &= !fullRowIntersects;
  }
  random_shuffle( mSequence.begin(), mSequence.end(), RandomNumberGenerator );
  mSequencePos = mSequence.begin();
  mSequenceCount = 0;
}

void
ERPSpellerTask::ClearSequence()
{
  mSequence.clear();
  mSequencePos = mSequence.begin();
  mSequenceCount = 0;
}

void
ERPSpellerTask::ClearTextHistory()
{
  while( !mTextHistory.empty() )
    mTextHistory.pop();
  mTextHistory.push( Parameter( "TextResult" ) );
  mpStatusBar->SetResultText( mTextHistory.top() );
}


void
ERPSpellerTask::LoadMenu( int                inMenuIdx,
                         GUI::Rect&         ioRect,
                         GUI::GraphDisplay& ioDisplay,
                         SetOfStimuli&      ioStimuli,
                         AssociationMap&    ioAssociations,
                         Speller&           ioSpeller ) const
{
  enum ColNames
  {
    Display = 0,
    Enter,
    DisplaySize,
    IconFile,
    SoundFile,
  };
  // Clear previously existing stimuli, speller targets, and stimulus-target associations.
  ioStimuli.DeleteObjects();
  ioSpeller.DeleteObjects();
  ioAssociations.clear();

  if( inMenuIdx < 0 || inMenuIdx >= NumMenus() )
  {
    bcierr << "Menu index specifies non-existent menu" << endl;
    return;
  }

  float targetWidth = MenuParam( "TargetWidth", inMenuIdx ) / 100.0,
        targetHeight = MenuParam( "TargetHeight", inMenuIdx ) / 100.0,
        targetTextHeight = MenuParam( "TargetTextHeight", inMenuIdx ) / 100.0 / targetHeight;
  RGBColor targetColor = RGBColor( MenuParam( "BackgroundColor", inMenuIdx ) ),
           textColor = RGBColor( MenuParam( "TextColor", inMenuIdx ) ),
           textColorIntensified = RGBColor( MenuParam( "TextColorIntensified", inMenuIdx ) );
  int   iconHighlightMode = MenuParam( "IconHighlightMode", inMenuIdx );
  float iconHighlightFactor = MenuParam( "IconHighlightFactor", inMenuIdx );
  int numMenus = NumMenus(),
      numMatrixRows = MenuRows( inMenuIdx ),
      numMatrixCols = MenuCols( inMenuIdx );

  ParamRef TargetDefinitions = MultipleMenus() ?
                                Parameter( "TargetDefinitions" )( inMenuIdx, 0 ) :
                                Parameter( "TargetDefinitions" );
  if( TargetDefinitions->NumColumns() <= Enter )
  {
    bcierr << "Number of TargetDefinition columns in menu #" << inMenuIdx + 1
           << " must be " << Enter + 1 << " or greater"
           << endl;
    return;
  }
  // Compute the enclosing rectangle's dimensions.
  // The speller matrix will be centered vertically and horizontally in the
  // rectangle as specified when LoadMenu() is called.
  float inputRectWidth = ioRect.right - ioRect.left,
        inputRectHeight = ioRect.bottom - ioRect.top,
        outputRectHeight = numMatrixRows * targetHeight,
        outputRectWidth = numMatrixCols * targetWidth;
  ioRect.left += ( inputRectWidth - outputRectWidth ) / 2;
  ioRect.top += ( inputRectHeight - outputRectHeight ) / 2;
  ioRect.right = ioRect.left + outputRectWidth;
  ioRect.bottom = ioRect.top + outputRectHeight;

  // For each target definition, create stimuli and speller targets.
  int numElements = TargetDefinitions->NumRows();
  if( numElements != numMatrixRows * numMatrixCols )
    bcierr << "Number of elements in menu #" << inMenuIdx + 1
           << " does not match matrix rows and columns as defined in "
           << "NumMatrixRows and NumMatrixColumns parameters"
           << endl;
  for( int i = 0; i < numElements; ++i )
  {
    int targetCol = i % numMatrixCols,
        targetRow = i / numMatrixCols;
    Association dummy;
    Association& rowSet = numMatrixRows > 1 ? ioAssociations[ targetRow + 1 ] : dummy,
               & colSet = numMatrixCols > 1 ? ioAssociations[ numMatrixRows + targetCol + 1 ] : dummy;

    string entryText = TargetDefinitions( i, Enter );
    { // Check for legal entry text
      istringstream iss( entryText );
      SpellerCommand command;
      while( iss >> command )
        if( ( command.Code() == "GOTO" || command.Code() == "GTO" )
            && ::atoi( command.Value().c_str() ) > numMenus )
          bcierr << "GOTO target index in Menu "
                 << inMenuIdx + 1 << ", entry "
                 << i << ", exceeds number of available menus ("
                 << numMenus << ")."
                 << endl;
    }

    AudioSpellerTarget* pTarget = new AudioSpellerTarget( ioSpeller );
    pTarget->SetEntryText( entryText )
            .SetTag( i + 1 );
    if( TargetDefinitions->NumColumns() > SoundFile )
      pTarget->SetSound( TargetDefinitions( i, SoundFile ) );
    rowSet.Add( pTarget );
    colSet.Add( pTarget );

    TextGroupStimulus* pTextGroupStimulus = new TextGroupStimulus( ioDisplay );
    GUI::Rect targetRect =
    {
      ioRect.left + targetCol * targetWidth,
      ioRect.top  + targetRow * targetHeight,
      ioRect.left + ( targetCol + 1 ) * targetWidth,
      ioRect.top  + ( targetRow + 1 ) * targetHeight
    };
    float displaySize = 1.0;
    if( TargetDefinitions->NumColumns() > DisplaySize )
      displaySize = TargetDefinitions( i, DisplaySize );
    pTextGroupStimulus->SetText( TargetDefinitions( i, Display ) )
                  .SetTextHeight( targetTextHeight * displaySize )
                  .SetTextColor( textColor )
                  .SetColor( RGBColor::NullColor )
                  .SetObjectRect( targetRect );
    pTextGroupStimulus->SetIntensifiedColor( textColorIntensified )
                  .SetPresentationMode( VisualStimulus::Intensify );
    ioStimuli.Add( pTextGroupStimulus );
    rowSet.Add( pTextGroupStimulus );
    colSet.Add( pTextGroupStimulus );

    if( TargetDefinitions->NumColumns() > IconFile )
    {
      string iconFile = TargetDefinitions( i, IconFile );
      if( !iconFile.empty() )
      {
        ImageStimulus* pIcon = new ImageStimulus( ioDisplay );
        pIcon->SetFile( iconFile )
              .SetRenderingMode( GUI::RenderingMode::Transparent )
              .SetObjectRect( targetRect );
        pIcon->SetPresentationMode( VisualStimulus::Mode( iconHighlightMode ) )
              .SetDimFactor( 1.0 / iconHighlightFactor );
        ioStimuli.insert( pIcon );
        rowSet.Add( pIcon );
        colSet.Add( pIcon );
      }
    }
  }
  // Create audio stimuli for rows and columns.
  if( Parameter( "AudioStimuliOn" ) != 0 )
  {
    ParamRef AudioStimuliRowsFiles = Parameter( "AudioStimuliRowsFiles" );
    int entry = 0;
    if( AudioStimuliRowsFiles->NumColumns() > inMenuIdx )
      entry = inMenuIdx;
    for( int i = 0; i < AudioStimuliRowsFiles->NumRows(); ++i )
    {
      int row = ::atoi( AudioStimuliRowsFiles->RowLabels()[ i ].c_str() );
      AudioStimulus* pStimulus = new AudioStimulus;
      pStimulus->SetSound( AudioStimuliRowsFiles( i, entry ) );
      if( !pStimulus->Error().empty() )
        bcierr << "AudioStimulusRowsFiles(" << i << ", " << entry << "): "
               << pStimulus->Error()
               << endl;
      ioStimuli.Add( pStimulus );
      ioAssociations[ row ].Add( pStimulus );
    }
    ParamRef AudioStimuliColsFiles = Parameter( "AudioStimuliColsFiles" );
    entry = 0;
    if( AudioStimuliColsFiles->NumColumns() > inMenuIdx )
      entry = inMenuIdx;
    for( int i = 0; i < AudioStimuliColsFiles->NumRows(); ++i )
    {
      int col = ::atoi( AudioStimuliColsFiles->RowLabels()[ i ].c_str() );
      AudioStimulus* pStimulus = new AudioStimulus;
      pStimulus->SetSound( AudioStimuliColsFiles( i, entry ) );
      if( !pStimulus->Error().empty() )
        bcierr << "AudioStimulusColsFiles(" << i << ", " << entry << "): "
               << pStimulus->Error()
               << endl;
      ioStimuli.Add( pStimulus );
      ioAssociations[ numMatrixRows + col ].Add( pStimulus );
    }
  }
}

bool
ERPSpellerTask::MultipleMenus() const
{
  return Parameter( "TargetDefinitions" )->Value( 0, 0 ).Kind()
           == Param::ParamValue::Matrix;
}

int
ERPSpellerTask::NumMenus() const
{
  int numMenus = 1;
  if( MultipleMenus() )
    numMenus = Parameter( "TargetDefinitions" )->NumRows();
  return numMenus;
}

int
ERPSpellerTask::MenuRows( int inMenuIdx ) const
{
  int menuRows = Parameter( "NumMatrixRows" );
  if( MultipleMenus() && Parameter( "NumMatrixRows" )->NumValues() > inMenuIdx )
    menuRows = Parameter( "NumMatrixRows" )( inMenuIdx );
  return menuRows;
}

int
ERPSpellerTask::MenuCols( int inMenuIdx ) const
{
  int menuCols = Parameter( "NumMatrixColumns" );
  if( MultipleMenus() && Parameter( "NumMatrixColumns" )->NumValues() > inMenuIdx )
    menuCols = Parameter( "NumMatrixColumns" )( inMenuIdx );
  return menuCols;
}

ParamRef
ERPSpellerTask::MenuParam( const std::string& inParamName, int inMenuIdx ) const
{
  if( inMenuIdx >= Parameter( inParamName )->NumValues() )
    return Parameter( inParamName );
  return Parameter( inParamName )( inMenuIdx );
}

string
ERPSpellerTask::StringTime()
{
  string result;
  time_t now;
  const int bufLen = 20;
  char timeBuf[ bufLen + 1 ];
  ::tzset();
  ::time( &now );
  if( ::strftime( timeBuf, bufLen, "%H:%M", ::localtime( &now ) ) )
    result = timeBuf;
  return result;
}

string
ERPSpellerTask::StringDate()
{
  string result;
  time_t now;
  const int bufLen = 20;
  char timeBuf[ bufLen + 1 ];
  ::tzset();
  ::time( &now );
  if( ::strftime( timeBuf, bufLen, "%b/%d/%y", ::localtime( &now ) ) )
    result = timeBuf;
  return result;
}

string
ERPSpellerTask::TimeStamp()
{
  string result;
  time_t now;
  const int bufLen = 20;
  char timeBuf[ bufLen + 1 ];
  ::tzset();
  ::time( &now );
  if( ::strftime( timeBuf, bufLen, "%b%d%y_%H%M", ::localtime( &now ) ) )
    result = timeBuf;
  return result;
}

string
ERPSpellerTask::SummaryFileExtension()
{
  return string( "_" ) + TimeStamp() + "_summary.txt";
}

string
ERPSpellerTask::DirectoryFileName() const
{
  string result = Parameter( "TextWindowFilePath" );
  if( !result.empty() )
    result = FileUtils::AbsolutePath( result ) + "/LastFile.txt";
  return result;
}
