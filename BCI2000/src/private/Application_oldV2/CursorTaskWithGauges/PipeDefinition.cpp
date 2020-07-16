////////////////////////////////////////////////////////////////////////////////
// $Id: PipeDefinition.cpp 2656 2010-02-03 16:55:53Z mellinger $
// Description: This file defines which filters will be used, and the sequence
//   in which they are applied.
//   Each Filter() entry consists of the name of the filter and a string which,
//   by lexical comparison, defines the relative position of the filter in the
//   sequence.
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "GaugeAppMain.h"
#include "CursorFeedbackTaskMod.h"

Filter( GaugeApp, 3.A );
Filter( CursorFeedbackTaskMod, 3.B );

