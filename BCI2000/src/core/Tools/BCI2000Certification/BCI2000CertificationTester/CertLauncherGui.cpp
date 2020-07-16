////////////////////////////////////////////////////////////////////////////////
// $Id: CertLauncherGui.cpp 5826 2018-11-23 14:14:08Z mellinger $
// Authors: Adam Wilson, juergen.mellinger@neurotechcenter.org
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
#include "CertLauncherGui.h"
#include <iostream>
#include <vector>
#include <string>

#include "FileUtils.h"
#include "StringUtils.h"
#include "ProcessUtils.h"
#include "Settings.h"
#include "ModuleInventory.h"

#include "AboutBox.h"
#include "ExecutableHelp.h"

#pragma warning( disable : 4482 )

Q_DECLARE_METATYPE(Config*)
Q_DECLARE_METATYPE(const Config*)
Q_DECLARE_METATYPE(TaskType*)
Q_DECLARE_METATYPE(const TaskType*)

namespace {

  auto sDefaultColor = Qt::white;
  auto sErrorColor = QColor(255, 50, 50);
  auto sOKColor = QColor(50, 255, 50);

  bool directoryContainsExtensionRecurse(const std::string& dir, const std::string& ext, int depth)
  {
    if(depth < 1)
      return false;
    FileUtils::List files, subdirs;
    FileUtils::ListDirectory(dir, files);
    for(const auto& file : files)
    {
      auto name = dir + "/" + file;
      if(FileUtils::IsDirectory(name))
        subdirs.push_back(name);
      else if(StringUtils::ToLower(FileUtils::ExtractExtension(file)) == ext)
        return true;
    }
    for(const auto& subdir : subdirs)
      if(directoryContainsExtensionRecurse(subdir, ext, depth - 1))
        return true;
    return false;
  }

  bool directoryContainsExtension(const std::string& dir, const std::string& ext, int depth)
  {
    return directoryContainsExtensionRecurse(FileUtils::AbsolutePath(dir), StringUtils::ToLower(ext), depth);
  }
};

CertLauncherGui::CertLauncherGui()
: mOnLauncherEvent(&CertLauncherGui::onLauncherEvent, this)
{
  qRegisterMetaType<Config*>();
  qRegisterMetaType<const Config*>();
  qRegisterMetaType<TaskType*>();
  qRegisterMetaType<const TaskType*>();
  mLauncher.AddListener(CertLauncher::onTaskBegin, &mOnLauncherEvent);
  mLauncher.AddListener(CertLauncher::onDone, &mOnLauncherEvent);
  mLauncher.AddListener(CertLauncher::onError, &mOnLauncherEvent);
  initUi();
  initVals();
}

CertLauncherGui::~CertLauncherGui()
{
  mLauncher.RemoveListener(CertLauncher::onDone, &mOnLauncherEvent);
  mLauncher.RemoveListener(CertLauncher::onTaskBegin, &mOnLauncherEvent);
}

