//////////////////////////////////////////////////////////////////////
// $Id: PrefDialog.cpp 5145 2015-11-27 12:18:20Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's preference dialog.
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
#include "PrefDialog.h"
#include "ui_PrefDialog.h"

extern Preferences* gpPreferences;

PrefDialog::PrefDialog( QWidget* parent )
: QDialog( parent ),
  m_ui( new Ui::PrefDialog )
{
  m_ui->setupUi( this );
  this->setWindowFlags( Qt::Dialog );
  this->setWindowModality( Qt::WindowModal );
  GetValues();
}

PrefDialog::~PrefDialog()
{
  delete m_ui;
}

void
PrefDialog::GetValues()
{
  m_ui->slider_UserLevel->setValue( gpPreferences->mUserLevel );

  m_ui->edit_CmdBtn1->setText( gpPreferences->mButtons[0].Cmd );
  m_ui->edit_NameBtn1->setText( gpPreferences->mButtons[0].Name );

  m_ui->edit_CmdBtn2->setText( gpPreferences->mButtons[1].Cmd );
  m_ui->edit_NameBtn2->setText( gpPreferences->mButtons[1].Name );

  m_ui->edit_CmdBtn3->setText( gpPreferences->mButtons[2].Cmd );
  m_ui->edit_NameBtn3->setText( gpPreferences->mButtons[2].Name );

  m_ui->edit_CmdBtn4->setText( gpPreferences->mButtons[3].Cmd );
  m_ui->edit_NameBtn4->setText( gpPreferences->mButtons[3].Name );

  m_ui->edit_OnConnect->setText( gpPreferences->mScript[ Preferences::AfterModulesConnected ] );
  m_ui->edit_OnConnect->setReadOnly( gpPreferences->mCmdlineSpecified[ Preferences::AfterModulesConnected ] );

  m_ui->edit_OnSetConfig->setText( gpPreferences->mScript[ Preferences::OnSetConfig ] );
  m_ui->edit_OnSetConfig->setReadOnly( gpPreferences->mCmdlineSpecified[ Preferences::OnSetConfig ] );

  m_ui->edit_OnStart->setText( gpPreferences->mScript[ Preferences::OnStart ] );
  m_ui->edit_OnStart->setReadOnly( gpPreferences->mCmdlineSpecified[ Preferences::OnStart ] );

  m_ui->edit_OnSuspend->setText( gpPreferences->mScript[ Preferences::OnSuspend ] );
  m_ui->edit_OnSuspend->setReadOnly( gpPreferences->mCmdlineSpecified[ Preferences::OnSuspend ] );

  m_ui->edit_OnResume->setText( gpPreferences->mScript[ Preferences::OnResume ] );
  m_ui->edit_OnResume->setReadOnly( gpPreferences->mCmdlineSpecified[ Preferences::OnResume ] );

  m_ui->edit_OnExit->setText( gpPreferences->mScript[ Preferences::OnExit ] );
  m_ui->edit_OnExit->setReadOnly( gpPreferences->mCmdlineSpecified[ Preferences::OnExit ] );
}

void
PrefDialog::PutValues() const
{
  gpPreferences->mUserLevel = m_ui->slider_UserLevel->value();

  gpPreferences->mButtons[0].Cmd = m_ui->edit_CmdBtn1->text();
  gpPreferences->mButtons[0].Name = m_ui->edit_NameBtn1->text();

  gpPreferences->mButtons[1].Cmd = m_ui->edit_CmdBtn2->text();
  gpPreferences->mButtons[1].Name = m_ui->edit_NameBtn2->text();

  gpPreferences->mButtons[2].Cmd = m_ui->edit_CmdBtn3->text();
  gpPreferences->mButtons[2].Name = m_ui->edit_NameBtn3->text();

  gpPreferences->mButtons[3].Cmd = m_ui->edit_CmdBtn4->text();
  gpPreferences->mButtons[3].Name = m_ui->edit_NameBtn4->text();

  if( !gpPreferences->mCmdlineSpecified[ Preferences::AfterModulesConnected ] )
    gpPreferences->mScript[ Preferences::AfterModulesConnected ] = m_ui->edit_OnConnect->text();

  if( !gpPreferences->mCmdlineSpecified[ Preferences::OnSetConfig ] )
    gpPreferences->mScript[ Preferences::OnSetConfig ] = m_ui->edit_OnSetConfig->text();

  if( !gpPreferences->mCmdlineSpecified[ Preferences::OnStart ] )
    gpPreferences->mScript[ Preferences::OnStart ] = m_ui->edit_OnStart->text();

  if( !gpPreferences->mCmdlineSpecified[ Preferences::OnSuspend ] )
    gpPreferences->mScript[ Preferences::OnSuspend ] = m_ui->edit_OnSuspend->text();

  if( !gpPreferences->mCmdlineSpecified[ Preferences::OnResume ] )
    gpPreferences->mScript[ Preferences::OnResume ] = m_ui->edit_OnResume->text();

  if( !gpPreferences->mCmdlineSpecified[ Preferences::OnExit ] )
    gpPreferences->mScript[ Preferences::OnExit ] = m_ui->edit_OnExit->text();
}

void
PrefDialog::on_okButton_clicked()
{
  PutValues();
//  close();
  QDialog::accept();
}

void
PrefDialog::on_cancelButton_clicked()
{
  //close();
  QDialog::reject();
}

void PrefDialog::on_slider_UserLevel_valueChanged( int value )
{
  switch( value )
  {
    case Preferences::Beginner:
      m_ui->label_UserLevel->setText( "Beginner" );
      break;
    case Preferences::Intermediate:
      m_ui->label_UserLevel->setText( "Intermediate" );
      break;
    case Preferences::Advanced:
      m_ui->label_UserLevel->setText( "Advanced" );
      break;
    default:
      ;
  }
}
