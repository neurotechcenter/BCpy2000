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

#include "UTargetSequence.h"
#include "BCIError.h"
#include "VCLDefines.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TARGETSEQUENCE::TARGETSEQUENCE()
{
/*shidong starts*/

debug = false;
if(debug)f = fopen("MuSpellerDebug.txt", "w");

/*shidong ends*/

 targets=new TARGETLIST();
 tree=new TREE();
 dictionary=new DICTIONARY();

 BEGIN_PARAMETER_DEFINITIONS
  "Speller int EnablePrediction= 1 "
    "0 0 1 // Enable word prediction (0=no, 1=yes) (boolean)",
  "Speller string DictionaryFile= odgenswords.voc "
    "% % % // Dictionary file for word prediction (inputfile)",
  /*shidong starts*/
  //VK adding SoundFile for audio capability
   "Speller matrix TargetDefinitionMatrix= 42 { ID Type Display FontSizeFactor IconFile SoundFile } 0 4 %20 1 %20 %20 1 4 Back-Up 1 %20 %20 2 2 ABCDEFGHI 1 %20 %20 3 2 JKLMNOPQR 1 %20 %20 4 2 STUVWXYZ 1 %20 %20 5 2 ABC 1 %20 %20 6 2 DEF 1 %20 %20 7 2 GHI 1 %20 %20 8 2 JKL 1 %20 %20 9 2 MNO 1 %20 %20 10 2 PQR 1 %20 %20 11 2 STU 1 %20 %20 12 2 VWX 1 %20 %20 13 2 YZ_ 1 %20 %20 14 3 A 1 %20 %20 15 3 B 1 %20 %20 16 3 C 1 %20 %20 17 3 D 1 %20 %20 18 3 E 1 %20 %20 19 3 F 1 %20 %20 20 3 G 1 %20 %20 21 3 H 1 %20 %20 22 3 I 1 %20 %20 23 3 J 1 %20 %20 24 3 K 1 %20 %20 25 3 L 1 %20 %20 26 3 M 1 %20 %20 27 3 N 1 %20 %20 28 3 O 1 %20 %20 29 3 P 1 %20 %20 30 3 Q 1 %20 %20 31 3 R 1 %20 %20 32 3 S 1 %20 %20 33 3 T 1 %20 %20 34 3 U 1 %20 %20 35 3 V 1 %20 %20 36 3 W 1 %20 %20 37 3 X 1 %20 %20 38 3 Y 1 %20 %20 39 3 Z 1 %20 %20 40 3 _ 1 %20 %20 999 5 DELETE 1 % % 0 0 0 // "
     "Target Definition Matrix",

 /* "Speller matrix TargetDefinitionMatrix= 42 { ID Type Display FontSizeFactor IconFile } 0 4 %20 1 %20 1 4 Back-Up 1 %20 2 2 ABCDEFGHI 1 %20 3 2 JKLMNOPQR 1 %20 4 2 STUVWXYZ 1 %20 5 2 ABC 1 %20 6 2 DEF 1 %20 7 2 GHI 1 %20 8 2 JKL 1 %20 9 2 MNO 1 %20 10 2 PQR 1 %20 11 2 STU 1 %20 12 2 VWX 1 %20 13 2 YZ_ 1 %20 14 3 A 1 %20 15 3 B 1 %20 16 3 C 1 %20 17 3 D 1 %20 18 3 E 1 %20%20 19 3 F 1 %20 20 3 G 1 %20 21 3 H 1 %20 22 3 I 1 %20 23 3 J 1 %20 24 3 K 1 %20 25 3 L 1 %20 26 3 M 1 %20 27 3 N 1 %20 28 3 O 1 %20 29 3 P 1 %20 30 3 Q 1 %20 31 3 R 1 %20 32 3 S 1 %20 33 3 T 1 %20 34 3 U 1 %20 35 3 V 1 %20 36 3 W 1 %20 37 3 X 1 %20 38 3 Y 1 %20 39 3 Z 1 %20 40 3 _ 1 %20 999 5 DELETE 1 % 0 0 0 // "
     "Target Definition Matrix",    */
  "Speller matrix TreeDefinitionMatrix= 65 { ParentID DisplayPosition TargetID } -1 1 1 -1 2 2 -1 3 3 -1 4 4 -1 5 999 2 1 1 2 2 5 2 3 6 2 4 7 2 5 999 3 1 1 3 2 8 3 3 9 3 4 10 3 5 999 4 1 1 4 2 11 4 3 12 4 4 13 4 5 999 5 1 1 5 2 14 5 3 15 5 4 16 5 5 999 6 1 1 6 2 17 6 3 18 6 4 19 6 5 999 7 1 1 7 2 20 7 3 21 7 4 22 7 5 999 8 1 1 8 2 23 8 3 24 8 4 25 8 5 999 9 1 1 9 2 26 9 3 27 9 4 28 9 5 999 10 1 1 10 2 29 10 3 30 10 4 31 10 5 999 11 1 1 11 2 32 11 3 33 11 4 34 11 5 999 12 1 1 12 2 35 12 3 36 12 4 37 12 5 999 13 1 1 13 2 38 13 3 39 13 4 40 13 5 999 0 0 0 // "
  "Tree Definition Matrix",
  /*shidong ends*/
 END_PARAMETER_DEFINITIONS

 activetargets_historynum=0;
 text_historynum=0;
 for (int i=0; i<MAX_TARGETHISTORY; i++)
  activetargets_history[i]=NULL;
}


