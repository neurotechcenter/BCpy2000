////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@neurotechcenter.org
// Description: Implementation of the QAbstractItemModel interface
//   displaying AnalysisEngine results.
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
#ifndef RESULTS_TREE_MODEL_H
#define RESULTS_TREE_MODEL_H

#include <QAbstractItemModel>
#include "AnalysisEngine.h"

class QMenu;

class ResultsTreeModel : public QAbstractItemModel
{
public:
    explicit ResultsTreeModel(const AnalysisEngine::Results&, QObject* parent = 0);
    ~ResultsTreeModel();

    QVariant data(const QModelIndex&, int) const override;
    Qt::ItemFlags flags(const QModelIndex&) const override;
    QVariant headerData(int, Qt::Orientation, int) const override;
    QModelIndex index(int, int, const QModelIndex&) const override;
    QModelIndex parent(const QModelIndex&) const override;
    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    bool canFetchMore(const QModelIndex&) const override;
    void fetchMore(const QModelIndex&) override;

    QMenu* contextMenu(const QModelIndex&) const;

private:
    struct Private;
    Private* p;
};

#endif // RESULTS_TREE_MODEL_H
