////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayMemo.h 5112 2015-11-18 14:35:10Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class for memo (text) type visualization displays.
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
#ifndef VIS_DISPLAY_MEMO_H
#define VIS_DISPLAY_MEMO_H

#include "VisDisplayLayer.h"

class QString;
class QTextEdit;

class VisDisplayMemo : public VisDisplayLayer
{
  Q_OBJECT

 public:
  VisDisplayMemo( const std::string& visID );
  virtual ~VisDisplayMemo();

 protected:
  void OnSetConfig( ConfigSettings& ) override;
  void OnMemo( const QString& ) override;

 private:
  int        mNumLines;
  QTextEdit* mpTextDisplay;
};

#endif // VIS_DISPLAY_MEMO_H