TARGETSEQUENCE::~TARGETSEQUENCE()
{
 if (targets)           delete targets;
 if (tree)              delete tree;
 if (dictionary)        delete dictionary;
 /*shidong starts*/
if(debug)  fclose(f);
 /*shidong ends*/

 targets=NULL;
 tree=NULL;
 dictionary=NULL;

 DeleteHistory();
}


int TARGETSEQUENCE::Initialize(int numT )
{
int ret;

 // delete the history of all previous active targets
 DeleteHistory();

 // Load the dictionary file
 try {
  if (Parameter("EnablePrediction") == 0)
     prediction=false;
  else
     prediction=true;

  // load and create all potential targets
  /*shidong starts*/
  NUM_TARGETS = numT;
  ret=LoadPotentialTargets();
  /*plist->GetParamPtr("TargetDefinitionMatrix")->GetNumColumns(),
  plist->GetParamPtr("TargetDefinitionMatrix")->GetNumRows(),
  plist->GetParamPtr("TreeDefinitionMatrix")->GetNumColumns(),
  plist->GetParamPtr("TreeDefinitionMatrix")->GetNumRows()   */

        if(debug) fprintf(f, "After LoadPotentialTargets, ret is %d.\n", ret);

  // possibly replace this with something better
  if (ret == 0)  Application->MessageBox(VCLSTR("Could not load target definition matrix."), VCLSTR("Error"), MB_OK);
  if (ret == -1) Application->MessageBox(VCLSTR("Could not load tree definition matrix."), VCLSTR("Error"), MB_OK);
  if (ret <= 0)  return(0);

        if(debug) fprintf(f, "After App->msgBox, ret is %d.\n", ret);

    /*shidong ends*/
 ret=dictionary->LoadDictionary(Parameter("DictionaryFile").c_str(), true);
        if(debug) fprintf(f, "After LoadDictionary, ret is %d, dic file is %s.\n", ret, Parameter("TargetDefinitionMatrix").c_str() );
  // if there was no error, add the dictionary to the potential targets
  if (ret == 1)
     AddDictionary2PotentialTargets();  /* */
} catch(...) {ret=0;}

 // possibly replace this with something better
 if ((ret == 0) && (prediction))
    Application->MessageBox(VCLSTR("Could not find dictionary file. Wrong directory ?"), VCLSTR("Error"), MB_OK);

 return(ret);
}


// delete the history of both the targets and the result text
void TARGETSEQUENCE::DeleteHistory()
{
int     i;

 // delete the history of targets
 for (i=0; i<MAX_TARGETHISTORY; i++)
  if (activetargets_history[i])
     {
     delete activetargets_history[i];
     activetargets_history[i]=NULL;
     }

 activetargets_historynum=0;
 text_historynum=0;
}


