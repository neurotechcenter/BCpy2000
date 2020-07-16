////////////////////////////////////////////////////////////////////$
// Authors: Paul Ignatenko <paul dot ignatenko at gmail dot com
// Description: Device controls for the actiCHamp source module
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
// History:
// [2016.05.03] Bug fixed; there was problem by initializing the actiCHamp amplifier, but only in the release mode (Maximum speed optimization).
//              the variable 'm_is_acquiring' wasn't initialize.
////////////////////////////////////////////////////////////////////////////////


#include "ChannelRouter.h"
#include "actiCHampDevice.h"
#include "BCIStream.h"
#include <windows.h>
#include <sstream>
#include <iostream>
#include <bitset>


#define preErrorStream bcierr
#define preOutputStream bciout


using namespace std;


actiCHampDevice::actiCHampDevice()
: m_device( 0 ), m_is_acquiring (false)
{    
    //Null this out just in case of compiler issues.
    mp_channel_matrix = 0;
    dataaux = 0;
    data32 = 0;
    data64 = 0;
    data96 = 0;
    data128 = 0;
    data160 = 0;
}

actiCHampDevice::~actiCHampDevice()
{
    this->close();
}



/**
 * Open a m_device by m_device number
 *
 * Opens a m_device and tries to intitialize the m_device, library, and FPGA has
 * m_init_tries to init the FPGA. The FPGA should init on first
 * try unless the file ActiChamp.bit is missing.
 *
 * @param devNum m_device number to initialize
 *
 * @return true if m_device initialized properly false otherwise
 */

bool actiCHampDevice::open(int devNum)
{
    int e;
    //Get the number of m_devices, with this we can test if we can see hardware.
    if(champGetCount() == 0)
    {
        preErrorStream << "Hardware is not available" << endl;
        return false;
    }
	
   
	m_device = champOpen(devNum);


    if(m_device == 0)
    {
		preErrorStream << "Failed to get device handle. device nr: " <<  devNum << endl;
        return false;
    }

    e = champGetVersion(m_device, &m_version);
    if (e != CHAMP_ERR_OK) {
        preErrorStream << "Could not get device version. actiCHamp Error: " << e << endl;
        return false;
    }
    
    if (m_version.Fpga == 0) {
        if (m_init_tries < 1)
        {
            preErrorStream << "FPGA failed to initalize... exiting" << endl;
            return false;
        }
        else
        {
            preErrorStream << "FPGA not initialized on this try, make sure that the firmware file is in the same directory as the DLL. Retrying to open the device...."<< endl;
            close();
            --m_init_tries;
            return open(devNum);
        }
    }

    m_init_tries = 3;

    e = champGetModules(m_device, &m_modules);
    if (e != CHAMP_ERR_OK) {
        preErrorStream << "Could not get device modules. actiCHamp Error: " << e << endl;
        return false;
    }


    e = champGetProperty(m_device, &m_properties);
    if (e != CHAMP_ERR_OK) {
        preErrorStream << "Could not get device properties. actiCHamp Error: " << e << endl;
        return false;
    }

    t_champSettingsEx temp_settings;
    e = champGetSettingsEx(m_device, &temp_settings);
    if( e != CHAMP_ERR_OK)
    {
        preErrorStream << "Could not get m_device default settings. actiCHamp Error: " << e << endl;
        return false;
    }

    m_mode = temp_settings.Mode;
    m_rate = temp_settings.Rate;
    m_averaging = temp_settings.AdcFilter;
    m_decimation = temp_settings.Decimation;

    m_acquisition_lock = CreateMutex(NULL, false, NULL);

    if(m_acquisition_lock == NULL)
    {
        preErrorStream << "CreateMutex error..." << GetLastError();
        return false;
    }


    return true;
}


/**
 * Closes the m_device
 *
 * Closes the m_device
 *
 * @return None
 */