void CertLauncherGui::initUi()
{
  this->setMinimumWidth(400);
  this->setMinimumHeight(400);
  QTabWidget *widget = new QTabWidget;
  setCentralWidget(widget);
  QGridLayout *mainLayout2 = new QGridLayout;
  QWidget *controlWidget = new QWidget;
  QGridLayout *mainLayout = new QGridLayout;
  QWidget *configWidget = new QWidget;
  widget->addTab(configWidget, "Configuration");
  widget->addTab(controlWidget, "Control");
  configWidget->setLayout(mainLayout);
  controlWidget->setLayout(mainLayout2);
  connect(widget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged()));

  //task list objects
  QGroupBox *taskGroup = new QGroupBox("Task List");
  QGridLayout *taskLayout = new QGridLayout;
  taskGroup->setLayout(taskLayout);
  mpTaskList = new QListWidget;
  mpTaskList->setSelectionMode(QAbstractItemView::ExtendedSelection);
  connect(mpTaskList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(updateTaskDetails()));
  connect(mpTaskList, SIGNAL(itemSelectionChanged()), this, SLOT(onTaskListItemSelectionChanged()));
  connect(mpTaskList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(onTaskListItemChanged(QListWidgetItem*)));

  mpAddTaskBtn = new QPushButton("+");
  mpDelTaskBtn = new QPushButton("-");
  mpDelTaskBtn->setEnabled(false);
  mpCopyTaskBtn = new QPushButton("Copy");
  mpCopyTaskBtn->setEnabled(false);
  mpTaskUpBtn = new QPushButton("Up");
  mpTaskUpBtn->setEnabled(false);
  mpTaskDownBtn = new QPushButton("Down");
  mpTaskDownBtn->setEnabled(false);
  taskLayout->addWidget(mpTaskList,0,0,1,3);
  taskLayout->addWidget(mpTaskUpBtn,1,0,1,1);
  taskLayout->addWidget(mpTaskDownBtn,1,1,1,1);
  taskLayout->addWidget(mpAddTaskBtn,2,0,1,1);
  taskLayout->addWidget(mpDelTaskBtn,2,1,1,1);
  taskLayout->addWidget(mpCopyTaskBtn,2,2,1,1);
  taskGroup->setMaximumWidth(230);
  //SIGNALS/SLOTS
  connect(mpTaskUpBtn, SIGNAL(clicked()), this, SLOT(onTaskBtnClicked()));
  connect(mpTaskDownBtn, SIGNAL(clicked()), this, SLOT(onTaskBtnClicked()));
  connect(mpAddTaskBtn, SIGNAL(clicked()), this, SLOT(onTaskBtnClicked()));
  connect(mpDelTaskBtn, SIGNAL(clicked()), this, SLOT(onTaskBtnClicked()));
  connect(mpCopyTaskBtn, SIGNAL(clicked()), this, SLOT(onTaskBtnClicked()));
  mainLayout->addWidget(taskGroup,0,0,1,1);

  mpDelTaskActn = new QAction("Delete", this);
  mpDelTaskActn->setShortcut(QKeySequence::Delete);
  connect(mpDelTaskActn, SIGNAL(triggered()), mpDelTaskBtn, SIGNAL(clicked()));

  mpSelectAllTasksActn = new QAction("Select &all", this);
  mpSelectAllTasksActn->setShortcut(QKeySequence::SelectAll);
  connect(mpSelectAllTasksActn, SIGNAL(triggered()), mpTaskList, SLOT(selectAll()));


  // controls
  QGroupBox *controlGroup = new QGroupBox("Controls");
  QGridLayout *controlLayout = new QGridLayout;
  controlGroup->setLayout(controlLayout);
  mpStartBtn = new QPushButton("Start");
  mpStartBtn->setEnabled(false);
  mpAnalyzeBtn = new QPushButton("Analyze");
  mpAnalyzeBtn->setEnabled( false );

  mpConfigOnStartChk = new QCheckBox("Auto Set Config");
  mpConfigOnStartChk->setTristate(false);
  mpConfigOnStartChk->setProperty("Tasks", "AutoCfg");
  mpRunOnStartChk = new QCheckBox("Auto Start");
  mpRunOnStartChk->setTristate(false);
  mpRunOnStartChk->setProperty("Tasks", "AutoStart");
  mpQuitOnSuspendChk = new QCheckBox("Auto Quit");
  mpQuitOnSuspendChk->setTristate(false);
  mpQuitOnSuspendChk->setProperty("Tasks", "AutoQuit");

  controlLayout->addWidget(mpConfigOnStartChk,0,0,1,1);
  controlLayout->addWidget(mpRunOnStartChk,0,1,1,1);
  controlLayout->addWidget(mpQuitOnSuspendChk,0,2,1,1);
  controlLayout->addWidget(mpStartBtn,1,1,1,1);
  controlLayout->addWidget(mpAnalyzeBtn,1,2,1,1);
  mainLayout2->addWidget(controlGroup,1,0,1,1);

  mainLayout2->setColumnStretch(1,100);
  mainLayout2->setRowStretch(2,100);

  //globals
  mpGlobalGroup = new QGroupBox("Global Settings");
  QGridLayout *globalLayout = new QGridLayout;
  mpGlobalGroup->setLayout(globalLayout);
  mpWindowLeft = new LabeledLineEdit("Window Left");
  mpWindowLeft->setProperty("Tasks", "WindowLeft");
  mpWindowTop = new LabeledLineEdit("Window Top");
  mpWindowTop->setProperty("Tasks", "WindowTop");
  mpWindowWidth = new LabeledLineEdit("Window Width");
  mpWindowWidth->setProperty("Tasks", "WindowWidth");
  mpWindowHeight = new LabeledLineEdit("Window Height");
  mpWindowHeight->setProperty("Tasks", "WindowHeight");
  mpDefaultSourceBox = new LabeledComboBox("Global Signal Source");
  mpDefaultSourceBox->setProperty("Tasks", "DefaultSource");
  mpDefaultSourceBox->addItems(bci::Inventory()[bci::Inventory::SignalSource]);
  mpDataDirBox = new GetDirWidget("Data Output Directory", "Select a Data Output Directory");
  mpDataDirBox->setProperty("Tasks", "DataDir");
  connect(mpDataDirBox->lineEdit(), SIGNAL(textChanged(QString)), this, SLOT(onDataDirTextChanged(QString)));
  mpRootDirBox = new GetDirWidget("BCI2000 Directory", "Select BCI2000 base Directory");
  mpRootDirBox->setProperty("Tasks", "RootDir");

  mpAppAddArguments = new LabeledLineEdit("Additional Application Module command line arguments");
  mpAppAddArguments->setProperty("Tasks", "AppAddArguments");
  mpExecuteBefore = new LabeledLineEdit("Shell execute before");
  mpExecuteBefore->setProperty("Tasks", "ExecuteBefore");
  mpExecuteAfter = new LabeledLineEdit("Shell execute after");
  mpExecuteAfter->setProperty("Tasks", "ExecuteAfter");

  globalLayout->addWidget(mpWindowLeft,0,0);
  globalLayout->addWidget(mpWindowTop,0,1);
  globalLayout->addWidget(mpWindowWidth,0,2);
  globalLayout->addWidget(mpWindowHeight,0,3);
  globalLayout->addWidget(mpDefaultSourceBox,1,0,1,4);
  globalLayout->addWidget(mpDataDirBox,2,0,1,4);
  globalLayout->addWidget(mpRootDirBox,3,0,1,4);
  globalLayout->addWidget(mpAppAddArguments,4,0,1,4);
  globalLayout->addWidget(mpExecuteBefore,5,0,1,2);
  globalLayout->addWidget(mpExecuteAfter,5,2,1,2);

  mpDefaultSourceBox->setProperty("Tasks", "DefaultSource");
  mpDefaultSourceBox->setText("");
  //signals
  mainLayout2->addWidget(mpGlobalGroup,0,0,1,1);

  //parameters
  mpTaskDetails = new QGroupBox("Task Details");
  QGridLayout *detailsLayout = new QGridLayout;
  mpTaskNameBox = new LabeledLineEdit("Task Name");
  mpTaskNameBox->setObjectName("TaskName");
  connect(mpTaskNameBox->lineEdit(), SIGNAL(textChanged(QString)), this, SLOT(onTaskNameTextChanged(QString)));
  detailsLayout->addWidget(mpTaskNameBox,0,0,1,3);

  auto pComboBox = new LabeledComboBox("Signal Source");
  pComboBox->setProperty("TaskType", "Source");
  pComboBox->addItems(bci::Inventory()[bci::Inventory::SignalSource]);
  pComboBox->setText("");
  detailsLayout->addWidget(pComboBox, 1, 0);

  pComboBox = new LabeledComboBox("Signal Processing");
  pComboBox->setProperty("TaskType", "SigProc");
  pComboBox->addItems(bci::Inventory()[bci::Inventory::SignalProcessing]);
  pComboBox->setText("");
  detailsLayout->addWidget(pComboBox, 2, 0);

  pComboBox = new LabeledComboBox("Application");
  pComboBox->setProperty("TaskType", "App");
  pComboBox->addItems(bci::Inventory()[bci::Inventory::Application]);
  pComboBox->setText("");
  detailsLayout->addWidget(pComboBox, 3, 0);

  QGroupBox *eventGroup = new QGroupBox("Events Configuration");
  QGridLayout *eventLayout = new QGridLayout;
  eventGroup->setLayout(eventLayout);

  auto pBox = new LabeledLineEdit("Amp Channel");
  pBox->setProperty("TaskType", "AmpChannel");
  eventLayout->addWidget(pBox,0,0);

  pBox = new LabeledLineEdit("Amp Stream");
  pBox->setProperty("TaskType", "AmpStream");
  eventLayout->addWidget(pBox,0,1);

  pBox = new LabeledLineEdit("Sample Rate");
  pBox->setProperty("TaskType", "SampleRate");
  eventLayout->addWidget(pBox,0,2);

  pBox = new LabeledLineEdit("Sample Block Size");
  pBox->setProperty("TaskType", "BlockSize");
  eventLayout->addWidget(pBox,0,3);

  pBox = new LabeledLineEdit("Video Channel");
  pBox->setProperty("TaskType", "VidChannel");
  eventLayout->addWidget(pBox,1,0);

  pBox = new LabeledLineEdit("Video Stream");
  pBox->setProperty("TaskType", "VidStream");
  eventLayout->addWidget(pBox,1,1);

  pBox = new LabeledLineEdit("Video State");
  pBox->setProperty("TaskType", "VidStateName");
  eventLayout->addWidget(pBox,1,2);

  pBox = new LabeledLineEdit("Video State Values");
  pBox->setProperty("TaskType", "VidStateValues");
  eventLayout->addWidget(pBox,1,3);

  pBox = new LabeledLineEdit("Audio Channel");
  pBox->setProperty("TaskType", "AudChannel");
  eventLayout->addWidget(pBox,2,0);

  pBox = new LabeledLineEdit("Audio Stream");
  pBox->setProperty("TaskType", "AudStream");
  eventLayout->addWidget(pBox,2,1);

  pBox = new LabeledLineEdit("Audio State");
  pBox->setProperty("TaskType", "AudStateName");
  eventLayout->addWidget(pBox,2,2);

  pBox = new LabeledLineEdit("Audio State Values");
  pBox->setProperty("TaskType", "AudStateValues");
  eventLayout->addWidget(pBox,2,3);

  QGroupBox *parmGroup = new QGroupBox("Parameters");
  QGridLayout *parmLayout = new QGridLayout;
  parmGroup->setLayout(parmLayout);
  mpParmList = new QListWidget;
  mpAddParmBtn = new QPushButton("+");
  mpDelParmBtn = new QPushButton("-");
  parmLayout->addWidget(mpParmList,0,0,3,3);
  parmLayout->addWidget(mpAddParmBtn,3,1);
  parmLayout->addWidget(mpDelParmBtn,3,2);
  parmGroup->setMaximumHeight(150);
  mpParmList->setSelectionMode(QAbstractItemView::ExtendedSelection);
  connect(mpAddParmBtn, SIGNAL(clicked()), this, SLOT(addParmsBtn()));
  connect(mpDelParmBtn, SIGNAL(clicked()), this, SLOT(delParmsBtn()));

  auto pCheckBox = new QCheckBox("Video Frame Test");
  pCheckBox->setTristate(false);
  pCheckBox->setProperty("TaskType", "VidFrameTest");
  parmLayout->addWidget(pCheckBox, 4, 0);

  detailsLayout->addWidget(parmGroup,1,1,3,2);
  detailsLayout->addWidget(eventGroup,4,0,1,3);
  mpTaskDetails->setLayout(detailsLayout);
  mpTaskDetails->setEnabled(false);

  mainLayout->addWidget(mpTaskDetails,0,1);

  auto pMenu = menuBar()->addMenu("&File");
  pMenu->addAction("&Open", this, SLOT(onFileOpen()), QKeySequence::Open);
  pMenu->addAction("&Save", this, SLOT(onFileSave()), QKeySequence::Save);
  pMenu->addAction("Save &as ...", this, SLOT(onFileSaveAs()), QKeySequence::SaveAs);
  pMenu->addAction("&Close", this, SLOT(onFileClose()), QKeySequence::Close);
  pMenu->addSeparator();
  pMenu->addAction("&Quit", this, SLOT(close()), QKeySequence::Quit);
  pMenu = new AutoEditMenu(this);
  menuBar()->addMenu(pMenu);
  connect(pMenu, SIGNAL(populate(QWidget*, QList<QAction*>&)),
    this, SLOT(onEditMenuPopulate(QWidget*, QList<QAction*>&)));
  pMenu = menuBar()->addMenu("&Help");
  pMenu->addAction("&Help ...", this, SLOT(onHelpHelp()), QKeySequence::HelpContents);
  pMenu->addAction("&About ...", this, SLOT(onHelpAbout()));

  eventLayout->setSpacing(0);
  detailsLayout->setSpacing(0);
  parmLayout->setSpacing(0);
  globalLayout->setSpacing(0);

  connect(mpStartBtn, SIGNAL(clicked()), this, SLOT(onStartBtnClicked()));
  connect(mpAnalyzeBtn, SIGNAL(clicked()), this, SLOT(onAnalyzeBtnClicked()));

  for(auto p : findChildren<QLineEdit*>())
    connect(p, SIGNAL(textChanged(QString)), this, SLOT(onWidgetTextChanged(QString)));
  for (auto p : findChildren<QComboBox*>())
    connect(p, SIGNAL(currentTextChanged(QString)), this, SLOT(onWidgetTextChanged(QString)));
  for(auto p : findChildren<QCheckBox*>())
    connect(p, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));

  mpProgressDialog = new QProgressDialog(this);
  mpProgressDialog->setAutoClose(true);
  mpProgressDialog->setAutoReset(true);
  mpProgressDialog->setMinimumSize(this->minimumWidth(), 50);
  mpProgressDialog->setVisible(false);
  mpProgressDialog->setMinimum(0);
  mpProgressDialog->setMaximum(1);
  mpProgressDialog->setValue(1);
  connect(mpProgressDialog, SIGNAL(canceled()), this, SLOT(onProgressDialogCanceled()));

  updateWindowTitle();
}

