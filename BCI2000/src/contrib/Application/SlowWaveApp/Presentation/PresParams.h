/////////////////////////////////////////////////////////////////////////////
//
// File: PresParams.h
//
// Date: Oct 17, 2001
//
// Author: Juergen Mellinger
//
// Description: Contains all parameter declarations for the presentation
//      module.
//
// Changes: Feb 1, 2004, jm: Various adaptations to using the presentation
//      classes within a BCI2000 application (#ifdef BCI2000).
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

#ifndef PRESPARAMSH
#define PRESPARAMSH

#include "ParamDecl.h"
#include "Utils/Util.h"

#define TIME_MAX                    67108864 // upper bound for time-valued parameters in s
#define TIME_MAX_STR                "67108864"

#define NUM_TARGET_BITS             3
#define SUPPRESS_CURSOR_MASK        (1<<NUM_TARGET_BITS)
#define MAX_NUM_TARGETS             (SUPPRESS_CURSOR_MASK-1)
#define MAX_NUM_TARGETS_STR         "7" // the string value of MAX_NUM_TARGETS
#define SUPPRESS_CURSOR_BIT( X )    (((X)&SUPPRESS_CURSOR_MASK)!=0)
#define TRUE_TARGET_CODE( X )       ((X)&(~SUPPRESS_CURSOR_MASK))

// "External" parameters declared here for easy access.

PARAM_DECL(
    SamplingRate,
    int,
    ext_Source,
    "%",
    "",
    1, 1024
)

PARAM_DECL(
    SampleBlockSize,
    int,
    ext_Source,
    "%",
    "",
    1, 10000
)

#ifndef BCI2000
PARAM_DECL(
    ResourceDirectory,
    string,
    ext_Storage,
    "Resources Resources a z",
    "Resource directory relative to application module (directory)",
    0, 0
)

PARAM_DECL(
    FBBegin,
    float,
    ext_Sequencer,
    "2.0 2.0 0 " TIME_MAX_STR,
    "Begin of cursor movement in s",
    0, TIME_MAX
)

PARAM_DECL(
    FBEnd,
    float,
    ext_Sequencer,
    "5.5 5.5 0 " TIME_MAX_STR,
    "End of cursor movement in s",
    0, TIME_MAX
)

PARAM_DECL(
    TaskBegin,
    float,
    ext_Sequencer,
    "0.0 0.0 0.0 " TIME_MAX_STR,
    "Task/stimulus presentation time",
    0, TIME_MAX
)
#endif // BCI2000

#ifndef BCI2000
PARAM_DECL(
    NumFBChannels,
    int,
    ext_Decider,
    "1 1 0 2",
    "number of FB channels",
    0, 2
)
#endif // BCI2000

#ifdef BCI2000
PARAM_DECL(
    NumberTargets,
    int,
    UsrTask,
    "2 2 0 " MAX_NUM_TARGETS_STR,
    "number of target codes",
    0, MAX_NUM_TARGETS
)
#else // BCI2000
PARAM_DECL(
    NumberOfTargets,
    int,
    ext_Decider,
    "4 4 0 " MAX_NUM_TARGETS_STR,
    "number of target codes",
    0, MAX_NUM_TARGETS
)
#endif // BCI2000

#ifdef BCI2000
#define FEEDBACK_SECTION Feedback
PARAM_DECL(
    FBCh0L,
    floatlist,
    FEEDBACK_SECTION,
    "2 -101 -101 0 -101 101",
    "Lower target bounds for feedback channel 0",
    -101, 101
)
PARAM_DECL(
    FBCh0U,
    floatlist,
    FEEDBACK_SECTION,
    "2 101 101 0 -101 101",
    "Upper target bounds for feedback channel 0",
    -101, 101
)
PARAM_DECL(
    FBCh1L,
    floatlist,
    FEEDBACK_SECTION,
    "2 -101 5 0 -101 101",
    "Lower target bounds for feedback channel 1",
    -101, 101
)
PARAM_DECL(
    FBCh1U,
    floatlist,
    FEEDBACK_SECTION,
    "2 -5 101 0 -101 101",
    "Upper target bounds for feedback channel 1",
    -101, 101
)
#endif // BCI2000