void actiCHampDevice::close()
{
    //Acquire mutex first...
    if(m_is_acquiring == true)
    {
        preOutputStream <<"Device still in data acquisition mode, stop the m_device before closing.\n";
    } 
    else if(m_device != NULL || m_device != 0)
    {
        champClose(m_device);
        m_device = 0;
        CloseHandle(m_acquisition_lock);

    }

    delete mp_channel_matrix;
    mp_channel_matrix = 0;


}


bool actiCHampDevice::init() 
{
    //Enable our modules
    m_modules.Enabled = m_modules.Present;
	t_champModules temp_modules = m_modules;
    int e = champSetModules(m_device, &temp_modules);

    if (e != CHAMP_ERR_OK) 
    {
        preErrorStream << "Error could not set modules correctly. Error: " << e <<endl;
        return false;
    }


    //Set our desired settings.
    t_champSettingsEx champ_init_settings;
    champ_init_settings.Mode       = m_mode;
    champ_init_settings.Rate       = m_rate;
    champ_init_settings.AdcFilter  = m_averaging;
    champ_init_settings.Decimation = m_decimation;

    e = champSetSettingsEx(m_device, &champ_init_settings); 
    if (e != CHAMP_ERR_OK) 
    {
        preErrorStream << "Error could not set settings correctly. Error: " << e <<endl;
        return false;
    }

    e = champGetProperty(m_device, &m_properties);
    if (e != CHAMP_ERR_OK) {
        preErrorStream << "Could not get m_device properties. actiCHamp Error: " << e << endl;
        return false;
    }

	//Sleep (1000);    
    return true;
}


/**
 * Initialize the m_device for data aquisition
 *
 * Tries to set the parameters for data aquisition according to the user
 * settings.
 *
 * @return True if the preperation was successful, false otherwise.
 */

bool actiCHampDevice::start()
{

    int e;

    // Start our amplifier
    e = champStart(m_device);
    if (e != CHAMP_ERR_OK) 
    {
        preErrorStream << "Error could not start the m_device correctly. Error: " << e <<endl;
        return false;
    }


    m_is_acquiring = true;
    return true;
}



/**
 * Takes m_device out of data acquisition mode
 *
 * Stops data acquisition on the m_device, the m_device will no longer continually
 * read data and fill it's buffer
 *
 * @return true if successful false otherwise
 */
bool actiCHampDevice::stop()
{
    if (!m_is_acquiring)
    {
        preErrorStream << "Error m_device not in data acquisition mode, it is already in stopped state.";
        return false;
    }
    else
    {
        int lock_status = WaitForSingleObject(m_acquisition_lock, 6000);
        int e;
        switch(lock_status)
        {
            case WAIT_OBJECT_0:
                e = champStop(m_device);
                if (e != CHAMP_ERR_OK) 
                {
                    preErrorStream << "Error could not stop the m_device correctly. Error: " << e <<endl;
                    return false;
                }

                m_is_acquiring = false;
                return true;
                break;

            case WAIT_TIMEOUT:
            case WAIT_FAILED:
                    preErrorStream << "Error could not stop the m_device correctly. Could not acquire aquisition handle" << endl;
                    return false;
                break;

        }
        return false;
    }
}



/**
 * Gets a certain ammount of data from the m_device
 *
 * Blocks until it collects a certain ammount of samples (<size>) from the m_device
 *  
 * @param output array of <size> where the collected data will go.
 * @param size the ammount of samples to collect
 *
 * @return return output filled with samples collected from the amp
 */