void CertLauncherGui::updateWindowTitle()
{
  QString title = String(FileUtils::StripDirectory(mConfig.fileName()));
  if(!title.isEmpty())
    title += " - ";
  title += QGuiApplication::applicationDisplayName();
  setWindowTitle(title);
}

void CertLauncherGui::initVals()
{
  Settings s;
  mpWindowLeft->lineEdit()->setText(s.value("AppWindowLeft", "0").toString());
  mpWindowTop->lineEdit()->setText(s.value("AppWindowTop", "0").toString());
  mpWindowWidth->lineEdit()->setText(s.value("AppWindowWidth", "800").toString());
  mpWindowHeight->lineEdit()->setText(s.value("AppWindowHeight", "800").toString());
  mpAppAddArguments->lineEdit()->setText(s.value("AppAddArguments", "").toString());
  mpExecuteBefore->lineEdit()->setText(s.value("ExecuteBefore", "").toString());
  mpExecuteAfter->lineEdit()->setText(s.value("ExecuteAfter", "").toString());

  mpConfigOnStartChk->setCheckState(Qt::Checked);
  mpRunOnStartChk->setCheckState(Qt::Checked);
  mpQuitOnSuspendChk->setCheckState(Qt::Checked);

  mpDefaultSourceBox->setText("");
  std::string rootDir = FileUtils::ExtractDirectory(FileUtils::ExecutablePath()) + "../../";
  rootDir = FileUtils::NormalizedPath(rootDir);
  if(FileUtils::IsDirectory(rootDir))
  {
    mpRootDirBox->setText(rootDir);
    std::string dataDir = FileUtils::NormalizedPath( rootDir + "/Data" );
    if( !dataDir.empty() )
    {
      dataDir = FileUtils::EnsureSeparator(dataDir) + "Certification";
      std::string name = dataDir;
      int i = 0;
      while( FileUtils::IsDirectory( name ) )
      {
        std::ostringstream oss;
        oss << "." << ++i;
        name = dataDir + oss.str();
      }
      dataDir = name;
    }
    mpDataDirBox->setText(dataDir);
  }
}