PARAM_DECL(
    FBCh__RS__L,
    floatlist,
    ext_Decider,
    "4 -101 -101 -101 -101 0 -101 101",
    "Lower target bounds for feedback channel " RUNTIME_SUFFIX,
    -101, 101
)
PARAM_DECL(
    FBCh__RS__U,
    floatlist,
    ext_Decider,
    "4 101 101 101 101 0 -101 101",
    "Upper target bounds for feedback channel " RUNTIME_SUFFIX,
    -101, 101
)

// Parameters on the presentation level

#ifdef BCI2000
# define PRES_SECTION   Feedback
#else
# define PRES_SECTION   Presentation
#endif

#ifndef BCI2000
PARAM_DECL(
    PRPresType,
    int,
    PRES_SECTION,
    "1 1 0 2",
    "0 no presentation, "
        "1 Training/Oddball/WordPresentation, "
        "2 Speller/CopySpeller"
        "(enumeration)",
    0, 2
)
#endif // BCI2000

PARAM_DECL(
    PRFBCoordChannels,
    intlist,
    PRES_SECTION,
    "2 0 1 0 -1 100",
    "signal channels for cursor x and y coordinates (-1 for none, 0 is timebase)",
    -1, 100
)

PARAM_DECL(
    PRFBBrightnessChannel,
    int,
    PRES_SECTION,
    "-1 -1 -1 100",
    "signal channel for cursor brightness (-1 for none)",
    -1, 100
)

PARAM_DECL(
    PRViewStyle,
    string,
    PRES_SECTION,
    "marine marine a z",
    "style name for feedback window colors",
    0, 0
)

#ifndef BCI2000
PARAM_DECL(
    PROldStyle,
    int,
    PRES_SECTION,
    "0 0 0 1",
    "use old colors and goal sizes for target views (0 no, 1 yes) (boolean)",
    0, 1
)
#endif // BCI2000

#ifndef BCI2000
// Without a task, any result will be "correct".
PARAM_DECL(
    PRCorrectResultOnly,
    int,
    PRES_SECTION,
    "0 0 0 1",
    "give feedback and show result for correct result only (0 no, 1 yes) (boolean)",
    0, 1
)
#endif // BCI2000

// Parameters on the model level

// TaskModel parameters

#ifdef BCI2000
# define TASK_MODEL_SECTION UsrTask
#else
# define TASK_MODEL_SECTION Presentation
#endif

#ifndef BCI2000_SPELLER
PARAM_DECL(
    PRSequenceType,
    int,
    TASK_MODEL_SECTION,
    "3 3 0 3",
    "0 no task, "
        "1 sequence file, "
        "2 random, "
        "3 explicit"
        "(enumeration)",
    0, 3
)

#ifdef BCI2000
# define DEFAULT_SEQUENCE "16 1 2 1 2 2 1 2 2 1 1 1 2 1 1 2 2 0 0"
#else
# define DEFAULT_SEQUENCE "16 1 4 1 4 4 1 4 4 1 1 1 4 1 1 4 4 0 0"
#endif
PARAM_DECL(
    PRTaskSequence,
    intlist,
    TASK_MODEL_SECTION,
    DEFAULT_SEQUENCE " " MAX_NUM_TARGETS_STR,
    "explicit task sequence",
    0, MAX_NUM_TARGETS
)

#ifdef BCI2000
PARAM_DECL(
    PRTargetProbabilities,
    floatlist,
    TASK_MODEL_SECTION,
    "2 0.5 0.5 0 0 0",
    "probabilities for target codes",
    0, 1.0
)
#else
PARAM_DECL(
    PRTargetProbabilities,
    floatlist,
    TASK_MODEL_SECTION,
    RUNTIME_ELEMENT,
    "probabilities for target codes",
    0, 1.0
)
#endif

PARAM_DECL(
    PRSequenceFile,
    string,
    TASK_MODEL_SECTION,
    "default.seq default.seq a z",
    "file name of sequence file(inputfile)",
    0, 0
)
#endif // BCI2000_SPELLER

// SpellerModel parameters
#ifdef BCI2000
# define SPELLER_MODEL_SECTION Speller
#else
# define SPELLER_MODEL_SECTION Presentation
#endif

#if( !defined( BCI2000 ) || defined( BCI2000_SPELLER ) )
PARAM_DECL(
    PRSpellerMode,
    int,
    SPELLER_MODEL_SECTION,
    "1 1 0 2",
    "0 no speller, "
        "1 free spelling, "
        "2 copy spelling"
        "(enumeration)",
    0, 2
)