void actiCHampDevice::get_data(GenericSignal & output, unsigned int size_in_samples)
{  
    int size_in_bytes = (m_properties.CountEeg + m_properties.CountAux + 2) * sizeof(int) * size_in_samples;

    if (m_properties.CountEeg == 0 && m_properties.CountAux == 8)
    {
        dataaux = new t_champDataModelAux[size_in_samples];
        read_data(dataaux, size_in_bytes);
        output_data(output, dataaux);
        delete[] dataaux;
        dataaux = 0;
    }
    else if (m_properties.CountEeg == 32 && m_properties.CountAux ==8)
    {
        data32 = new t_champDataModel32[size_in_samples];
        read_data(data32, size_in_bytes);
        output_data(output, data32);
        delete[] data32;
        data32 = 0;
    }
    else if (m_properties.CountEeg == 64 && m_properties.CountAux ==8)
    {
        data64 = new t_champDataModel64[size_in_samples];
        read_data(data64, size_in_bytes);
        output_data(output, data64);
        delete[] data64;
        data64 = 0;
    }
    else if (m_properties.CountEeg == 96 && m_properties.CountAux ==8)
    {
        data96 = new t_champDataModel96[size_in_samples];
        read_data(data96, size_in_bytes);
        output_data(output, data96);
        delete[] data96;
        data96 = 0;

    }
    else if (m_properties.CountEeg == 128 && m_properties.CountAux ==8)
    {
        data128 = new t_champDataModel128[size_in_samples];
        read_data(data128, size_in_bytes);
        output_data(output, data128);
        delete[] data128;
        data128 = 0;

    }
    else if (m_properties.CountEeg == 160 && m_properties.CountAux ==8)
    {
        data160 = new t_champDataModel160[size_in_samples];
        read_data(data160, size_in_bytes);
        output_data(output, data160);
        delete[] data160;
        data160 = 0;
    }
    else{
        preErrorStream << "Uknown number of modules present" <<endl ;
    }


}

void actiCHampDevice::output_data(GenericSignal & output, t_champDataModelAux* data)
{
    for(unsigned int e = 0; e < (unsigned int) output.Elements(); ++e)
    {
        bitset<32> channel_bits (static_cast<int>(data[e].Triggers));
        for(unsigned int i = 0; i < 6; i++)
        {
            if(channel_bits[16+i] == 1 && channel_bits[22+i] == 1)
            {
                bcierr << "Error in module " << i <<endl;
            }
        }

        for(unsigned int c = 0; c < (unsigned int)output.Channels(); ++c)
        {
            const TypedChannel *p_ch = mp_channel_matrix->genericToTyped(c);
            
            if(p_ch->getType().compare("aux") == 0)
            {
                //Get data
                output(c, e) = data[e].Aux[p_ch->getIndex()];
                //Apply Resolution
                output(c, e) = output(c, e) * m_properties.ResolutionAux;
                output(c, e) = output(c, e) * 1000000;
            }
            //Trigger Channels
            else if(p_ch->getType().compare("trigger") == 0)
            {
                output(c, e) = channel_bits[p_ch->getIndex()];
            }

            else
            {
                preErrorStream << "Error: Unknown Channel Type " <<p_ch->getType()<<endl;
            }

        }
    }

}
template<class T> 
void actiCHampDevice::output_data(GenericSignal & output, T& data)
{

    for(unsigned int e = 0; e < (unsigned int) output.Elements(); ++e)
    {
        //Check for data errors
        bitset<32> channel_bits (static_cast<int>(data[e].Triggers));
        for(unsigned int i = 0; i < 6; i++)
        {
            if(channel_bits[16+i] == 1 && channel_bits[22+i] == 1)
            {
                bcierr << "Error in module " << i <<endl;
            }
        }

        for(unsigned int c = 0; c < (unsigned int)output.Channels(); ++c)
        {
            //Get channel & process it
            const TypedChannel *p_ch = mp_channel_matrix->genericToTyped(c);

            //Eeg Channels
            if(p_ch->getType().compare("eeg") == 0)
            {
                // Are we using reference?
                if(m_reference_channel >= 0)
                {
                    //Get data
                    output(c, e) = data[e].Main[p_ch->getIndex()];
                    output(c, e) = output(c, e) - data[e].Main[m_reference_channel];
                    //Apply Resolution
                    output(c, e) = output(c, e) * m_properties.ResolutionEeg;
                    output(c, e) = output(c, e) * 1000000;

                }
                else
                {
                    //Get data
                    output(c, e) = data[e].Main[p_ch->getIndex()];
                    //Apply Resolution
                    output(c, e) = output(c, e) * m_properties.ResolutionEeg;
                    output(c, e) = output(c, e) * 1000000;
                }
            }
            //Aux Channels
            else if(p_ch->getType().compare("aux") == 0)
            {
                //Get data
                output(c, e) = data[e].Aux[p_ch->getIndex()];
                //Apply Resolution
                output(c, e) = output(c, e) * m_properties.ResolutionAux;
                output(c, e) = output(c, e) * 1000000;
            }
            //Trigger Channels
            else if(p_ch->getType().compare("trigger") == 0)
            {
                output(c, e) = channel_bits[p_ch->getIndex()];
            }
            else
            {
                preErrorStream << "Error: Unknown Channel Type " <<p_ch->getType()<<endl;
            }

        }
    }

}