void CertLauncherGui::saveSettings()
{
  Settings s;
  s.setValue("AppWindowLeft", mpWindowLeft->lineEdit()->text());
  s.setValue("AppWindowTop", mpWindowTop->lineEdit()->text());
  s.setValue("AppWindowWidth", mpWindowWidth->lineEdit()->text());
  s.setValue("AppWindowHeight", mpWindowHeight->lineEdit()->text());
  s.setValue("AppAddArguments", mpAppAddArguments->lineEdit()->text());
  s.setValue("ExecuteBefore", mpExecuteBefore->lineEdit()->text());
  s.setValue("ExecuteAfter", mpExecuteAfter->lineEdit()->text());
}

bool CertLauncherGui::allowDestructiveAction()
{
  if (mConfig.tasksModified())
  {
    auto response = QMessageBox::question(this, "Save ini File",
      "Changes have been made to the tasks.\n"
      "Would you like to save changes, discard changes, or cancel?",
      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
    );
    switch(response)
    {
    case QMessageBox::Save:
      onFileSave();
      /* fall through */
    case QMessageBox::Discard:
      return true;
    }
    return false;
  }
  return true;
}

void CertLauncherGui::closeEvent(QCloseEvent *ev)
{
  if(allowDestructiveAction())
  {
    saveSettings();
    ev->accept();
  }
  else
  {
    ev->ignore();
  }
}

