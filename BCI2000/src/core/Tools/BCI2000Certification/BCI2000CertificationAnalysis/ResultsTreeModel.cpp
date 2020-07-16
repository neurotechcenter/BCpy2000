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
#include "ResultsTreeModel.h"
#include "Gui/ScriptEditor.h"
#include "AnalysisEngine/Objects/Statistics.h"
#include "FileUtils.h"
#include "QtSignalConnector.h"
#include <QBrush>
#include <QMenu>
#include <QApplication>
#include <QFileDialog>

namespace {
  std::string objectInfo(const AnalysisEngine::Object* p, bool includeType)
  {
    std::string info = p->name();
    if(info.empty())
    {
      info = p->type();
      includeType = false;
    }
    if(includeType && p->name() == p->type())
      includeType = false;
    std::string summary = p->summary();
    if(includeType)
    {
      if(summary.empty())
        summary = p->type();
      else
        summary = p->type() + ", " + summary;
    }
    if(!summary.empty())
      info += " [" + summary + "]";
    return info;
  }

  struct Node
  {
    Node(Node* parent) : parent(parent) {}
    virtual ~Node() { for(auto p : mChildren) delete p; }
    virtual int cols() const { return 2; }
    QVariant coldata(int col, int role)
    {
      switch(role)
      {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
          return QString::fromUtf8(column(col).c_str());
        case Qt::ForegroundRole:
          return QBrush(color());
      }
      return QVariant();
    }
    virtual std::string column(int idx) const { return ""; }
    virtual QColor color() const { return Qt::black; }
    virtual int children() const { return mChildren.size(); }
    virtual Node* child(int idx) { return idx >= mChildren.size() ? nullptr : mChildren[idx]; }
    virtual void createChildren() {}
    virtual QMenu* contextMenu() { return nullptr; }
    int findChild(Node* child) const
    {
      for(int i = 0; i < mChildren.size(); ++i)
        if(mChildren[i] == child)
          return i;
      return -1;
    }
    Node* parent;
    std::vector<Node*> mChildren;
  };
  struct NamedNode : Node
  {
    NamedNode(Node* parent, const std::string& name) : Node(parent), mName(name) {}
    std::string column(int idx) const override { return idx == 0 ? mName : ""; }
    std::string mName;
  };
  struct SessionNode : Node
  {
    SessionNode::SessionNode(Node* parent)
    : Node(parent), mpData(nullptr), mState(success), mpContextMenu(nullptr)
    {
    }
    ~SessionNode()
    {
      delete mpContextMenu;
      for(auto p : mWindows)
        delete p;
    }
    void setData(const AnalysisEngine::SessionData*);
    std::string firstError() const
    {
      for(const auto& log : mpData->logs)
        if(!log.error.empty())
          return log.description + ", " + log.error;
      return "";
    }
    enum { error, success, partialSuccess, failed } mState;
    std::string column(int col) const override
    {
      if(col == 0)
        return mpData->description;
      if(col == 1) switch(mState)
      {
        case error: return firstError();
        case partialSuccess: return "missing or invalid data, not failed";
        case success: return "passed";
        case failed: return "failed";
      }
      return "";
    }
    QColor color() const override
    {
      switch(mState)
      {
        case error:
          return Qt::darkRed;
        case success:
          return Qt::black;
        case partialSuccess:
          return Qt::gray;
        case failed:
          return Qt::darkRed;
      }
      return Qt::black;
    }
    void showScript(int idx, bool debuglog)
    {
      if(mWindows.empty())
        mWindows.resize(2 * mpData->logs.size());
      auto& p = mWindows[idx + (debuglog ? mpData->logs.size() : 0)];
      if(!p)
      {
        const auto& log = mpData->logs[idx];
        p = new ScriptEditor(nullptr);
        p->setAttribute(Qt::WA_QuitOnClose, false);
        p->setFileNameUtf8(debuglog ? log.description + ".log" : log.description + ".bciscript");
        p->setContentUtf8(debuglog ? log.log : log.script);
        p->setRows(40);
        p->setCols(80);
        p->showMenuBar();
        std::string desc = mpData->description + " " + log.description + " script";
        if(debuglog)
          desc += " debug log";
        p->setWindowTitle(QString::fromUtf8(desc.c_str()));
        for(int i = 0; i <= idx; ++i)
          p->move(p->pos() + QPoint(15, 25));
      }
      p->show();
      qApp->setActiveWindow(p);
    }
    QMenu* contextMenu() override { return mpContextMenu; }
    QMenu* mpContextMenu;
    const AnalysisEngine::SessionData* mpData;
    QtSignalConnector mConnector;
    std::vector<ScriptEditor*> mWindows;
  };
  struct ObjectNode : Node
  {
    ObjectNode(const AnalysisEngine::Object* data, Node* parent)
    : Node(parent), mpData(data), mpContextMenu(nullptr) {}
    ~ObjectNode() { delete mpContextMenu; }
    std::string column(int idx) const override
    {
      if(idx == 0)
        return objectInfo(mpData, true);
      if(idx == 1)
        return mpData->description();
      return "";
    }
    QColor color() const override
    {
      if(!mpData->valid())
        return Qt::darkGray;
      return Qt::black;
    }
    int children() const override { return mpData->depends().size(); }
    void createChildren() override
    {
      while(mChildren.size() < children())
        mChildren.push_back(new ObjectNode(mpData->depends()[mChildren.size()], this));
    }
    QMenu* contextMenu() override
    {
      if(!mpContextMenu)
      {
        mpContextMenu = new QMenu(nullptr);
        auto action = mpContextMenu->addAction("plot");
        action->setEnabled(mpData->canPlot());
        if(!Dataplot::available())
          action->setToolTip("install Gnuplot to enable plotting");
        mConnector.Connect(action, SIGNAL(triggered()), MemberCall<void(ObjectNode*)>(&ObjectNode::doPlot, this));
        action = mpContextMenu->addAction("export ...");
        action->setEnabled(mpData->canExport("*.*"));
        mConnector.Connect(action, SIGNAL(triggered()), MemberCall<void(ObjectNode*)>(&ObjectNode::doExport, this));
      }
      return mpContextMenu;
    }
    void doPlot() { mpData->doPlot(); }
    void doExport()
    {
      auto name = QString::fromUtf8(mpData->name().c_str());
      auto path = QString::fromLocal8Bit(FileUtils::AbsolutePath("./").c_str()) + name + ".tsv";
      path = QFileDialog::getSaveFileName(nullptr, "save " + name, path);
      if(!path.isEmpty())
        mpData->doExport(path.toLocal8Bit().constData());
    }

