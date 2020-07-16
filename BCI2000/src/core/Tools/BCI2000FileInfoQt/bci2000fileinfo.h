//////////////////////////////////////////////////////////////////////
// $Id: bci2000fileinfo.h 4607 2013-10-14 13:18:08Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: The main window of BCI2000FileInfo.
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
///////////////////////////////////////////////////////////////////////
#ifndef BCI2000FILEINFO_H
#define BCI2000FILEINFO_H

#include <QMainWindow>
#include "BCI2000FileReader.h"

namespace Ui
{
    class BCI2000FileInfo;
}

class BCI2000FileInfo : public QMainWindow
{
    Q_OBJECT

public:
    BCI2000FileInfo(QWidget *parent = 0);
    ~BCI2000FileInfo();

protected:
    // Drag methods
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void GetParameters();

private slots:
    // Buttons
    void on_parmButton_clicked();
    void on_actionOpen_triggered();
    void on_actionAbout_triggered();
    void on_actionBCI2000_Help_triggered();

private:
    void OpenFile( QString filename );
    void WriteSettings() const;
    void ReadSettings();

    Ui::BCI2000FileInfo *ui;
    BCI2000FileReader mFile;
    ParamList         mParameters;
};

#endif // BCI2000FILEINFO_H