void CertLauncherGui::addParmsBtn()
{
  QStringList fname = QFileDialog::getOpenFileNames(this, "Select BCI2000 Parameter Files", 0, "prm files (*.prm)");

  if (fname.size() == 0)
    return;

  for (int i = 0; i < fname.size(); ++i)
    fname[i] = QFileInfo(fname[i]).fileName();

  auto pTask = taskFromListItem(mpTaskList->currentItem());
  for (int i = 0; i < fname.size(); i++)
    pTask->addParm(toString(fname[i]));

  updateTaskDetails();
}


void CertLauncherGui::onFileOpen()
{
  if(allowDestructiveAction())
  {
    QString fname = QFileDialog::getOpenFileName(this, "Select ini File", 0, "ini files (*.ini)");

    if (fname.size() == 0)
      return;

    openIni(fname);
  }
}

void CertLauncherGui::onFileClose()
{
  if(allowDestructiveAction())
    openIni("");
}

void CertLauncherGui::onFileSave()
{
  if (mConfig.fileName().empty()){
    onFileSaveAs();
  }
  else{
    saveIni(toQString(mConfig.fileName()));
  }
}

void CertLauncherGui::onFileSaveAs()
{
  auto defaultFile = toQString(mConfig.fileName());
  QString fname = QFileDialog::getSaveFileName(this, "Save ini File", defaultFile, "ini files (*.ini)");

  if (fname.size() == 0)
    return;

  saveIni(fname);
}

void CertLauncherGui::onHelpHelp()
{
  ExecutableHelp().Display();
}

void CertLauncherGui::onHelpAbout()
{
  AboutBox().Display();
}

void CertLauncherGui::onEditMenuPopulate(QWidget* p, QList<QAction*>& items)
{
  if(p == mpTaskList)
  {
    mpDelTaskActn->setEnabled(!mpTaskList->selectedItems().empty());
    mpSelectAllTasksActn->setEnabled(mpTaskList->count() > 0);
    for(auto& item : items)
    {
      if(item->shortcut().matches(QKeySequence::Delete) == QKeySequence::ExactMatch)
        item = mpDelTaskActn;
      else if(item->shortcut().matches(QKeySequence::SelectAll) == QKeySequence::ExactMatch)
        item = mpSelectAllTasksActn;
    }
  }
}

void CertLauncherGui::saveIni(const QString& file)
{
  String badTasks;
  for(auto& task : mConfig.Tasks.value())
  {
    if(task.hasErrors())
      badTasks += task.Name.toString() + "\n";
  }
  if(!badTasks.empty())
  {
    QMessageBox msg;
    msg.setWindowTitle("Save despite errors?");
    msg.setText(
      "Some task definitions are invalid.\nSave nevertheless?"
    );
    msg.setDetailedText(toQString(badTasks));
    msg.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Save);
    msg.exec();
    if(msg.result() != QMessageBox::Save)
      return;
  }
  bool ok = mConfig.writeToFile(toString(file));
  if(!ok)
  {
    QMessageBox::critical(this, "Error", "Could  not write to\n" + file);
    return;
  }
  updateWindowTitle();
}

bool CertLauncherGui::openIni(const QString& file)
{
  bool ok =  mConfig.readFromFile(toString(file));
  if (!file.isEmpty())
  {
    if(!ok && !FileUtils::IsFile(toString(file)))
    {
      QMessageBox::critical(this, "Error", file + "\ndoes not exist.");
      return false;
    }
    if(!ok)
    {
      QMessageBox::critical(this, "Error", "Could not read\n" + file + "\nas a certification .ini file.");
      return false;
    }
  }
  mpTaskList->clear();
  for(auto& task : mConfig.Tasks.value())
  {
    auto item = createTaskListItem(&task, -1);
    updateTaskListItemInfo(item);
  }
  mpDefaultSourceBox->setText(mConfig.DefaultSource.value());
  updateWindowTitle();
  return true;
}