//int TARGETSEQUENCE::LoadPotentialTargets(const int targetRow, const int targetCol, const int treeRow, const int treeCol)
int TARGETSEQUENCE::LoadPotentialTargets()
{
/*shidong starts */
 int    targetID, parentID, displaypos, ptr, targettype;
 TARGET *cur_target;
 


 // if we already have a list of potential targets, delete this list
 if (targets) delete targets;
 targets=new TARGETLIST();
 if(debug)fprintf(f, "Row # is %d.\n", Parameter("TargetDefinitionMatrix")->NumRows());
 if(debug)fprintf(f, "ID \tType \tDisplay\tFontSizeFactor\tIconFile\n");



/* */


 // parse the target definition matrix
 for (int i = 0; i < Parameter("TargetDefinitionMatrix")->NumRows(); i++)
 {
  targetID =
        AnsiString(Parameter("TargetDefinitionMatrix")(i,0).c_str()).ToInt();
 cur_target = new TARGET(targetID);
 cur_target->targettype = AnsiString(Parameter("TargetDefinitionMatrix")(i,1).c_str()).ToInt();
 cur_target->Caption = AnsiString(Parameter("TargetDefinitionMatrix")(i,2).c_str());
 cur_target->FontSizeFactor = (float)(Parameter("TargetDefinitionMatrix")(i,3));
 cur_target->IconFile = AnsiString(Parameter("TargetDefinitionMatrix")(i,4).c_str());
 cur_target->SoundFile = AnsiString(Parameter("TargetDefinitionMatrix")(i,5).c_str());
 if(debug)fprintf(f, "%d\t", cur_target->targetID);
 if(debug)fprintf(f, "%d\t", cur_target->targettype);
 if(debug)fprintf(f, "%s\t", cur_target->Caption);
 if(debug)fprintf(f, "%f\n", cur_target->FontSizeFactor);
 if(debug)fprintf(f, "%s\n", cur_target->IconFile);
 if(debug)fprintf(f, "%s\n", cur_target->SoundFile);

 // VK adding for playing .wav files
 if ((cur_target->SoundFile != "") && (cur_target->SoundFile != " "))
 {
   // perform parsing to determine sound file or TextToSpeech
   if (cur_target->SoundFile.SubString(0,1) != "'" && (!cur_target->wavplayer) )  // implies .wav file
   {
     cur_target->wavplayer = new WavePlayer;
     cur_target->wavplayer->SetFile(cur_target->SoundFile.c_str());
   }
 }
 targets->Add(cur_target);
 }//for




/*
char    buf[256], line[256];
FILE    *fp;
int     targetID, parentID, displaypos, ptr, targettype;
TARGET  *cur_target;

 // if we already have a list of potential targets, delete this list
 if (targets) delete targets;
 targets=new TARGETLIST();

 // read the target definition file
 fp=fopen(targetdeffilename, "rb");
 if (!fp) return(0);

 while (!feof(fp))
  {
  fgets(line, 255, fp);
  if (strlen(line) > 2)
     {
     ptr=0;
     // first column ... target code
     ptr=get_argument(ptr, buf, line, 255);
     targetID=atoi(buf);
     cur_target=new TARGET(targetID);
     // second column ... caption
     ptr=get_argument(ptr, buf, line, 255);
     cur_target->Caption=AnsiString(buf).Trim();
     // third column ... icon
     ptr=get_argument(ptr, buf, line, 255);
     cur_target->IconFile=AnsiString(buf).Trim();
     targettype=TARGETTYPE_NOTYPE;
     if ((targetID == TARGETID_BLANK) || (targetID == TARGETID_BACKUP) || (targetID == TARGETID_ROOT))
        targettype=TARGETTYPE_CONTROL;
     if ((targetID >= TARGETID_A) && (targetID <= TARGETID__))
        targettype=TARGETTYPE_CHARACTER;
     if ((targetID >= TARGETID_ABCDEFGHI) && (targetID <= TARGETID_YZ_))
        targettype=TARGETTYPE_CHARACTERS;
     cur_target->targettype=targettype;
     targets->Add(cur_target);
     }
  }
 fclose(fp);
 
  */ 
 // load the tree file to go with the list of targets
 //if (tree->LoadTree(treeRow, treeCol) == 0)
 if (tree->LoadTree() == 0)
    return(-1);
                 
 return(1);
 /*shidong ends*/
}


int TARGETSEQUENCE::AddDictionary2PotentialTargets()
{
TARGET  *cur_target;
int     i, cur_targetID;

 cur_targetID=targets->GetMaxTargetID()+1;

 // go through the dictionary and add the words to the potential targets
 for (i=0; i<dictionary->GetNumWords(); i++)
  {
  cur_target=new TARGET(cur_targetID);
  cur_target->Caption=AnsiString(dictionary->GetWord(i)).Trim();
  cur_target->targettype=TARGETTYPE_WORD;
  targets->Add(cur_target);
  cur_targetID++;
  }

 return(1);
}


