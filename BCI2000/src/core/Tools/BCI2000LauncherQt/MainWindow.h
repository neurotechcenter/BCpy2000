////////////////////////////////////////////////////////////////////////////////
// $Id: MainWindow.h 4607 2013-10-14 13:18:08Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The main window of the BCI2000Launcher tool.
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

#include "ModuleInventory.h"

class QListWidget;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_launchButton_clicked();
    void on_subjectSessionEdit_textChanged(QString );
    void on_saveDirEdit_textChanged(QString );
    void on_subjectNameEdit_textChanged(QString );
    void on_subjectSessionEdit_editingFinished();
    void on_chooseDirBtn_clicked();
    void on_removeParmfileBtn_clicked();
    void on_addParmfileBtn_clicked();
    void on_actionDown_triggered();
    void on_actionUp_triggered();
    void on_clearSelectionBtn_clicked();
    void on_actionTo_Signal_Source_triggered();
    void on_actionTo_Signal_Processing_triggered();
    void on_actionTo_Applications_triggered();
    void on_actionTo_Others_triggered();
    void on_actionBCI2000_Help_triggered();
    void on_actionAbout_triggered();
    void on_actionQuit_triggered();

private:
    void SetupProgramsGroup();
    void SetupParametersGroup();
    void SetupStatusGroup();
    void SetupActions();

    void SetupInventory();
    QStringList ScanForPrograms();
    void DistributePrograms( const QStringList& );
    void ClearProgramEntries( const QString& );
    void IntroduceNewProgram( const QString& );

    void MoveListItem( QListWidget* pTarget );
    void SaveSettings() const;
    void ReadSettings();
    void SyncToolTip();

    bool ExecuteCommand( const char* cmd, const char* wd );
    QStringList ParseCommand( const char* cmd );

private:
    QStringList mParameterFiles;
    bci::Inventory mInventory;
};

#endif // MAINWINDOW_H