    const AnalysisEngine::Object* mpData;
    QMenu* mpContextMenu;
    QtSignalConnector mConnector;
  };
  struct RequirementNode : Node
  {
    RequirementNode(const AnalysisEngine::Requirement* data, Node* parent)
    : Node(parent), mpData(data) {}
    std::string column(int idx) const override
    {
      if(idx == 0)
        return mpData->condition;
      if(idx == 1)
      {
        if(mpData->value == 0)
          return "failed";
        if(mpData->value == mpData->value)
          return "passed";
        return "missing or invalid data";
      }
      return "";
    }
    QColor color() const override
    {
      if(mpData->value == 0)
        return Qt::darkRed;
      if(mpData->value == mpData->value)
        return Qt::black;
      return Qt::darkGray;
    }
    int children() const override { return mpData->depends.size(); }
    void createChildren() override
    {
      while(mChildren.size() < children())
        mChildren.push_back(new ObjectNode(mpData->depends[mChildren.size()], this));
    }
    const AnalysisEngine::Requirement* mpData;
  };
  struct ReportNode : ObjectNode
  {
    ReportNode(const AnalysisEngine::Report* data, Node* parent)
    : ObjectNode(data->object, parent), mpReportData(data) {}
    std::string column(int col) const override
    {
      if(col == 0)
        return objectInfo(mpReportData->object, false);
      return mpReportData->description;
    }
    const AnalysisEngine::Report* mpReportData;
  };
  void SessionNode::setData(const AnalysisEngine::SessionData* pSession)
  {
    mpData = pSession;
    if(!firstError().empty())
      mState = error;
    auto pReq = new NamedNode(this, "Requirements");
    for(auto& r : mpData->requirements) {
      if(!(r.value == r.value) && mState == success)
        mState = partialSuccess;
      if(r.value == 0)
        mState = failed;
      pReq->mChildren.push_back(new RequirementNode(&r, pReq));
    }
    if(pReq->children() > 0)
      mChildren.push_back(pReq);
    auto pStat = new NamedNode(this, "Performance statistics");
    auto pProc = new NamedNode(this, "Processing stages");
    for(auto& r : mpData->reports)
    {
      if(dynamic_cast<const AnalysisDataObjects::StatisticsObject*>(r.object))
        pStat->mChildren.push_back(new ReportNode(&r, pStat));
      else
        pProc->mChildren.push_back(new ReportNode(&r, pProc));
    }
    if(pStat->children() > 0)
      mChildren.push_back(pStat);
    if(pProc->children() > 0)
      mChildren.push_back(pProc);
    mpContextMenu = new QMenu;
    auto pScripts = mpContextMenu->addMenu("show script");
    auto pLogs = mpContextMenu->addMenu("show debug log");
    int idx = 0;
    for(const auto& log : mpData->logs)
    {
      auto name = QString::fromUtf8(log.description.c_str()) + " script";
      mConnector.Connect(
        pScripts->addAction(name), SIGNAL(triggered()),
        MemberCall<void(SessionNode*, int, bool)>(&SessionNode::showScript, this, idx, false)
      );
      mConnector.Connect(
        pLogs->addAction(name), SIGNAL(triggered()),
        MemberCall<void(SessionNode*, int, bool)>(&SessionNode::showScript, this, idx, true)
      );
      ++idx;
    }
  }
}

