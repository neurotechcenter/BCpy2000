////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@neurotechcenter.org
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
#include "ScriptEditor.h"
#include "QtSignalConnector.h"
#include "FileUtils.h"
#include <QTextDocumentFragment>
#include <QPlainTextEdit>
#include <QBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>

#include "Debugging.h"

typedef bool (ScriptEditor::Private::*onEnabledFunc)() const;
Q_DECLARE_METATYPE(onEnabledFunc);

struct ScriptEditor::Private : QPlainTextEdit, QtSignalConnector
{
  Private(QWidget* parent) : QPlainTextEdit(parent), mRows(40), mCols(80)
  {
    setFont(QFont("Courier", 8));
    setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    setTextInteractionFlags(
      Qt::TextInteractionFlag::TextSelectableByKeyboard
      | Qt::TextInteractionFlag::TextSelectableByMouse
    );
  }

  void adaptSize()
  {
    int rows = std::min(toPlainText().count('\n'), mRows);
    rows = std::max(rows, 4);
    int width = mCols * fontMetrics().width('M'),
        height = rows * fontMetrics().lineSpacing();
    setMinimumSize(width, height);
    parentWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  }

  void onMenuAboutToShow()
  {
    auto pMenuBar = dynamic_cast<QMenuBar*>(parentWidget()->layout()->menuBar());
    for(auto pMenu : pMenuBar->findChildren<QMenu*>())
    {
      for(auto pAction : pMenu->actions())
      {
        auto onEnabledProperty = pAction->property("onEnabled");
        if(onEnabledProperty.canConvert<onEnabledFunc>())
        {
          auto onEnabled = onEnabledProperty.value<onEnabledFunc>();
          pAction->setEnabled((this->*onEnabled)());
        }
      }
    }
  }

  bool hasSelection() const { return textCursor().hasSelection(); }
  bool isEditable() const { return textInteractionFlags() & Qt::TextInteractionFlag::TextEditable; }
  bool canCut() const { return hasSelection() && isEditable(); }

  void fileOpen(const std::string& fileNameUtf8)
  {
    QString desc = parentWidget()->windowTitle();
    QString path;
    if(!fileNameUtf8.empty())
      path = QString::fromUtf8(FileUtils::AbsolutePath(fileNameUtf8).c_str());
    QString filter = QString::fromUtf8(FileUtils::ExtractExtension(fileNameUtf8).c_str());
    if(!filter.isEmpty())
      filter = "*" + filter;
    path = QFileDialog::getOpenFileName(this, "Open " + desc, path, filter);
    if(!path.isEmpty())
    {
      QFile file(path);
      if(file.open(QIODevice::ReadOnly | QIODevice::Text))
      {
        auto content = file.readAll();
        setPlainText(QString::fromUtf8(content.constData(), content.length()));
        mFileNameUtf8 = path.toUtf8().constData();
      }
      else
      {
        QMessageBox::critical(this, "Error",  "Could not read\n\t" + desc + "\nfrom\n\t" + path);
      }
    }
  }

  void onFileOpen()
  {
    fileOpen(mFileNameUtf8);
  }

  void onFileSaveAs()
  {
    auto desc = parentWidget()->windowTitle();
    auto path = QString::fromUtf8(FileUtils::AbsolutePath(mFileNameUtf8).c_str());
    auto filter = "*" + QString::fromUtf8(FileUtils::ExtractExtension(mFileNameUtf8).c_str());
    path = QFileDialog::getSaveFileName(this, "Save " + desc, path, filter);
    if(!path.isEmpty())
    {
      QFile file(path);
      if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
      {
        file.write(toPlainText().toUtf8().constData());
        mFileNameUtf8 = path.toUtf8().constData();
      }
      else
      {
        QMessageBox::critical(this, "Error",  "Could not write\n\t" + desc + "\nto\n\t" + path);
      }
    }
  }

  void onFileClose() { parentWidget()->close(); }
  void onEditSelectAll() { selectAll(); }
  void onEditCopy() { copy(); }
  void onEditCut() { cut(); }
  void onEditPaste() { paste(); }

