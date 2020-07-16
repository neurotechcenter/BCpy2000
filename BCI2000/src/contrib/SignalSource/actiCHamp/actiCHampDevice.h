////////////////////////////////////////////////////////////////////$
// Authors: Paul Ignatenko <paul dot ignatenko at gmail dot com
// Description: Header for device control
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
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTICHAMP_DEVICE_H
#define  __ACTICHAMP_DEVICE_H

#include "ChannelRouter.h"
#include "ActiChamp.imports.h"
#include "GenericSignal.h"
#include <string>
#include <iostream>
#include <ostream>
#include <sstream>


class actiCHampDevice {
    private:
        void output_data(GenericSignal & output, t_champDataModelAux* data);
        template<class T> 
           void output_data(GenericSignal & output, T& data);
        void read_data(void* buffer , unsigned int size);
        t_champMode        m_mode;                // Device mode of operation
        t_champRate        m_rate;                // Device sampling rate
        t_champAdcFilter   m_averaging;           // Averaging filter values
        t_champDecimation  m_decimation;          // Decimation filter values

        t_champModules     m_modules;             // Device Modules to use
        t_champVersion     m_version;             // Version of device
        t_champProperty    m_properties;          // Properties structure


        int                m_desired_rate;        // User desired sampling rate
        int                m_init_tries;          // Attempts at initialization
        int                m_reference_channel;   // What channel is our reference
        bool               m_is_acquiring;        // Are we acquiring data?

        HANDLE             m_acquisition_lock;    // Mutex for data acquisition
        HANDLE             m_device;              // Device handle
        ChannelRouter     *mp_channel_matrix;     // Our channel routing matrix

        // Pointers for handling different data types (module numbers)
        t_champDataModelAux *dataaux;
        t_champDataModel32  *data32;
        t_champDataModel64  *data64;
        t_champDataModel96  *data96;
        t_champDataModel128 *data128;
        t_champDataModel160 *data160;


    public:
        actiCHampDevice();
        ~actiCHampDevice();


        bool open(int device_number = 0 );
        void close();

        bool init();
        bool start();
        bool stop();

        // Data functions
        void get_impedance_data ();
        void get_data (GenericSignal & output, unsigned int size_in_samples);

        //Variable Manipulation functions.

        // Mode
        t_champMode mode() const
            { return m_mode; };
        void  set_mode(t_champMode m)
            { m_mode = m; }

        // Rate
        t_champRate get_rate() const
            {return m_rate; };
        void  set_rate (t_champRate r)
            { m_rate = r; };
        bool set_rate (unsigned int  r);

        //Averaging
        t_champAdcFilter get_averaging() const
            { return m_averaging; };
        void set_averaging  (t_champAdcFilter  a)
            { m_averaging = a; };

        //Decimation
        t_champDecimation get_decimation() const
            { return m_decimation; };
        void set_decimation (t_champDecimation  d)
            { m_decimation = d; };

        //Modules
        t_champModules get_modules() const
            { return m_modules; };
        void set_modules(t_champModules m)
            { m_modules = m; };

        //Version
        t_champVersion     get_version()       const
            { return m_version; };
        void DeviceVersion ( std::string& version      ) const;

        //Properties
        t_champProperty    get_properties()  const
            { return m_properties; }; 
        void GetProperties ( std::string& properties   ) const;

        //Reference Channel
        int get_reference_channel() const
            { return m_reference_channel; };
        void set_reference_channel(int r)
            { m_reference_channel = r; };
        
        //Acquisiton
        bool is_acquiring() const
            { return m_is_acquiring; };

        //ActiveShield Gain
        int set_activeshield_gain (unsigned int g)
            { return champSetActiveShieldGain(m_device, g);}

        //Channels
        unsigned int get_max_supported_eeg();
        unsigned int get_max_supported_aux();
        unsigned int get_max_supported_trigger();
        bool setup_channels(unsigned int in_num_ch, unsigned int in_num_eeg, unsigned int in_num_aux, unsigned int in_num_triggers);
        bool route_channels(unsigned int* in_eeg_list, unsigned int* in_aux_list, unsigned int* in_trigger_list);


        //Error Functions
        t_champDataStatus  get_data_status()   const;
        t_champErrorStatus get_error_status()  const;
        bool Settings      ( std::string& settings     ) const;
        bool DataStatus    ( std::string& data_status  ) const;
        bool ErrorStatus   ( std::string& error_status ) const;

        


};

#endif