struct ResultsTreeModel::Private : SessionNode
{
  Private(const AnalysisEngine::Results& r) : SessionNode(nullptr)
  {
    if(!r.empty())
      setData(r.back());
    auto pFiles = new NamedNode(this, "Individual tasks");
    mChildren.push_back(pFiles);
    for(int i = 0; i + 1 < r.size(); ++i)
    {
      auto p = new SessionNode(pFiles);
      p->setData(r[i]);
      pFiles->mChildren.push_back(p);
    }
  }
};

ResultsTreeModel::ResultsTreeModel(const AnalysisEngine::Results& data, QObject* parent)
: QAbstractItemModel(parent), p(new Private(data))
{
}

ResultsTreeModel::~ResultsTreeModel()
{
  delete p;
}

QVariant
ResultsTreeModel::data(const QModelIndex &index, int role) const
{
  Node* pNode = static_cast<Node*>(index.internalPointer());
  return pNode->coldata(index.column(), role);
}

Qt::ItemFlags
ResultsTreeModel::flags(const QModelIndex &index) const
{
  if(!index.isValid())
    return 0;
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);
  return flags;
}

QVariant
ResultsTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  return QAbstractItemModel::headerData(section, orientation, role);
}

QModelIndex
ResultsTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if(!hasIndex(row, column, parent))
    return QModelIndex();
  Node* pNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : p;
  if(row >= pNode->children())
    return QModelIndex();
  return createIndex(row, column, pNode->child(row));
}

QModelIndex
ResultsTreeModel::parent(const QModelIndex &index) const
{
  if(!index.isValid())
    return QModelIndex();
  Node* pNode = static_cast<Node*>(index.internalPointer());
  Node* pParent = pNode->parent;
  if(!pParent || pParent == p)
    return QModelIndex();
  int i = pParent->findChild(pNode);
  if(i < 0)
    return QModelIndex();
  return createIndex(i, 0, pParent);
}

int
ResultsTreeModel::rowCount(const QModelIndex &parent) const
{
  if(parent.column() > 0)
    return 0;
  if(!parent.isValid())
    return p->children();
  return static_cast<Node*>(parent.internalPointer())->children();
}

int
ResultsTreeModel::columnCount(const QModelIndex &parent) const
{
  if(!parent.isValid())
    return p->cols();
  return static_cast<Node*>(parent.internalPointer())->cols();
}

bool
ResultsTreeModel::canFetchMore(const QModelIndex& index) const
{
  if(!index.isValid())
    return false;
  auto p = static_cast<Node*>(index.internalPointer());
  return p->children() > p->mChildren.size();
}

void
ResultsTreeModel::fetchMore(const QModelIndex& index)
{
  if(index.isValid())
  {
    auto p = static_cast<Node*>(index.internalPointer());
    p->createChildren();
  }
}

QMenu*
ResultsTreeModel::contextMenu(const QModelIndex& index) const
{
  if(!index.isValid())
    return p->contextMenu();
  return static_cast<Node*>(index.internalPointer())->contextMenu();
}