PARAM_DECL(
    PRTreeFile,
    string,
    SPELLER_MODEL_SECTION,
    "config\\default.spt % % %",
    "file name for tree (inputfile)",
    0, 0
)

PARAM_DECL(
    PRDictionaryFile,
    string,
    SPELLER_MODEL_SECTION,
    "config\\default.dic % % %",
    "file name for dictionary (inputfile)",
    0, 0
)

PARAM_DECL(
    PRCurrentDocument,
    string,
    SPELLER_MODEL_SECTION,
    "current.txt % % %",
    "file name for document being worked on (outputfile)",
    0, 0
)

PARAM_DECL(
    PRFeedbackRect,
    floatlist,
    SPELLER_MODEL_SECTION,
    "4 -40 -100 100 100 0 -100 100",
    "rectangle for feedback view",
    -100, 100
)

PARAM_DECL(
    PRDocumentRect,
    floatlist,
    SPELLER_MODEL_SECTION,
    "4 -100 -100 -40 0 0 -100 100",
    "rectangle for document view",
    -100, 100
)

PARAM_DECL(
    PRTextEntryRect,
    floatlist,
    SPELLER_MODEL_SECTION,
    "4 -100 0 -40 100 0 -100 100",
    "rectangle for text entry view",
    -100, 100
)

#if 0
PARAM_DECL(
    PRChoiceRect,
    floatlist,
    SPELLER_MODEL_SECTION,
    "4 -100 0 100 100 0 -100 100",
    "rectangle for choice view",
    -100, 100
)
#endif

PARAM_DECL(
    PRDocumentFont,
    string,
    SPELLER_MODEL_SECTION,
    "Arial_6 Arial_6 a z",
    "text properties for document view",
    0, 0
)

PARAM_DECL(
    PRTextEntryFont,
    string,
    SPELLER_MODEL_SECTION,
    "Arial_6_bold Arial_6_bold a z",
    "text properties for text entry view",
    0, 0
)

#if 0
PARAM_DECL(
    PRChoiceFont,
    string,
    SPELLER_MODEL_SECTION,
    "Arial_6_center Arial_6_center a z",
    "font properties for choice view",
    0, 0
)
#endif

PARAM_DECL(
    PRTextToCopy,
    string,
    SPELLER_MODEL_SECTION,
    "ICH_SCHREIBE_TEXT ICH_SCHREIBE_TEXT a z",
    "text to be copied",
    0, 0
)
#endif // ( !defined( BCI2000 ) || defined( BCI2000_SPELLER ) )

// Parameters on the views level

// Stimulus view parameters

#ifdef BCI2000
# define STIMULUS_VIEW_SECTION Views
#else
# define STIMULUS_VIEW_SECTION StimulusView
#endif

PARAM_DECL(
    PRVisStimDuration,
    int,
    STIMULUS_VIEW_SECTION,
    "1000 1000 0 " TIME_MAX_STR,
    "duration of visual stimuli in ms",
    0, TIME_MAX
)

// Feedback view parameters

#ifdef BCI2000
# define FEEDBACK_VIEW_SECTION Feedback
#else
# define FEEDBACK_VIEW_SECTION FeedbackView
#endif

#ifdef PERUVIAN_BRIGHTNESS_HACK
PARAM_DECL(
    PRVisFBMode,
    int,
    FEEDBACK_VIEW_SECTION,
    "1 1 0 3",
    "0 no FB, "
        "1 cursor from file, "
        "2 ball, "
        "3 brightness"
        "(enumeration)",
    0, 3
)
#else
PARAM_DECL(
    PRVisFBMode,
    int,
    FEEDBACK_VIEW_SECTION,
    "2 2 0 2",
    "0 no FB, "
        "1 cursor from file, "
        "2 ball"
        "(enumeration)",
    0, 2
)
#endif // PERUVIAN_BRIGHTNESS_HACK

#ifdef USE_WAVE_SYNTH
PARAM_DECL(
    PRAudFBMode,
    int,
    FEEDBACK_VIEW_SECTION,
    "1 1 0 2",
    "0 no acoustic FB, "
        "1 MIDI, "
        "2 continuous synth"
        "(enumeration)",
    0, 2
)
#else // USE_WAVE_SYNTH
PARAM_DECL(
    PRAudFBMode,
    int,
    FEEDBACK_VIEW_SECTION,
    "1 1 0 1",
    "0 no acoustic FB, "
        "1 MIDI"
        "(enumeration)",
    0, 1
)
#endif // USE_WAVE_SYNTH