void actiCHampDevice::read_data(void* buffer , unsigned int size)
{
    int e;
    int wait_result = WaitForSingleObject(m_acquisition_lock, 2000);
    switch(wait_result)
    {
        case WAIT_OBJECT_0:
            e = champGetDataBlocking(m_device, buffer, size);
            if (e < 0)
            {
                preErrorStream << "Error acquiring data" <<endl;
            }
            if (!ReleaseMutex(m_acquisition_lock))
            {
                preErrorStream << "Could not release acquisition lock!" <<endl;
            }
            break;
        case WAIT_TIMEOUT:
        case WAIT_FAILED:
            preOutputStream << "Could not acquire mutex for acquisition, m_device must be terminating." <<endl;
            break;
    }

}

t_champDataStatus actiCHampDevice::get_data_status() const
{
    t_champDataStatus champ_data_info;
    int e = champGetDataStatus(m_device, &champ_data_info);
    if (e != CHAMP_ERR_OK)
    {
        preErrorStream << "Could not get m_device data status. actiCHamp data: " << e << endl;
    }
    return champ_data_info;
}

t_champErrorStatus actiCHampDevice::get_error_status() const
{
    t_champErrorStatus champ_error_info;
    int e = champGetErrorStatus(m_device, &champ_error_info);
    if (e != CHAMP_ERR_OK)
    {
        preErrorStream << "Could not get m_device error status. actiCHamp Error: " << e << endl;
    }
    return champ_error_info;
}

bool actiCHampDevice::Settings(string& settings) const
{
    t_champSettingsEx temp_settings;
    int e = champGetSettingsEx(m_device, &temp_settings);
    if( e != CHAMP_ERR_OK)
    {
        preErrorStream << "Could not get m_device settings. actiCHamp Error: " << e << endl;
        return false;
    }
    else
    {
        stringstream ss;
        ss << "Device Settings: "<<  "\n"
            << " Rate = " << temp_settings.Rate << "\n"
            << " Mode = " << temp_settings.Mode << "\n"
            << " Averaging = " << temp_settings.AdcFilter << "\n"
            << " Decimation = " << temp_settings.Decimation << "\n"
            << endl;
        settings = ss.str();
    }
    return true;

}

/**
 * Get the data status of the m_device into ostream output
 *
 * Gets the m_device data status, and outputs it in to a ostream in a text
 * readable format.
 *
 * @param output ostream to pack data into
 *
 * @return output will have text readable data status
 */
bool actiCHampDevice::DataStatus(string& data_status) const
{
    t_champDataStatus champ_data_info;
    int e = champGetDataStatus(m_device, &champ_data_info);
    if (e != CHAMP_ERR_OK)
    {
        preErrorStream << "Could not get m_device data status. actiCHamp Error: " << e << endl;
        return false;
    }
    else
    {
        stringstream ss;
        ss << "Data Status: "      << "\n"
               << "  Total Samples = " << champ_data_info.Samples << "\n"
               << "  Errors = "        << champ_data_info.Errors  << "\n"
               << "  Rate = "          << champ_data_info.Rate    << "\n"
               << "  Speed = "         << champ_data_info.Speed   << "\n"
               << endl;
        data_status = ss.str();
    }
    return true;
}


