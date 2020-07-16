////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: Kienan Knight-Boehm (kienan {at} kienankb.com)
// Description: NihonKohdenADC header
//   
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
#ifndef INCLUDED_NIHONKOHDEN_ADC_H
#define INCLUDED_NIHONKOHDEN_ADC_H

#include "BufferedADC.h"
#include "EegDataSource.h"

class NihonKohdenADC : public BufferedADC
{
 public:
           NihonKohdenADC();
  virtual ~NihonKohdenADC();
  virtual void OnPublish();
  virtual void OnAutoConfig();
  virtual void OnPreflight( SignalProperties& Output ) const;
  virtual void OnInitialize( const SignalProperties& Output );
  virtual void OnStartAcquisition();
  virtual void DoAcquire( GenericSignal& Output );
  virtual void OnStopAcquisition();

 private:
  static void CALLBACK DataSourceStateChanged( int nState, int nSubState, void * pAddInfo );
  bool NKErrorCheck( int val ) const;

  unsigned long Connect( const std::string &deviceAddress ) const;
  void Disconnect( unsigned long identifier ) const;

  bool mAutoCh;
  std::map< int, int > mChannelIndices;
  unsigned long mIdentifier;
  unsigned int mBufferChannels;
  int mNumberOfSignalChannels;
  float* mpBuffer;
  unsigned int mChannelCount;

};

#endif // INCLUDED_NIHONKOHDEN_ADC_H