PARAM_DECL(
    PRFBCursorImg,
    string,
    FEEDBACK_VIEW_SECTION,
    "Ball.bmp % % %",
    "name of cursor image file (inputfile)",
    0, 0
)

PARAM_DECL(
    PRFBAlwaysOn,
    int,
    FEEDBACK_VIEW_SECTION,
    "0 0 0 1",
    "0 no feedback display outside feedback interval, "
        "1 always show feedback cursor/play feedback sound"
        "(enumeration)",
    0, 1
)

PARAM_DECL(
    PRGMFBInterval,
    float,
    FEEDBACK_VIEW_SECTION,
    "3 3 0.1 6",
    "acoustic FB range in octaves",
    0, 6
)

PARAM_DECL(
    PRGMFBScale,
    int,
    FEEDBACK_VIEW_SECTION,
    "1 1 0 6",
    "0 no scale, "
        "1 chromatic, "
        "2 major, "
        "3 melodic minor, "
        "4 harmonic minor, "
        "5 pentatonic, "
        "6 whole tone"
        "(enumeration)",
    0, 6
)

PARAM_DECL(
    PRGMFBInstrument,
    int,
    FEEDBACK_VIEW_SECTION,
    "1 1 1 128",
    "GM instrument for acoustic FB",
    0, 128
)

PARAM_DECL(
    PRGMFBVolume,
    int,
    FEEDBACK_VIEW_SECTION,
    "70 70 0 128",
    "MIDI volume for acoustic FB",
    0, 128
)

PARAM_DECL(
    PRGMFBCenterNote,
    int,
    FEEDBACK_VIEW_SECTION,
    "69 69 0 128",
    "MIDI note at zero position for acoustic FB (69: 440Hz a')",
    0, 128
)

// Target view parameters

#ifdef BCI2000
# define TARGET_VIEW_SECTION Targets
#else
# define TARGET_VIEW_SECTION TargetViews
#endif

// global parameters for all target views
PARAM_DECL(
    PRContinuousAudResult,
    int,
    TARGET_VIEW_SECTION,
    "0 0 0 1",
    "repeat file C<result.wav> continuously during feedback: "
        "0: no, "
        "1: yes"
        "(boolean)",
    0, 1
)

// parameters for each target view
PARAM_DECL_SUFFIX(
    PRVisTaskMode,
    int,
    TARGET_VIEW_SECTION,
    "1 1 0 1",
    "target " RUNTIME_SUFFIX ": "
        "0 no visual task representation, "
        "1 light goal"
        "(enumeration)",
    0, 1
)

// Special case for targetCode == 0.
// NOTE: Special cases must go after the general one.
PARAM_DECL(
    PRVisTaskMode0,
    int,
    TARGET_VIEW_SECTION,
    "0 0 0 1",
    "no target: "
        "0 no visual task representation, "
        "1 light background"
        "(enumeration)",
    0, 1
)

PARAM_DECL_SUFFIX(
    PRVisResultMode,
    int,
    TARGET_VIEW_SECTION,
    "1 1 0 1",
    "target " RUNTIME_SUFFIX ": "
        "0 no visual result, "
        "1 blink goal"
        "(enumeration)",
    0, 1
)

// Special case for targetCode == 0.
// NOTE: Special cases must go after the general one.
PARAM_DECL(
    PRVisResultMode0,
    int,
    TARGET_VIEW_SECTION,
    "0 0 0 1",
    "no target: "
        "0 no visual result, "
        "1 blink background"
        "(enumeration)",
    0, 1
)

PARAM_DECL_SUFFIX(
    PRAudTaskMode,
    int,
    TARGET_VIEW_SECTION,
    "0 0 0 2",
    "target " RUNTIME_SUFFIX ": "
        "0 no auditory task, "
        "1 MIDI, "
        "2 WAV"
        "(enumeration)",
    0, 2
)

// Special case for targetCode == 0.
// NOTE: Special cases must go after the general one.
PARAM_DECL(
    PRAudTaskMode0,
    int,
    TARGET_VIEW_SECTION,
    "0 0 0 2",
    "no target: "
        "0 no auditory task, "
        "1 MIDI, "
        "2 WAV"
        "(enumeration)",
    0, 2
)

