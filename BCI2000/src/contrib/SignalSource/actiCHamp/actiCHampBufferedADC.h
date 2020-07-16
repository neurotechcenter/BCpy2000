////////////////////////////////////////////////////////////////////////////////
// $Id: actiCHampBufferedADC.h 5317 2016-04-20 15:11:27Z mellinger $
// Authors: Paul Ignatenko <paul dot ignatenko at gmail dot com
// Description: actiCHampBufferedADC header
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

#ifndef INCLUDED_actiCHampBufferedADC_H  // makes sure this header is not included more than once
#define INCLUDED_actiCHampBufferedADC_H

#include "actiCHampDevice.h"
#include "BufferedADC.h"
#include "PrecisionTime.h"

class actiCHampBufferedADC : public BufferedADC
{
 public:
  actiCHampBufferedADC();
  ~actiCHampBufferedADC();
  void OnPublish() override;
  void OnHalt() override;
  void OnPreflight( SignalProperties& Output ) const override;
  void OnInitialize( const SignalProperties& Output ) override;
  void OnStartAcquisition() override;
  void DoAcquire( GenericSignal& Output ) override;
  void OnStopAcquisition() override;

 private:
  actiCHampDevice  m_actiCHamp;
  double           m_msec_per_block;
  int              m_sample_block_size;
  int              m_sample_rate;
  int              m_mode;
  int              m_device_number;
  int              m_reference_channel;
  unsigned int     m_total_channel_count;
  unsigned int     m_eeg_count;
  unsigned int     m_aux_count;
  unsigned int     m_trigger_count;
  unsigned int     m_active_shield_gain;
  unsigned int    *mp_eeg_list;
  unsigned int    *mp_aux_list;
  unsigned int    *mp_trigger_list;
};

#endif // INCLUDED_actiCHampBufferedADC_H