/**
 * Get the error status of the m_device into ostream output
 *
 * Gets the m_device error status, and outputs it in to a ostream in a text
 * readable format.
 *
 * @param output ostream to pack error into
 *
 * @return output will have text readable error status
 */
bool actiCHampDevice::ErrorStatus(string& error_status) const
{
    t_champErrorStatus champ_error_info;
    int e = champGetErrorStatus(m_device, &champ_error_info);
    if (e != CHAMP_ERR_OK)
    {
        preErrorStream << "Could not get m_device error status. actiCHamp Error: " << e << endl;
        return false;
    }
    else
    {
        stringstream ss;
        ss << "Error Status: "      << "\n"
               << "  Total Samples = "  << champ_error_info.Samples    << "\n"
               << "  CRC Errors = "     << champ_error_info.Crc        << "\n"
               << "  Counter Errors = " << champ_error_info.Counter    << "\n"
               << "  Error on Aux = "   << champ_error_info.Modules[0] << "\n"
               << "  Error on M1 = "    << champ_error_info.Modules[1] << "\n"
               << "  Error on M2 = "    << champ_error_info.Modules[2] << "\n"
               << "  Error on M3 = "    << champ_error_info.Modules[3] << "\n"
               << "  Error on M4 = "    << champ_error_info.Modules[4] << "\n"
               << "  Error on M5 = "    << champ_error_info.Modules[5] << "\n"
               << endl;
        error_status = ss.str();
    }
        return true;
}

/**
 * Get the version of the m_device into ostream output
 *
 * Gets the m_device version, and outputs it in to a ostream in a text
 * readable format.
 *
 * @param output ostream to pack error into
 *
 * @return output will have text readable version
 */
void actiCHampDevice::DeviceVersion(string& version) const
{
    stringstream ss;
    ss << "Device Version Information: " << "\n"
           << "    Dll: "                    << m_version.Dll     << "\n"
           << "    DRIVER: "                 << m_version.Driver  << "\n"
           << "    CYPRESS: "                << m_version.Cypress << "\n"
           << "    FPGA: "                   << m_version.Fpga    << "\n"
           << "    MSP340: "                 << m_version.Msp430  << "\n" 
           << endl;
    version = ss.str();
}

/**
 * Get the properties of the m_device into ostream output
 *
 * Gets the m_device properties, and outputs it in to a ostream in a text
 * readable format.
 *
 * @param output ostream to pack error into
 *
 * @return output will have text readable properties
 */
void actiCHampDevice::GetProperties(string& properties) const
{
    stringstream ss;
    ss << "Device Property Information: "                << "\n"
           << "Number of EEG channels"
           << "numbers of Eeg channels: "                    << m_properties.CountEeg      << "\n"
           << "numbers of Aux channels: "                    << m_properties.CountAux    << "\n"
           << "numbers of input triggers: "                  << m_properties.TriggersIn    << "\n"

           << "numbers of output triggers: "                 << m_properties.TriggersOut   << "\n"
           << "!< Sampling rate, Hz: "                       << m_properties.Rate          << "\n"
           << "!< EEG amplitude scale coefficients, V/bit: " << m_properties.ResolutionEeg<<
    "\n"
           << "!< AUX amplitude scale coefficients, V/bit: " << m_properties.ResolutionAux<<
    "\n"
           << "!< EEG input range peak-peak, V: "            << m_properties.RangeEeg      << "\n"
           << "!< AUX input range peak-peak, V: "            << m_properties.RangeAux      << "\n"
           << endl;
    properties = ss.str();
}