// get these targets, in case there is word prediction
TARGETLIST *TARGETSEQUENCE::GetActiveTargetsPrediction(int cur_parentID, BYTE /*backuppos*/, const char *cur_prefix)
{
TARGETLIST *new_list;
int     targetID, i, nummatching;
TARGET  *new_target, *target;
BYTE    offset;
bool    *populated;

 nummatching=dictionary->GetNumMatching(cur_prefix, false);

 // sanity check
 if ((!cur_prefix) || (!prediction) || (nummatching == 0) || (nummatching >= NUM_TARGETS))
    return(NULL);

 new_list=new TARGETLIST;                                               // the list of active targets
 new_list->parentID=cur_parentID;
 populated=new bool[NUM_TARGETS];
 for (i=0; i<NUM_TARGETS; i++)
  populated[i]=false;

 // if BACK-UP is on the top, then start populating farther down
 offset=0;
 /*shiodng starts*/
 //if (backuppos == 0) offset=NUM_TARGETS-nummatching;
 /*shiodng ends*/

 // set the mode of these targets to predictive mode
 new_list->predictionmode=MODE_PREDICTION;

 /*shidong starts*/
 /*
 // create the target for BACK UP
 targetID=TARGETID_BACKUP;
 target=targets->GetTargetPtr(targetID);
 new_target=target->CloneTarget();
 new_target->Color=clYellow;
 // new_target->TextColor=clGreen;
 new_target->TextColor=clBlack;
 new_target->parentID=cur_parentID;
 new_target->targetposition=backuppos;
 populated[new_target->targetposition]=true;            // mark that target as being occupied
 new_list->Add(new_target);

 /*shiodng ends*/
 // create the targets with the words in them
 for (i=0; i<nummatching; i++)
  {
  target=targets->GetTargetPtr(dictionary->GetMatchingWord(cur_prefix, false, i));
  new_target=target->CloneTarget();
  new_target->parentID=cur_parentID;
  new_target->Color=clYellow;
  // new_target->TextColor=clGreen;
        /*shidong starts*/
        new_target->targettype= target->targettype;
        /*shidong ends*/
  new_target->TextColor=clBlack;
  new_target->targetposition=offset+(BYTE)i;
  populated[new_target->targetposition]=true;           // mark that target as being occupied
  new_list->Add(new_target);
  }

 // populate the "missing" targets with blank targets
 for (i=0; i<NUM_TARGETS; i++)
  {
  // if that target population is not populated ...
  if (!populated[i])
     {
     // create a blank target
     targetID=TARGETID_BLANK;
     target=targets->GetTargetPtr(targetID);
     new_target=target->CloneTarget();
     new_target->parentID=cur_parentID;
     new_target->Color=clYellow;
     /*shidong starts*/
     new_target->targettype = target->targettype;
     /*shidong ends*/
     // new_target->TextColor=clGreen;
     new_target->TextColor=clBlack;
     new_target->targetposition=i;
     new_list->Add(new_target);
     }
  }

 delete [] populated;
 return(new_list);
}


