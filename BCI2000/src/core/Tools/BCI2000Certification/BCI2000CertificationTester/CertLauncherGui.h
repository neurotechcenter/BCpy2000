////////////////////////////////////////////////////////////////////////////////
// $Id: CertLauncherGui.h 5814 2018-11-06 14:51:57Z mellinger $
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
#ifndef CERT_LAUNCHER_GUI_H
#define CERT_LAUNCHER_GUI_H
#include <QtWidgets>
#include "StringUtils.h"
#include "Runnable.h"
#include "TaskType.h"
#include "CertLauncher.h"

namespace {
  QString toQString(const std::string& s) { return QString::fromLocal8Bit(s.c_str()); }
  std::string toString(const QString& s) { return std::string(s.toLocal8Bit().constData()); }
}

class LineEditWrapper
{
protected:
  LineEditWrapper(QLineEdit* p) : mpEdit(p) {}
public:
  QLineEdit* lineEdit() { return mpEdit; }
  void setText(const Tiny::String& txt, bool blockSignals = false)
  {
    bool wasBlocked = mpEdit->blockSignals(blockSignals);
    mpEdit->setText(toQString(txt));
    mpEdit->blockSignals(wasBlocked);
  }
  Tiny::String text() const
  { return toString(mpEdit->text()); }
  void setProperty(const char* name, const QVariant& value)
  { mpEdit->setProperty(name, value); }
  QVariant property(const char* name) const
  { return mpEdit->property(name); }
private:
  QLineEdit* mpEdit;
};

class LabeledLineEdit : public QWidget, public LineEditWrapper
{
  Q_OBJECT
public:
  using LineEditWrapper::property;
  using LineEditWrapper::setProperty;

  LabeledLineEdit(QString labelName)
  : LineEditWrapper(new QLineEdit(this))
  {
    QVBoxLayout *layout = new QVBoxLayout;
    auto label = new QLabel(labelName);
    layout->addWidget(label);
    layout->addWidget(lineEdit());
    this->setLayout(layout);
  }
};

class GetFileWidget : public QWidget, public LineEditWrapper
{
  Q_OBJECT
public:
  using LineEditWrapper::property;
  using LineEditWrapper::setProperty;

  GetFileWidget(QString labelName, QString fileDlgTxt, QString fileFilter)
  : LineEditWrapper(new QLineEdit(this))
  {
    mFileDlgTxt = fileDlgTxt;
    mFileFilter = fileFilter;
    mFileName = "";
    QGridLayout *layout = new QGridLayout;
    this->setLayout(layout);
    mGetFileBtn = new QPushButton("...");
    layout->addWidget(new QLabel(labelName),0,0,1,1);
    layout->addWidget(lineEdit(),1,0,1,3);
    layout->addWidget(mGetFileBtn,1,3,1,1);
    connect (mGetFileBtn, SIGNAL(clicked()), this, SLOT(getFile()));
  }

  const QString& getFileName(){return mFileName;}

public slots:
  void getFile()
  {
    QString fileName = QFileDialog::getOpenFileName(this, mFileDlgTxt, NULL, mFileFilter);
    if (fileName.size() == 0)
      return;

    mFileName = fileName;
    lineEdit()->setText(mFileName);
  }
  void setFile(QString file)
  {
    mFileName = file;
    lineEdit()->setText(mFileName);
  }
  QString getCurrentFile()
  {
    return mFileName;
  }

private:
  QPushButton *mGetFileBtn;

  QString mFileName;
  QString mFileDlgTxt;
  QString mFileFilter;
};

class GetDirWidget : public QWidget, public LineEditWrapper
{
  Q_OBJECT
public:
  using LineEditWrapper::property;
  using LineEditWrapper::setProperty;

  GetDirWidget(QString labelName, QString fileDlgTxt)
  : LineEditWrapper(new QLineEdit(this))
  {
    mFileDlgTxt = fileDlgTxt;
    mFileName = "";
    QGridLayout *layout = new QGridLayout;
    this->setLayout(layout);
    mGetFileBtn = new QPushButton("...");
    layout->addWidget(new QLabel(labelName),0,0,1,1);
    layout->addWidget(lineEdit(),1,0,1,3);
    layout->addWidget(mGetFileBtn,1,3,1,1);
    connect(mGetFileBtn, SIGNAL(clicked()), this, SLOT(getFile()));
  }

  const QString& getDirName(){return mFileName;}
public slots:
  void getFile()
  {
    QString fileName = QFileDialog::getExistingDirectory(this, mFileDlgTxt);
    if (fileName.size() == 0)
      return;

    mFileName = fileName;
    lineEdit()->setText(mFileName);
  }

  void setFile(QString file)
  {
    mFileName = file;
    lineEdit()->setText(mFileName);
  }

private:
  QPushButton *mGetFileBtn;

  QString mFileName;
  QString mFileDlgTxt;
  QString mFileFilter;
};

class LabeledComboBox : public QWidget
{
  Q_OBJECT
  QComboBox* mpComboBox;
public:
  LabeledComboBox(QString labelName)
  : mpComboBox(new QComboBox(this))
  {
    QVBoxLayout *layout = new QVBoxLayout;
    auto label = new QLabel(labelName);
    layout->addWidget(label);
    layout->addWidget(comboBox());
    this->setLayout(layout);
    comboBox()->setEditable(true);
  }
  QComboBox* comboBox() { return mpComboBox; }
  void setText(const Tiny::String& txt, bool blockSignals = false)
  {
    bool wasBlocked = mpComboBox->blockSignals(blockSignals);
    mpComboBox->setCurrentText(toQString(txt));
    mpComboBox->blockSignals(wasBlocked);
  }
  void addItems(const std::set<std::string>& items)
  {
    for(auto& item : items)
      mpComboBox->addItem(toQString(item));
  }
  Tiny::String text() const
  {
    return toString(mpComboBox->currentText());
  }
  void setProperty(const char* name, const QVariant& value)
  {
    mpComboBox->setProperty(name, value);
  }
  QVariant property(const char* name) const
  {
    return mpComboBox->property(name);
  }
};

