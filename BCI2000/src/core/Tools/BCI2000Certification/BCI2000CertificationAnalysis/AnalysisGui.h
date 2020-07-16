////////////////////////////////////////////////////////////////////////////////
// $Id $
// Authors: Adam Wilson, juergen.mellinger@neurotechcenter.org
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2018: BCI2000 team and many external contributors ]
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
#ifndef ANALYSIS_GUI_H
#define ANALYSIS_GUI_H

#include <QtWidgets>
#include "Analyzer.h"
#include "Runnable.h"
#include "CollapsibleGroup.h"

class ScriptEditor;
namespace { struct FilesListField; }

class AnalysisGui : public QWidget
{
  Q_OBJECT

public:
  AnalysisGui();

private slots:
  void onRequirementsChanged();
  void onRunButtonClicked();
  void onSaveButtonClicked();
  void onSetOutputDirButtonClicked();
  void onProgressCanceled();
  void onAnalyzerBegin();
  void onFileProgress(int cur, int total, QString info);
  void onDataProgress(int cur, int total);
  void onAnalyzerDone();
  void onResultsContextMenu(const QPoint&);

private:
  void onFilesListItemsRemoved();
  void onFilesListItemsAdded();
  void onAnalyzerMessage();
  void clearResults();
  void setGlobalResult(const QString&);
  QString defaultDir(const QString& = "");
  void closeEvent(QCloseEvent*) override;

  FilesListField* mpFilesList;
  QPushButton* mpRunButton, *mpSaveButton;
  QLineEdit* mpOutputDirLineEdit;
  ScriptEditor* mpRequirements, *mpInputScript, *mpAnalysisScript;
  CollapsibleGroup* mpRequirementsGroup, *mpInputScriptGroup, *mpAnalysisScriptGroup;
  QLabel* mpGlobalResultText;
  QTreeView* mpResultsView;
  QSplitter* mpSplitter;

  QProgressDialog mProgress;
  QString mOutputDir;
  std::string mOutputFileName;

  MemberCall<void(AnalysisGui*)> mOnFilesListItemsRemoved, mOnFilesListItemsAdded;

  Analyzer mAnalyzer;
  MemberCall<void(AnalysisGui*)> mOnAnalyzerMessage;
  int mFileCur, mFileTotal;
};

#endif // ANALYSIS_GUI_H