bool actiCHampDevice::set_rate(unsigned int r)
{
    this->m_desired_rate = r;

    switch(m_desired_rate)
    {
        case 100000:
            m_rate = CHAMP_RATE_100KHZ;
            m_averaging = CHAMP_ADC_NATIVE;
            m_decimation = CHAMP_DECIMATION_0;
            return true;
            break;
        case 50000:
            m_rate = CHAMP_RATE_50KHZ;
            m_averaging = CHAMP_ADC_NATIVE;
            m_decimation = CHAMP_DECIMATION_0;
            return true;
            break;
        case 10000:
            m_rate = CHAMP_RATE_10KHZ;
            m_averaging = CHAMP_ADC_NATIVE;
            m_decimation = CHAMP_DECIMATION_0;
            return true;
            break;
        default:
            if(m_desired_rate > 100000)
            {
            preErrorStream << "Unsupported user entered rate" << endl;
            return false;
            }
            else if(m_desired_rate > 50000)
            {
                if (100000%m_desired_rate != 0)
                {
                    preErrorStream << "Unsupported user rate, must cleanly go into 10000" << endl;
                    return false;
                }
                else
                {
                    m_rate = CHAMP_RATE_100KHZ;
                    m_averaging = (t_champAdcFilter)((int)100000/m_desired_rate);
                    m_decimation = (t_champDecimation)((int)100000/m_desired_rate);
                    return true;
                }
            }
            else if(m_desired_rate > 10000)
            {
                if (50000%m_desired_rate != 0)
                {
                    preErrorStream << "Unsupported user rate, must cleanly go into 10000" << endl;
                    return false;
                }
                else
                {
                    m_rate = CHAMP_RATE_50KHZ;
                    m_averaging = (t_champAdcFilter)((int)50000/m_desired_rate);
                    m_decimation = (t_champDecimation)((int)50000/m_desired_rate);
                    return true;
                }
            }
            else if(m_desired_rate > 0)
            {
                if (10000%m_desired_rate != 0)
                {
                    preErrorStream << "Unsupported user rate, must cleanly go into 10000" << endl;
                    return false;
                }
                else
                {
                    m_rate = CHAMP_RATE_10KHZ;
                    m_averaging = (t_champAdcFilter)((int)50000/m_desired_rate);
                    m_decimation = (t_champDecimation)((int)10000/m_desired_rate);
                    return true;
                }
            }
            break;
    }

    return false;

}


void actiCHampDevice::get_impedance_data ()
{
    unsigned int *impedance_buffer;
    //actichamp_w.py:298 -- don't ask.
    int size = 1000;
    impedance_buffer = new unsigned int[size];


    int e = champImpedanceGetData(m_device, impedance_buffer, size);

    int size_in_samples = 50;
    int size_in_bytes = (m_properties.CountEeg + m_properties.CountAux + 2) * sizeof(int) * size_in_samples;

    if (m_properties.CountEeg == 0 && m_properties.CountAux == 8)
    {
        dataaux = new t_champDataModelAux[size_in_samples];
        read_data(dataaux, size_in_bytes);
        delete[] dataaux;
        dataaux = 0;
    }
    else if (m_properties.CountEeg == 32 && m_properties.CountAux ==8)
    {
        data32 = new t_champDataModel32[size_in_samples];
        read_data(data32, size_in_bytes);
        delete[] data32;
        data32 = 0;
    }
    else if (m_properties.CountEeg == 64 && m_properties.CountAux ==8)
    {
        data64 = new t_champDataModel64[size_in_samples];
        read_data(data64, size_in_bytes);
        delete[] data64;
        data64 = 0;
    }
    else if (m_properties.CountEeg == 96 && m_properties.CountAux ==8)
    {
        data96 = new t_champDataModel96[size_in_samples];
        read_data(data96, size_in_bytes);
        delete[] data96;
        data96 = 0;

    }
    else if (m_properties.CountEeg == 128 && m_properties.CountAux ==8)
    {
        data128 = new t_champDataModel128[size_in_samples];
        read_data(data128, size_in_bytes);
        delete[] data128;
        data128 = 0;

    }
    else if (m_properties.CountEeg == 160 && m_properties.CountAux ==8)
    {
        data160 = new t_champDataModel160[size_in_samples];
        read_data(data160, size_in_bytes);
        delete[] data160;
        data160 = 0;
    }
    else{
        preErrorStream << "Uknown number of modules present" <<endl ;
    }

    if( e == CHAMP_ERR_OK)
    {
        preOutputStream << "Current Impedance Values: " << endl;
        for( unsigned int i = 0 ; i < m_properties.CountEeg ; ++i)
        {

            preOutputStream << "Channel " << i << ":   " << impedance_buffer[i] << endl;

        }
    }
    else
    {
        preOutputStream  << "Count not get impedance values for any channels." << "ERROR : " << e << endl;
    }

    delete[] impedance_buffer;
    impedance_buffer = 0;

}