  int mRows, mCols;
  std::string mFileNameUtf8;
};

ScriptEditor::ScriptEditor(QWidget* parent)
: QWidget(parent), p(new Private(this))
{
  auto* pLayout = new QHBoxLayout;
  pLayout->setMargin(0);
  pLayout->addWidget(p);
  setLayout(pLayout);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  p->adaptSize();
  connect(p, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
}

void ScriptEditor::showMenuBar()
{
  static const struct
  { 
    const char* text;
    int shortcut;
    void (Private::*onTriggered)();
    bool (Private::*onEnabled)() const;
  } actions[] =
  {
    { "&File" },
    { "&Open...", Qt::ControlModifier | Qt::Key_O, &Private::onFileOpen, &Private::isEditable },
    { "&Save as...", Qt::ControlModifier | Qt::Key_S, &Private::onFileSaveAs },
    { "&Close", Qt::ControlModifier | Qt::Key_W, &Private::onFileClose },
    { nullptr },
    { "&Edit" },
    { "Select &all...", Qt::ControlModifier | Qt::Key_A, &Private::onEditSelectAll },
    { "Cu&t", Qt::ControlModifier | Qt::Key_X, &Private::onEditCut, &Private::canCut },
    { "&Copy", Qt::ControlModifier | Qt::Key_C, &Private::onEditCut, &Private::hasSelection },
    { "&Paste", Qt::ControlModifier | Qt::Key_V, &Private::onEditPaste, &Private::canPaste },
    { nullptr },
  };
  auto* pMenuBar = new QMenuBar;
  QMenu* pMenu = nullptr;
  for(auto action : actions)
  {
    if(!action.text)
      pMenu = nullptr;
    else if(!pMenu)
    {
      pMenu = pMenuBar->addMenu(action.text);
      p->Connect(pMenu, SIGNAL(aboutToShow()), MemberCall<void (Private*)>(&Private::onMenuAboutToShow, p));
    }
    else
    {
      auto pAction = pMenu->addAction(action.text);
      pAction->setShortcut(action.shortcut);
      if(action.onEnabled)
        pAction->setProperty("onEnabled", QVariant::fromValue(action.onEnabled));
      p->Connect(pAction, SIGNAL(triggered()), MemberCall<void (Private*)>(action.onTriggered, p));
    }
  }
  layout()->setMenuBar(pMenuBar);
}

ScriptEditor::~ScriptEditor()
{
}

void ScriptEditor::open(const std::string& fileUtf8)
{
  p->fileOpen(fileUtf8);
}

void ScriptEditor::saveAs()
{
  p->onFileSaveAs();
}

void ScriptEditor::close()
{
  p->onFileClose();
}

void ScriptEditor::setFileNameUtf8(const std::string& s)
{
  p->mFileNameUtf8 = s;
}

const std::string& ScriptEditor::fileNameUtf8() const
{
  return p->mFileNameUtf8;
}

void ScriptEditor::setContentUtf8(const std::string& s)
{
  p->setPlainText(QString::fromUtf8(s.c_str()));
  p->adaptSize();
}

std::string ScriptEditor::contentUtf8() const
{
  return p->toPlainText().toUtf8().data();
}

void ScriptEditor::setRows(int rows)
{
  p->mRows = rows;
  p->adaptSize();
}

int ScriptEditor::rows() const
{
  return p->mRows;
}

void ScriptEditor::setCols(int cols)
{
  p->mCols = cols;
  p->adaptSize();
}

int ScriptEditor::cols() const
{
  return p->mCols;
}

void ScriptEditor::setEditable(bool b)
{
  auto editableFlags = Qt::TextInteractionFlag::TextEditable
          | Qt::TextInteractionFlag::TextEditorInteraction;
  auto f = p->textInteractionFlags();
  if(b)
    f |= editableFlags;
  else
    f &= ~editableFlags;
  p->setTextInteractionFlags(f);
}

bool ScriptEditor::isEditable() const
{
  return p->isEditable();
}
