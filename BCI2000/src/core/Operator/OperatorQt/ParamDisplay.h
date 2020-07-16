//////////////////////////////////////////////////////////////////////
// $Id: ParamDisplay.h 5175 2015-12-16 17:21:51Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A ParamDisplay class that handles a single parameter's
//       associated user interface, consisting of a group of user
//       interface widgets arranged inside a parent widget.
//       ParamDisplay is the interface to the outside world;
//       actual behavior is implemented by a class hierarchy
//       descending from a "DisplayBase" class.
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
#ifndef PARAM_DISPLAY_H
#define PARAM_DISPLAY_H

#include <QWidget>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>

#include <map>
#include <set>
#include <string>

#include "Param.h"
class ParsedComment;
class DisplayBase;

class ParamDisplay  // This class is the interface to the outside world.
{
  public:
    ParamDisplay();
    ParamDisplay( const Param&, QWidget* );
    ParamDisplay( const ParamDisplay& );
    ~ParamDisplay();
    const ParamDisplay& operator=( const ParamDisplay& );

    void SetTop( int top );
    void SetLeft( int left );
    int  GetBottom();
    int  GetRight();
    void Hide();
    void Show();

    void WriteValuesTo( Param& ) const;
    void ReadValuesFrom( const Param& );
    bool Modified() const;

  private:
    DisplayBase* mpDisplay;
};

// The ParamLabel class exists to allow for custom
// "WhatsThis" behavior by overriding QLabel::event().
class ParamLabel : public QLabel
{
  Q_OBJECT

 public:
  ParamLabel( QWidget*, const ParsedComment& );
  virtual ~ParamLabel() {}

 protected:
  virtual bool event( QEvent* );

 private:
  std::string mParamName,
              mHelpContext;
};

// The class hierarchy descending from DisplayBase
// contains implementations for various
// kinds of parameters.
class DisplayBase : public QObject
{
  friend class ParamDisplay; // access to mRefCount

  Q_OBJECT

 public:
  enum
  {
    leftMargin_ = 0,
    topMargin_,

    labelsOffsetX_,
    labelsOffsetY_,
    commentOffsetX_,
    commentOffsetY_,
    valueOffsetX_,
    valueOffsetY_,
    valueWidth_,
    valueHeight_,
    buttonWidth_,
    buttonHeight_,
    buttonSpacingX_,
    userlevelOffsetX_,
    userlevelWidth_,
    userlevelOffsetY_,
    userlevelHeight_,
    totalWidth_,

    measuresCount,
  };
  static int Measures[];
  static bool InitMeasures( float inDpi = 0 );

 protected:
  DisplayBase( const ParsedComment&, QWidget* );

 public:
  virtual ~DisplayBase();

  void SetTop( int );
  void SetLeft( int );
  int  GetBottom();
  int  GetRight();
  void Show();
  void Hide();

  virtual void WriteValuesTo( Param& ) const;
  virtual void ReadValuesFrom( const Param& );
  bool Modified() const { return mModified; }

 protected:
  void AddWidget( QWidget* c )
  { mWidgets.insert( c ); }

 public slots:
  void OnContentChange()
  { mModified = true; }

 private:
  typedef std::set<QWidget*>        WidgetContainer;
  typedef WidgetContainer::iterator WidgetIterator;
  WidgetContainer                   mWidgets;

  mutable int mRefCount;
  bool        mModified;
  int         mTop,
              mLeft;
  QSlider*    mpUserLevel;
};

// This is the base class for all displays where there is a separate label
// holding the "comment" part of the parameter.
class SeparateComment : public DisplayBase
{
  Q_OBJECT
 protected:
  SeparateComment( const ParsedComment&, QWidget* );
};

// This is the base class for all displays that contain an edit field
// holding parameter values.
class SingleEntryEdit : public SeparateComment
{
  Q_OBJECT
 public:
  SingleEntryEdit( const ParsedComment&, QWidget* );
  virtual void WriteValuesTo( Param& ) const;
  virtual void ReadValuesFrom( const Param& );
 private slots:
  void OnEditChange();
 protected:
  QLineEdit* mpEdit;
};

// This is the base class for all displays that contain a button beside
// the edit field.
class SingleEntryButton : public SingleEntryEdit
{
  Q_OBJECT
 public:
  SingleEntryButton( const ParsedComment&, QWidget* );
 private slots:
  void OnButtonClick();
 private:
  virtual void ButtonClick() = 0;
 protected:
  std::string mComment;
};

class SingleEntryInputFile : public SingleEntryButton
{
  Q_OBJECT
 public:
  SingleEntryInputFile( const ParsedComment&, QWidget* );
 private:
  virtual void ButtonClick();
};

class SingleEntryOutputFile : public SingleEntryButton
{
  Q_OBJECT
 public:
  SingleEntryOutputFile( const ParsedComment&, QWidget* );
 private:
  virtual void ButtonClick();
};

class SingleEntryDirectory : public SingleEntryButton
{
  Q_OBJECT
 public:
  SingleEntryDirectory( const ParsedComment&, QWidget* );
 private:
  virtual void ButtonClick();
};

class SingleEntryColor : public SingleEntryButton
{
  Q_OBJECT
 public:
  SingleEntryColor( const ParsedComment&, QWidget* );
 private:
  virtual void ButtonClick();
};

class SingleEntryBlob : public SeparateComment
{
  Q_OBJECT
 public:
  SingleEntryBlob( const ParsedComment&, QWidget* );
  ~SingleEntryBlob();
  void WriteValuesTo( Param& ) const;
  void ReadValuesFrom( const Param& );
 private slots:
  void OnViewButtonClick();
  void OnLoadButtonClick();
  void OnSaveButtonClick();
 private:
  void ReadData();
  QWidget* mpViewButton, *mpSaveButton, *mpLoadButton;
  std::string mName;
  class Blob* mpData;
  const Param* mpParam;
  int mKind;
};

class List : public SingleEntryButton
{
  Q_OBJECT
 public:
  List( const ParsedComment&, QWidget* );
  virtual void WriteValuesTo( Param& ) const;
  virtual void ReadValuesFrom( const Param& );
 private:
  virtual void ButtonClick();
  void ParamToText( const Param& );
  void TextToParam( Param& ) const;
  std::string mName;
  int mKind;
};

class Matrix : public SeparateComment
{
  Q_OBJECT
 public:
  Matrix( const ParsedComment&, QWidget* );
  virtual void WriteValuesTo( Param& ) const;
  virtual void ReadValuesFrom( const Param& );
 private slots:
  void OnEditButtonClick();
  void OnLoadButtonClick();
  void OnSaveButtonClick();
 private:
  bool   mMatrixWindowOpen;
  Param  mParam;
  int mKind;
};

class SingleEntryEnum : public SeparateComment
{
  Q_OBJECT
 public:
  SingleEntryEnum( const ParsedComment&, QWidget* );
  virtual void WriteValuesTo( Param& ) const;
  virtual void ReadValuesFrom( const Param& );
 private:
  QComboBox* mpComboBox;
  int        mIndexBase;
};

class SingleEntryBoolean : public DisplayBase
{
  Q_OBJECT
 public:
  SingleEntryBoolean( const ParsedComment&, QWidget* );
  virtual void WriteValuesTo( Param& ) const;
  virtual void ReadValuesFrom( const Param& );
 private:
  QCheckBox* mpCheckBox;
};


#endif // PARAM_DISPLAY_H