bool actiCHampDevice::setup_channels(unsigned int in_num_ch, unsigned int in_num_eeg, unsigned int in_num_aux, unsigned int in_num_triggers)
{
    if(in_num_ch > 0 && in_num_eeg +  in_num_aux + in_num_triggers == in_num_ch)
    {
        mp_channel_matrix = new ChannelRouter(in_num_ch);

        mp_channel_matrix->addChannelType("eeg", in_num_eeg);
        mp_channel_matrix->addChannelType("aux", in_num_aux);
        mp_channel_matrix->addChannelType("trigger", in_num_triggers);

    }
    return false;
}

bool actiCHampDevice::route_channels(unsigned int* in_eeg_list, unsigned int* in_aux_list, unsigned int* in_trigger_list)
{
    if(mp_channel_matrix)
    {
        int offset = 0;
        int ch_count;


        //Main Channels
        ch_count =  mp_channel_matrix->get_typed_channel_count("eeg");
        if(ch_count < 0)
        {
            preErrorStream << "Channel Routing Matrix was not setup correctly" << endl;
        }

        for( unsigned int i = 0; i < (unsigned int) ch_count; ++i)
        {
            mp_channel_matrix->route(i, "eeg", in_eeg_list[i]);
        } 


        // Aux Channels
        offset = mp_channel_matrix->get_typed_channel_count("eeg");
        ch_count =  mp_channel_matrix->get_typed_channel_count("aux");
        if(ch_count < 0)
        {
            preErrorStream << "Channel Routing Matrix was not setup correctly" << endl;
        }


        for(unsigned int i = 0; i < (unsigned int) ch_count; ++i)
        {
            mp_channel_matrix->route(offset + i, "aux", in_aux_list[i]);
        }

        //Trigger channels
        //
        offset = mp_channel_matrix->get_typed_channel_count("eeg")  + mp_channel_matrix->get_typed_channel_count("aux") ;
        ch_count =  mp_channel_matrix->get_typed_channel_count("trigger");
        if(ch_count < 0)
        {
            preErrorStream << "Channel Routing Matrix was not setup correctly" << endl;
        }

        for(unsigned int i = 0; i < (unsigned int) ch_count; ++i)
        {
            mp_channel_matrix->route(offset + i, "trigger", in_trigger_list[i]);
        }

        return true;
    }
    return false;

}


unsigned int actiCHampDevice::get_max_supported_trigger()
{
    if ( (1 & (m_modules.Present)) == 1 )
        return 16;
    return 0;
}
unsigned int actiCHampDevice::get_max_supported_aux()
{
    if ( (1 & (m_modules.Present)) == 1 )
        return 8;
    return 0;
}
unsigned int actiCHampDevice::get_max_supported_eeg()
{
    unsigned int counter = 0;
    for(int i = 1; i < 6;++i)
    {
        int mask = 1 << i;
        if( (mask & (m_modules.Present)) == mask) 
            ++counter;
    }
    return (counter * 32);
    
}