// gets new targets, given a certain rootID
// i.e., the targetID of the last selected target
// parentID==TARGETID_ROOT on start
TARGETLIST *TARGETSEQUENCE::GetActiveTargets(int cur_parentID,  const char *cur_prefix)
{
// BYTE backuppos
TARGETLIST *new_list;
int     targetID, displaypos, count, nummatching;
TARGET  *new_target, *target;
BYTE    offset;
bool    fullypopulated;                 // specifies whether or not there is a target for every display position. if not, then we shuffle backup around in a funky fashion

/*shidong starts*/
int     backuppos;                      //position of the "Back-Up"



  
/*shidong ends*/


 // include prediction, if we specified a prefix and we enabled prediction, and
 // if the number of possible words with this prefix fits in the free targets
 if ((cur_prefix) && (prediction))
    {
    nummatching=dictionary->GetNumMatching(cur_prefix, false);
    if ((nummatching > 0) && (nummatching < NUM_TARGETS))
       return(GetActiveTargetsPrediction(cur_parentID, backuppos,  cur_prefix));
    }

 new_list=new TARGETLIST;                                               // the list of active targets
 new_list->parentID=cur_parentID;

 // set the mode of these targets to normal mode, i.e., no prediction
 new_list->predictionmode=MODE_NORMAL;

 // if BACK-UP is on the top, shift everything down by one
 offset=0;
 /*shidong starts*/
 //if (backuppos == 0) offset=1;
 /*shidong ends*/

 // in the tree, the first three displaypositions are reserved for the actual targets
 // the last displayposition is not in the tree, but will be dynamically assigned here for BACKUP
 // the logic is a little strange, please forgive

 // count the number of targets on the screen
 // if there is a target defined for each display position, no need to shuffle BACKUP around, etc.
 count=0;
 fullypopulated=false;
 for (displaypos=0; displaypos<NUM_TARGETS; displaypos++)
  {
  targetID=tree->DetermineTargetID(cur_parentID, displaypos);

  //if(debug) fprintf(f, "in loop, count is %d, cur_parentID is %d, displaypos is %d, and id is %d.\n", count,cur_parentID, displaypos, targetID);

  if (targetID != TARGETID_NOID) count++;
  }
 if(debug) fprintf(f,"count is %d. \n", count);
 if (count == NUM_TARGETS)
    {
    fullypopulated=true;
    offset=0;
    }
 if(debug) fprintf(f,"\nB4 reset count is %d. targetID is %d, fully is %d.\n", count, targetID, fullypopulated);

 // if there is at least one target (as defined by the tree) on the screen, populate the targets
 if (count > 0)
    {
    count=0;
    for (displaypos=0; displaypos<NUM_TARGETS; displaypos++)
     {
     // if every display position on the screen is filled with a target, then just get that target ID
     if (fullypopulated)
        {
        if(debug) fprintf(f,"The list is fully populated.\n");
        targetID=tree->DetermineTargetID(cur_parentID, displaypos);
        }
     else
        {
                if(debug) fprintf(f,"The list is NOT fully populated.\n");

                if ((displaypos == NUM_TARGETS-1) && (activetargets_historynum == 0))     // if we are at root and we haven't spelled anything (i.e., there is no history), then use a blank target
                {
                        targetID=TARGETID_BLANK;
                }
                else                                                                      // otherwise use the target determined by the tree or determine BACK UP
                {
                if (displaypos == NUM_TARGETS-1)
                {
                        // if we are at the last position, then we should have had NUM_TARGETS-1
                        // if not, then we might have selected a dummy target that does not lead to anything in the tree
                        if (count == NUM_TARGETS-1)
                        targetID=TARGETID_BACKUP;
                        else
                        targetID=TARGETID_NOID;
                }
                else
                        targetID=tree->DetermineTargetID(cur_parentID, displaypos);
                }
        }//else the list is not fully populated
//if(debug) fprintf(f,"B4 safety check, count is %d. targetID is %d.\n", count, targetID);        
        
     // safety check ...
     if (targetID == TARGETID_NOID) targetID=TARGETID_BLANK;
     target=targets->GetTargetPtr(targetID);
     if ((targetID >= 0) && (target))
        {
        // add to active targets
        new_target=target->CloneTarget();
        new_target->Color=clYellow;
        // new_target->TextColor=clGreen;
        new_target->TextColor=clBlack;
        new_target->parentID=cur_parentID;
        /*shidong starts*/
        new_target->targettype= target->targettype;
        /*shidong ends*/
        // if the current targetID is "BLANK" or "BACKUP" choose the defined displayposition
        // otherwise, choose the displayposition stored in the tree, offset by whether or not BACKUP is the first or last target
        /*shidong starts*/
        /*
        if ((targetID == TARGETID_BLANK) || (targetID == TARGETID_BACKUP))
           new_target->targetposition=backuppos;
        else */
           new_target->targetposition=(BYTE)displaypos+offset;
        if(debug) fprintf(f, "The new target's ID is %d, position is %d.\n", new_target->targetID, new_target->targetposition);

        /*shidong ends*/

        new_list->Add(new_target);
        count++;
        }
        if(debug) fprintf(f, "count is %d, and curTargetID is %d.\n", count, targetID);
        if(debug) fprintf(f, "list has %d targets, last target's ID is %d.\n\n", new_list->GetNumTargets(), new_target->targetID);
     } //for
    } //if

 // if, for some reason, the lists contain no target, then display the root targets
 // and delete the (empty) lists of current targets and in history
 if (new_list->GetNumTargets() == 0)
    {
    if(debug) fprintf(f, "This should never be called.\n");

    delete new_list;
    new_list=GetActiveTargets(TARGETID_ROOT, NULL);
    }
 if(debug) fprintf(f, "This Active Target List has %d targets.\n", new_list->GetNumTargets());
 return(new_list);
}