class AutoEditMenu : public QMenu
{
  Q_OBJECT

public:
  AutoEditMenu(QWidget* parent)
  : QMenu("&Edit", parent), mpMenu(nullptr)
  {
    static const struct { const char* text; int shortcut; }
    actions[] = {
      { "&Undo", QKeySequence::Undo },
      { "&Redo", QKeySequence::Redo },
      { "", 0 },
      { "Cu&t", QKeySequence::Cut },
      { "&Copy", QKeySequence::Copy },
      { "&Paste", QKeySequence::Paste },
      { "&Delete", QKeySequence::Delete },
      { "", 0 },
      { "Select &All", QKeySequence::SelectAll },
    };
    mpDefaultMenu = new QMenu(this);
    for(auto& a : actions)
    {
      auto p = mpDefaultMenu->addAction(a.text);
      p->setShortcuts(QKeySequence::StandardKey(a.shortcut));
      p->setEnabled(false);
    }
    connect(this, SIGNAL(aboutToShow()), this, SLOT(onShow()));
  }

signals:
   void populate(QWidget*, QList<QAction*>&);

private slots:
  void onShow()
  {
    clear();
    delete mpMenu;
    QList<QAction*> items;
    auto pEdit = dynamic_cast<QLineEdit*>(parentWidget()->focusWidget());
    if(pEdit)
    {
      mpMenu = pEdit->createStandardContextMenu();
    }
    else
    {
      mpMenu = new QMenu(this);
      mpMenu->addActions(mpDefaultMenu->actions());
    }
    items = mpMenu->actions();
    emit populate(parentWidget()->focusWidget(), items);
    addActions(items);
  }
private:
  QMenu* mpMenu, *mpDefaultMenu;
};

class CertLauncherGui : public QMainWindow
{
  Q_OBJECT

public:
  CertLauncherGui();
  ~CertLauncherGui();

private:
  void initUi();
  void initVals();
  void saveSettings();

  bool openIni(const QString&);
  void saveIni(const QString&);

  //task list
  QListWidget *mpTaskList;
  QAction* mpDelTaskActn;
  QAction* mpSelectAllTasksActn;
  QPushButton *mpAddTaskBtn;
  QPushButton *mpDelTaskBtn;
  QPushButton *mpCopyTaskBtn;
  QPushButton *mpTaskUpBtn, *mpTaskDownBtn;

  //task details
  QGroupBox* mpTaskDetails;
  LabeledLineEdit *mpTaskNameBox;
  QListWidget *mpParmList;
  QPushButton *mpAddParmBtn;
  QPushButton *mpDelParmBtn;

  //controls
  QPushButton *mpStartBtn;
  QPushButton *mpAnalyzeBtn;
  QCheckBox *mpConfigOnStartChk;
  QCheckBox *mpRunOnStartChk;
  QCheckBox *mpQuitOnSuspendChk;

  //global
  QGroupBox* mpGlobalGroup;
  LabeledLineEdit *mpWindowLeft;
  LabeledLineEdit *mpWindowTop;
  LabeledLineEdit *mpWindowWidth;
  LabeledLineEdit *mpWindowHeight;
  LabeledComboBox *mpDefaultSourceBox;
  GetDirWidget *mpDataDirBox;
  GetDirWidget *mpRootDirBox;
  LabeledLineEdit *mpAppAddArguments;
  LabeledLineEdit *mpExecuteBefore;
  LabeledLineEdit *mpExecuteAfter;

private slots:
  void onFileOpen();
  void onFileSave();
  void onFileSaveAs();
  void onFileClose();
  void onHelpHelp();
  void onHelpAbout();

  void onEditMenuPopulate(QWidget*, QList<QAction*>&);

  void addParmsBtn();
  void delParmsBtn();

  void onTaskListItemChanged(QListWidgetItem*);
  void onTaskListItemSelectionChanged();
  void onTaskBtnClicked();
  void onTaskNameTextChanged(const QString&);
  void onWidgetTextChanged(const QString&);
  void onDataDirTextChanged(const QString&) { updateAnalyzeBtn(); }
  void onCheckBoxStateChanged(int);
  void updateTaskDetails();

  void onTabChanged();
  void onStartBtnClicked();
  void onProgressDialogCanceled();
  void onLauncherEvent(const Config*, const TaskType*, int, QString);
  void onAnalyzeBtnClicked();

protected:
  void closeEvent(QCloseEvent *);

private:
  void updateWindowTitle();
  void moveTask(int);
  void updateAnalyzeBtn();
  bool allowDestructiveAction();
  void onLauncherEvent();

  QListWidgetItem* createTaskListItem(TaskType* task, int atIdx);
  TaskType* destroyTaskListItem(QListWidgetItem*);
  void updateTaskListItemInfo(QListWidgetItem*);
  TaskType* taskFromListItem(QListWidgetItem*);

  Property::BasicProperty* propertyFromWidget(const QWidget*, QWidget** = nullptr);
  void updateWidgetInfo(QWidget*);
  void setInfoState(QWidget *, int, const QString&);

private: //data members
  Config mConfig;
  QProgressDialog* mpProgressDialog;
  CertLauncher mLauncher;
  MemberCall<void(CertLauncherGui*)> mOnLauncherEvent;
};
#endif // CERT_LAUNCHER_GUI_H