QListWidgetItem*
CertLauncherGui::createTaskListItem(TaskType* pTask, int atIdx)
{
  QListWidgetItem *item = new QListWidgetItem(toQString(pTask->Name.toString()));
  item->setCheckState(pTask->Skip ? Qt::Unchecked : Qt::Checked);
  item->setData(Qt::UserRole, QVariant::fromValue(pTask));
  if(atIdx < 0)
    mpTaskList->addItem(item);
  else
    mpTaskList->insertItem(atIdx, item);
  return item;
}

TaskType* CertLauncherGui::destroyTaskListItem(QListWidgetItem* pItem)
{
  auto pTask = pItem->data(Qt::UserRole).value<TaskType*>();
  delete pItem;
  return pTask;
}

void CertLauncherGui::updateTaskListItemInfo(QListWidgetItem* pItem)
{
  auto pTask = taskFromListItem(pItem);
  if(!pTask)
    return;

  for(auto p : pTask->properties())
  {
    if(!p->valid())
    {
      bool wasBlocked = mpTaskList->blockSignals(true);
      pItem->setCheckState(pTask->Skip ? Qt::Unchecked : Qt::PartiallyChecked);
      pItem->setTextColor(Qt::darkRed);
      pItem->setToolTip(toQString(p->error()));
      mpTaskList->blockSignals(wasBlocked);
      return;
    }
  }
  bool wasBlocked = mpTaskList->blockSignals(true);
  pItem->setCheckState(pTask->Skip ? Qt::Unchecked : Qt::Checked);
  pItem->setTextColor(Qt::black);
  pItem->setToolTip("");
  mpTaskList->blockSignals(wasBlocked);
}

TaskType* CertLauncherGui::taskFromListItem(QListWidgetItem* item)
{
  if(!item)
    return nullptr;
  return item->data(Qt::UserRole).value<TaskType*>();
}