void TARGETSEQUENCE::PushTargetsOnHistory(TARGETLIST *activetargets)
{
TARGET  *target;
int     i, targetID;


  if(debug) fprintf(f, "In pushTargetHistory, The push Target List has %d targets.\n", activetargets->GetNumTargets());
  if(debug) fprintf(f, "In PushTargetsOnHistory, before push activetargets_historynum is  %d.\n", activetargets_historynum);

 // don't do anything if the list of active targets does not contain a single target
 if (activetargets->GetNumTargets() == 0)
    return;

 activetargets_history[activetargets_historynum]=new TARGETLIST;        // add the same thing to the history of previous target lists
 activetargets_history[activetargets_historynum]->predictionmode=activetargets->predictionmode;
 for (i=0; i<activetargets->GetNumTargets(); i++)
  {
  targetID=activetargets->GetTargetID(i);
  target=activetargets->GetTargetPtr(targetID, i);

  /*shidong starts*/
        if(debug) fprintf(f, "IN LOOP,  current dsiplaypos is %d, and the target's ID is %d.\n", i, target->targetID);
  
  TARGET *toAdd;
  toAdd = target->CloneTarget();
  toAdd->targettype = target->targettype;

  // add also to target history so that we can back up
  // only if, of course, we don't run outa space
  if (activetargets_historynum < MAX_TARGETHISTORY-1)
     activetargets_history[activetargets_historynum]->Add(toAdd);
  }        /*shidong ends*/
 activetargets_historynum++;
 if(debug) fprintf(f, "In PushTargetsOnHistory, after push activetargets_historynum is  %d.\n", activetargets_historynum);

}


void TARGETSEQUENCE::PushTextOnHistory(AnsiString text)
{
 if (text_historynum < MAX_TEXTHISTORY-1)
    {
    text_history[text_historynum]=text;
    text_historynum++;
    }
}


// retrieves previous targets
// in case we are already at the very start (in order to prevent crashing)
// just return the target list for the rootID
TARGETLIST *TARGETSEQUENCE::GetPreviousTargets()
{
TARGETLIST *ptr;

  if(debug) fprintf(f, "In GetPreviousTargets, activetargets_historynum is  %d.\n", activetargets_historynum);

 // we are at the start already
 if (activetargets_historynum == 0)
    return(GetActiveTargets(TARGETID_ROOT, NULL));

 // otherwise, return the previous list
 activetargets_historynum--;
 ptr=activetargets_history[activetargets_historynum];
 activetargets_history[activetargets_historynum]=NULL;          // 'release' this list of targets; active targets will be freed at some other place


  if(debug) fprintf(f, "In GetPreviousTargets, After decrease, ptr has %d targets.\n", ptr->GetNumTargets());

  if(debug) fprintf(f, "In GetPreviousTargets, After decrease, activetargets_historynum is  %d.\n", activetargets_historynum);

 return(ptr);
}


// retrieves previous text
// in case we are already at the very start (in order to prevent crashing)
// (we shouldn't be able to back up from the first step anyway)
// just return " "
AnsiString TARGETSEQUENCE::GetPreviousText()
{
 // we are at the start already
 if (text_historynum == 0)
    return("");

 // otherwise, return the previous text
 text_historynum--;
 return(text_history[text_historynum]);
}


// **************************************************************************
// Function:   get_argument
// Purpose:    parses a line
//             it returns the next token that is being delimited by a ";"
// Parameters: ptr - index into the line of where to start
//             buf - destination buffer for the token
//             line - the whole line
//             maxlen - maximum length of the line
// Returns:    the index into the line where the returned token ends
// **************************************************************************
int TARGETSEQUENCE::get_argument(int ptr, char *buf, const char *line, int maxlen) const
{
 // skip one preceding semicolon, if there is any
 if ((line[ptr] == ';') && (ptr < maxlen))
    ptr++;

 // go through the string, until we either hit a semicolon, or are at the end
 while ((line[ptr] != ';') && (line[ptr] != '\n') && (line[ptr] != '\r') && (ptr < maxlen))
  {
  *buf=line[ptr];
  ptr++;
  buf++;
  }

 *buf=0;
 return(ptr);
}