PARAM_DECL_SUFFIX(
    PRAudResultMode,
    int,
    TARGET_VIEW_SECTION,
    "0 0 0 2",
    "target " RUNTIME_SUFFIX ": "
        "0 no auditory result, "
        "1 MIDI, "
        "2 WAV"
        "(enumeration)",
    0, 2
)

// Special case for targetCode == 0.
// NOTE: Special cases must go after the general one.
PARAM_DECL(
    PRAudResultMode0,
    int,
    TARGET_VIEW_SECTION,
    "0 0 0 2",
    "no target: "
        "0 no auditory result, "
        "1 MIDI, "
        "2 WAV"
        "(enumeration)",
    0, 2
)

PARAM_DECL_SUFFIX(
    PRGMTaskInstrument,
    int,
    TARGET_VIEW_SECTION,
    "1 1 1 128",
    "GM instrument for target " RUNTIME_SUFFIX,
    0, 128
)

PARAM_DECL_SUFFIX(
    PRGMTaskVolume,
    int,
    TARGET_VIEW_SECTION,
    "70 70 0 128",
    "MIDI volume for target " RUNTIME_SUFFIX,
    0, 128
)

PARAM_DECL_SUFFIX(
    PRGMTaskNote,
    int,
    TARGET_VIEW_SECTION,
    "60 60 0 128",
    "MIDI note for target " RUNTIME_SUFFIX,
    0, 128
)

PARAM_DECL_SUFFIX(
    PRTaskSound,
    string,
    TARGET_VIEW_SECTION,
    "sounds\\target" RUNTIME_SUFFIX ".wav % % %",
    "WAV file for target " RUNTIME_SUFFIX,
    0, 0
)

PARAM_DECL_SUFFIX(
    PRGMResultInstrument,
    int,
    TARGET_VIEW_SECTION,
    "1 1 1 128",
    "GM instrument for target " RUNTIME_SUFFIX,
    0, 128
)

PARAM_DECL_SUFFIX(
    PRGMResultVolume,
    int,
    TARGET_VIEW_SECTION,
    "70 70 0 128",
    "MIDI volume for target " RUNTIME_SUFFIX,
    0, 128
)

PARAM_DECL_SUFFIX(
    PRGMResultNote,
    int,
    TARGET_VIEW_SECTION,
    "60 60 0 128",
    "MIDI note for target " RUNTIME_SUFFIX,
    0, 128
)

PARAM_DECL_SUFFIX(
    PRResultSound,
    string,
    TARGET_VIEW_SECTION,
    "sounds\\result" RUNTIME_SUFFIX ".wav % % %",
    "WAV file for result " RUNTIME_SUFFIX,
    0, 0
)

PARAM_DECL_SUFFIX(
    PRTargetFont,
    string,
    TARGET_VIEW_SECTION,
    "CourierNew_10_bold_center_vcenter CourierNew_10_bold_center_vcenter a z",
    "text properties for target " RUNTIME_SUFFIX,
    0, 0
)

// Marker view parameters

#ifdef BCI2000
# define MARKER_VIEW_SECTION Views
#else
# define MARKER_VIEW_SECTION MarkerView
#endif

PARAM_DECL(
    PRVisMarker,
    int,
    MARKER_VIEW_SECTION,
    "1 1 0 2",
    "0 no visual marker, "
        "1: Zero Bar, "
        "2: Fixation Cross"
        "(enumeration)",
    0, 2
)

#ifdef BCI2000
PARAM_DECL(
    PRAudMarkers,
    floatlist,
    MARKER_VIEW_SECTION,
    "2 TaskBegin FeedbackBegin 0 0 0",
    "Time offsets for auditory markers (values or parameter names)",
    0, 0
)

#else
PARAM_DECL(
    PRAudMarkers,
    floatlist,
    MARKER_VIEW_SECTION,
    "2 TaskBegin FBBegin 0 0 0",
    "Time offsets for auditory markers (values or parameter names)",
    0, 0
)
#endif // BCI2000

PARAM_DECL(
    PRAudMarkerSoundFiles,
    list,
    MARKER_VIEW_SECTION,
    "2 sounds\\tick.wav sounds\\tack.wav % % %",
    "Audio files for auditory markers",
    0, 0
)

PARAM_DECL(
    PRGMMarkerInstruments,
    intlist,
    MARKER_VIEW_SECTION,
    "2 0 0 14 1 128",
    "GM instruments for auditory markers (0 for off)",
    0, 128
)