void CertLauncherGui::delParmsBtn()
{
  auto pTask = taskFromListItem(mpTaskList->currentItem());
  Q_ASSERT(pTask);

  int ret = QMessageBox::question(this, "Remove Parameters", "Are you sure you want to remove these parameter files?",
    QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

  if (ret == QMessageBox::Cancel)
    return;

  int p = 0;
  while (p < mpParmList->count()){
    if (mpParmList->item(p)->isSelected()){
      pTask->delParm(p);
      delete mpParmList->takeItem(p);
    }
    else
      p++;
  }
}

void CertLauncherGui::onTaskListItemSelectionChanged()
{
  mpDelTaskBtn->setEnabled(!mpTaskList->selectedItems().empty());
  mpCopyTaskBtn->setEnabled(mpTaskList->selectedItems().size() == 1);
  mpTaskUpBtn->setEnabled(mpCopyTaskBtn->isEnabled() && mpTaskList->currentRow() > 0);
  mpTaskDownBtn->setEnabled(mpCopyTaskBtn->isEnabled() && mpTaskList->currentRow() < mpTaskList->count() - 1);
}

void CertLauncherGui::onTaskListItemChanged(QListWidgetItem* item)
{ // will always toggle the item's Skip data property
  auto pTask = taskFromListItem(item);
  pTask->Skip.setValue(!pTask->Skip);
  updateTaskListItemInfo(item);
  if(item->isSelected())
  {
    auto selectedItems = mpTaskList->selectedItems();
    for(auto p : selectedItems)
    {
      if(p != item)
      {
        taskFromListItem(p)->Skip = pTask->Skip;
        updateTaskListItemInfo(p);
      }
    }
  }
}

void CertLauncherGui::onTaskBtnClicked()
{
  if(sender() == mpAddTaskBtn || sender() == mpCopyTaskBtn)
  {
    TaskType task;
    if(sender() == mpAddTaskBtn)
    {
      task.Name.setValue("unnamed");
    }
    else if(sender() == mpCopyTaskBtn)
    {
      auto pTask = taskFromListItem(mpTaskList->currentItem());
      Q_ASSERT(pTask);
      task = *pTask;
      task.Name.setValue(task.Name.value() + "_copy");
    }
    if(mConfig.taskByName(task.Name))
    {
      std::ostringstream oss;
      int i = 1;
      do
      {
        oss.str("");
        oss << task.Name.value() << ++i << std::flush;
      } while(mConfig.taskByName(oss.str()));
      task.Name.setValue(oss.str());
    }
    auto pos = mConfig.Tasks.value().end();
    auto row = mpTaskList->currentRow();
    if(row >= 0)
      pos = mConfig.Tasks.value().begin() + row;
    pos = mConfig.Tasks.value().insert(pos, task);
    auto item = createTaskListItem(&*pos, row);
    updateTaskListItemInfo(item);
    mpTaskList->scrollToItem(item);
    mpTaskList->clearSelection();
    mpTaskList->setCurrentItem(item);
  }
  else if(sender() == mpDelTaskBtn)
  {
    auto selection = mpTaskList->selectedItems();
    int answer = QMessageBox::Cancel;
    if(selection.size() == 1)
    {
      answer = QMessageBox::warning(this, "Delete selected item?",
        QString("You are about to delete task \"%1\".\nProceed?").arg(selection.first()->text()),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    }
    else if(!selection.empty())
    {
      answer = QMessageBox::warning(this, "Delete selected items?",
        QString("You are about to delete %1 entries from the list of tasks.\nProceed?").arg(selection.size()),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    }
    if(answer == QMessageBox::Ok)
    {
      for(auto item : selection)
      {
        auto pTask = destroyTaskListItem(item);
        mConfig.remove(pTask);
      }
    }
  }
  else if (sender() == mpTaskUpBtn)
  {
    moveTask(-1);
  }
  else if (sender() == mpTaskDownBtn)
  {
    moveTask(1);
  }
}

void CertLauncherGui::moveTask(int dir)
{
  int row = mpTaskList->currentRow();
  if(row + dir < 0)
    return;
  if(row + dir >= mpTaskList->count())
    return;
  auto item = mpTaskList->takeItem(row);
  mpTaskList->insertItem(row + dir, item);
  mConfig.move(taskFromListItem(item), dir);
  mpTaskList->setCurrentItem(item);
  mpTaskList->scrollToItem(item);
}

void CertLauncherGui::updateTaskDetails()
{
  mpParmList->clear();
  for(auto field : mpTaskDetails->findChildren<LabeledLineEdit*>())
  {
    setInfoState(field->lineEdit(), Property::EmptyValid, "");
    field->setText("", true);
  }
  for (auto field : mpTaskDetails->findChildren<LabeledComboBox*>())
  {
    setInfoState(field->comboBox(), Property::EmptyValid, "");
    field->setText("", true);
  }
  for (auto field : mpTaskDetails->findChildren<QCheckBox*>())
  {
    auto b = field->blockSignals(true);
    field->setChecked(false);
    field->blockSignals(false);
  }

  auto pCurTask = taskFromListItem(mpTaskList->currentItem());
  mpTaskDetails->setEnabled(pCurTask);
  if(!pCurTask)
    return;

  mpTaskNameBox->setText(pCurTask->Name.value());
  for (int i=0; i < pCurTask->ParmFiles.value().size(); i++){
    QListWidgetItem *item = new QListWidgetItem(toQString(pCurTask->getParmFilesDisp()[i]));
    item->setData(Qt::UserRole, QString(pCurTask->ParmFiles.value()[i]));
    mpParmList->addItem(item);
  }

  for(auto field : mpTaskDetails->findChildren<LabeledLineEdit*>())
  {
    auto p = propertyFromWidget(field->lineEdit());
    if(p)
    {
      field->setText(p->toString(), true);
      setInfoState(field->lineEdit(), p->state(), toQString(p->error()));
    }
  }
  for (auto field : mpTaskDetails->findChildren<LabeledComboBox*>())
  {
    auto p = propertyFromWidget(field->comboBox());
    if (p)
    {
      field->setText(p->toString(), true);
      setInfoState(field->comboBox(), p->state(), toQString(p->error()));
    }
  }
  for (auto field : mpTaskDetails->findChildren<QCheckBox*>())
  {
    auto p = propertyFromWidget(field);
    if (p)
    {
      auto b = field->blockSignals(true);
      field->setChecked(p->toString() == "1");
      field->blockSignals(b);
    }
  }
}

void CertLauncherGui::onTabChanged()
{
  auto pTabWidget = dynamic_cast<QTabWidget*>(sender());
  QWidget* pCurrentWidget = pTabWidget ? pTabWidget->currentWidget() : nullptr;
  if(pCurrentWidget && pCurrentWidget->isAncestorOf(mpStartBtn))
  {
    bool canStart = mConfig.activeTasks() > 0;
    mpStartBtn->setEnabled(canStart);
    updateAnalyzeBtn();
  }
}

void CertLauncherGui::onStartBtnClicked()
{
  Assert(!mConfig.Tasks.empty());
  mpStartBtn->setEnabled(false);
  mpAnalyzeBtn->setEnabled(false);
  mpProgressDialog->hide();
  bool ok = mLauncher.run(&mConfig);
  Q_ASSERT(ok);
}

void
CertLauncherGui::onLauncherEvent()
{
  auto arg = mLauncher.Message().Argument<CertLauncher::MessageArg*>();
  Tiny::String error = arg->error;
  QMetaObject::invokeMethod(this, "onLauncherEvent",
    Q_ARG(const Config*, arg->pConfig), Q_ARG(const TaskType*, arg->pTask),
    Q_ARG(int, arg->remaining), Q_ARG(QString, toQString(error))
  );
}

void
CertLauncherGui::onLauncherEvent(const Config* pTasks, const TaskType* pTask, int remaining, QString error)
{
  if(!mpProgressDialog->isVisible())
  {
    mpProgressDialog->setRange(0, remaining);
    mpProgressDialog->show();
  }
  int numTasks = mpProgressDialog->maximum(), currentTask = numTasks - remaining + 1;
  mpProgressDialog->setValue(currentTask - 1);
  if(pTask)
  {
    mpProgressDialog->setWindowTitle(QString("Running Task %1 of %2 ...").arg(currentTask).arg(numTasks));
    mpProgressDialog->setLabelText(toQString(pTask->Name.value()));
  }
  if(!error.isEmpty())
  {
    mLauncher.kill();
    QMessageBox::warning(this, "Launcher error", error);
  }
  if(!pTask || !error.isEmpty())
  {
    mpProgressDialog->setRange(0, 0);
    mpProgressDialog->hide();
    mpStartBtn->setEnabled(true);
  }
  updateAnalyzeBtn();
}

void
CertLauncherGui::onProgressDialogCanceled()
{
  mLauncher.kill();
}

void CertLauncherGui::onAnalyzeBtnClicked()
{
  String exe = FileUtils::ExtractDirectory(FileUtils::ExecutablePath()) + "BCI2000CertAnalysis",
         dir = FileUtils::AbsolutePath(mConfig.DataDir),
         args = " --data \"" + dir + "\"";
  if(!FileUtils::IsFile(exe) && FileUtils::IsFile(exe + ".exe"))
    exe += ".exe";
  if(!FileUtils::IsFile(exe))
  {
    QMessageBox::warning(this, "Analysis failed", QString("%1 does not exist").arg(toQString(exe)));
    return;
  }
  int result = 0;
  if(!ProcessUtils::ExecuteAsynchronously(exe, args, result))
  {
    QMessageBox::warning(this, "Analysis failed", QString("Could not execute %1 %2").arg(toQString(exe)).arg(toQString(args)));
    return;
  }
}

void CertLauncherGui::updateAnalyzeBtn()
{
  bool containsData = directoryContainsExtension(mpDataDirBox->lineEdit()->text().toUtf8().constData(), ".dat", 2);
  mpAnalyzeBtn->setEnabled(containsData);
}

void CertLauncherGui::onTaskNameTextChanged(const QString& text)
{
  auto lineEdit = dynamic_cast<QLineEdit*>(sender());
  Q_ASSERT(lineEdit);
  auto& name = taskFromListItem(mpTaskList->currentItem())->Name;
  name.setValue(toString(text));
  bool wasBlocked = mpTaskList->blockSignals(true);
  mpTaskList->currentItem()->setText(text);
  mpTaskList->blockSignals(wasBlocked);
  for(int i = 0; i < mpTaskList->count(); ++i)
    updateTaskListItemInfo(mpTaskList->item(i));
  setInfoState(lineEdit, name.state(), toQString(name.error()));
}

Property::BasicProperty*
CertLauncherGui::propertyFromWidget(const QWidget* pWidget, QWidget** ppParent)
{
  Property::BasicProperty* p = nullptr;
  if(ppParent)
    *ppParent = nullptr;
  if(!pWidget)
    return p;

  auto prop = pWidget->property("Tasks");
  if(prop.isValid())
  {
    p = mConfig.property(toString(prop.toString()));
    Q_ASSERT(p);
    if(ppParent)
      *ppParent = mpGlobalGroup;
  }
  else
  {
    auto pTask = taskFromListItem(mpTaskList->currentItem());
    prop = pWidget->property("TaskType");
    if(prop.isValid() && pTask)
    {
      p = pTask->property(toString(prop.toString()));
      Q_ASSERT(p);
      if(ppParent)
        *ppParent = mpTaskDetails;
    }
  }
  return p;
}

void CertLauncherGui::onWidgetTextChanged(const QString& text)
{
  QWidget* pParent = nullptr, *pWidget = dynamic_cast<QWidget*>(sender());
  auto p = propertyFromWidget(pWidget, &pParent);
  if(p)
  {
    p->set(toString(text.trimmed()));
    if(pParent)
    {
      for(auto field : pParent->findChildren<QLineEdit*>())
        updateWidgetInfo(field);
      for (auto field : pParent->findChildren<QComboBox*>())
        updateWidgetInfo(field);
      if(pParent == mpTaskDetails)
        updateTaskListItemInfo(mpTaskList->currentItem());
    }
    else if(pWidget)
    {
      updateWidgetInfo(pWidget);
    }
  }
}

void CertLauncherGui::onCheckBoxStateChanged(int state)
{
  QWidget* pParent = nullptr, *pWidget = dynamic_cast<QWidget*>(sender());
  auto p = propertyFromWidget(pWidget, &pParent);
  Q_ASSERT(pWidget);
  if (p)
  {
    p->set(state > 0 ? "1" : "0");
    if (pParent == mpTaskDetails)
      updateTaskListItemInfo(mpTaskList->currentItem());
  }
}

void CertLauncherGui::updateWidgetInfo(QWidget* pWidget)
{
  auto p = propertyFromWidget(pWidget);
  if(p)
    setInfoState(pWidget, p->state(), toQString(p->error()));
}

void CertLauncherGui::setInfoState(QWidget *w, int c, const QString& s)
{
  QPalette p = w->palette();
  if(c & Property::Invalid)
      p.setColor(QPalette::Normal, QPalette::Base, sErrorColor);
  else if(c & Property::NotEmpty)
      p.setColor(QPalette::Normal, QPalette::Base, sOKColor);
  else
      p.setColor(QPalette::Normal, QPalette::Base, sDefaultColor);
  w->setPalette(p);
  w->setToolTip(s);
}