PARAM_DECL(
    PRGMMarkerVolumes,
    intlist,
    MARKER_VIEW_SECTION,
    "2 100 100 100 0 128",
    "MIDI volume for auditory markers",
    0, 128
)

PARAM_DECL(
    PRGMMarkerNotes,
    intlist,
    MARKER_VIEW_SECTION,
    "2 88 72 88 0 128",
    "MIDI notes for auditory markers",
    0, 128
)

// Reinforcement view parameters

#ifdef BCI2000
# define REINFORCEMENT_VIEW_SECTION Views
#else
# define REINFORCEMENT_VIEW_SECTION ReinforcementView
#endif

PARAM_DECL(
    PRVisReinforcement,
    int,
    REINFORCEMENT_VIEW_SECTION,
    "1 1 0 1",
    "0 no visual reinforcement, "
        "1 smiley"
        "(enumeration)",
    0, 1
)

PARAM_DECL(
    PRAudReinforcement,
    int,
    REINFORCEMENT_VIEW_SECTION,
    "1 1 0 2",
    "0 no auditory reinforcement, "
        "1 MIDI, "
        "2 WAV"
        "(enumeration)",
    0, 2
)

PARAM_DECL(
    PRGMRIInstrument,
    int,
    REINFORCEMENT_VIEW_SECTION,
    "10 10 1 128",
    "GM instrument for acoustic reinforcement",
    0, 128
)

PARAM_DECL(
    PRGMRIVolume,
    int,
    REINFORCEMENT_VIEW_SECTION,
    "70 70 0 128",
    "MIDI volume for acoustic reinforcement",
    0, 128
)

PARAM_DECL(
    PRRISoundFile,
    string,
    REINFORCEMENT_VIEW_SECTION,
    "sounds\\welldone.wav % % %",
    "sound file for acoustic reinforcement (inputfile)",
    0, 0
)

PARAM_DECL(
    PRRIFrequency,
    intlist,
    REINFORCEMENT_VIEW_SECTION,
    "2 1 1 1 1 10000",
    "reinforce only if at least n trials and m correct trials",
    1, 10000
)

// Artifact view parameters

#ifdef BCI2000
# define ARTIFACT_VIEW_SECTION Views
#else
# define ARTIFACT_VIEW_SECTION ArtifactView
#endif

PARAM_DECL(
    PRVisInvalid,
    int,
    ARTIFACT_VIEW_SECTION,
    "1 1 0 1",
    "0 no visual invalid indicator, "
        "1 cross"
        "(enumeration)",
    0, 1
)

PARAM_DECL(
    PRAudInvalid,
    int,
    ARTIFACT_VIEW_SECTION,
    "1 1 0 1",
    "0 no auditory invalid indicator, "
        "1 MIDI"
        "(enumeration)",
    0, 1
)

PARAM_DECL(
    PRGMIVInstrument,
    int,
    ARTIFACT_VIEW_SECTION,
    "57 57 1 128",
    "GM instrument for acoustic invalid indicator",
    0, 128
)

PARAM_DECL(
    PRGMIVVolume,
    int,
    ARTIFACT_VIEW_SECTION,
    "100 100 0 128",
    "MIDI volume for acoustic invalid indicator",
    0, 128
)

PARAM_DECL(
    PRGMIVNote,
    int,
    ARTIFACT_VIEW_SECTION,
    "36 36 0 128",
    "MIDI note for acoustic invalid indicator",
    0, 128
)

// Score view parameters

#ifdef BCI2000
# define SCORE_VIEW_SECTION Views
#else
# define SCORE_VIEW_SECTION ScoreView
#endif

PARAM_DECL(
    PRScoreViewMode,
    int,
    SCORE_VIEW_SECTION,
    "1 1 0 1",
    "score view: "
        "0: off, "
        "1: on"
        "(enumeration)",
    0, 1
)

PARAM_DECL(
    PRScoreFont,
    string,
    SCORE_VIEW_SECTION,
    "Arial Arial a z",
    "font name for score view",
    0, 0
)

PARAM_DECL(
    PRScoreRect,
    floatlist,
    SCORE_VIEW_SECTION,
    "4 -96 -96 -94 -36 0 -101 101",
    "rectangle for score display",
    -101, 101 + EPS
)

#endif // PRESPARAMSH
