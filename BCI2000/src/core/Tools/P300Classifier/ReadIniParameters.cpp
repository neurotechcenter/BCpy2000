#include "ReadIniParameters.h"

///////////////////////////////////////////////////////////////////////////////
/// Read initial parameters given in a file (.ini). The defined parameters are 
/// then written in the structure Parameter.
/// @param [in] szFile		Name of the file (.ini) containing all the initial parameters. 
/// @param [out] IniParam	Structure Parameter containing all the initial parameters. 
/// \author Cristhian Potes
/// \date July 15, 2009

bool ReadIniParameters(string szFile, InitialParameter &IniParam)
{
///////////////////////////////////////////////////////////////////////////////
// Section: Define variables
CDataFile	CfgFile;
vector<int> chset;
vector<float> windlen;
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Section: Testing input parameters and initialize parameters
if (CfgFile.Load(szFile.c_str()))
{
	if (CheckCfgParameterKeyExist(CfgFile,"dir","Initialization"))
		IniParam.dir = CfgFile.GetString("dir","Initialization");
	else
		IniParam.dir = "C:";	

	if (CheckCfgParameterKeyExist(CfgFile,"decimation_frequency_Hz","Initialization"))
		IniParam.Decimation_Frequency = CfgFile.GetInt("decimation_frequency_Hz","Initialization");
	else
		IniParam.Decimation_Frequency = 20;

	if (CheckCfgParameterKeyExist(CfgFile,"spatial_filter","Initialization"))
		IniParam.SF = CfgFile.GetInt("spatial_filter","Initialization");
	else
		IniParam.SF = 1;

	if (CheckCfgParameterKeyExist(CfgFile,"penter","Initialization"))
		IniParam.penter = CfgFile.GetFloat("penter","Initialization");
	else
		IniParam.penter = 0.1000f;

	if (CheckCfgParameterKeyExist(CfgFile,"premove","Initialization"))
		IniParam.premove = CfgFile.GetFloat("premove","Initialization");
	else
		IniParam.premove = 0.1500f;

	if (CheckCfgParameterKeyExist(CfgFile,"maxiter","Initialization"))
		IniParam.maxiter = CfgFile.GetInt("maxiter","Initialization");
	else
		IniParam.maxiter = 60;

	if (CheckCfgParameterKeyExist(CfgFile,"channel_set","Initialization"))
	{
		string channelset = CfgFile.GetValue("channel_set","Initialization");
		read_vector(channelset, IniParam.channel_set);
	}
	else
		IniParam.channel_set.push_back(1); 

	if (CheckCfgParameterKeyExist(CfgFile,"Resp_window_ms","Initialization"))
	{
		string Resp_window = CfgFile.GetValue("Resp_window_ms","Initialization");
		read_vector(Resp_window, IniParam.windlen);
	}
	else
	{
		IniParam.windlen.push_back(0);
		IniParam.windlen.push_back(800);
	}
	return true;
}
else
// file does not exist
return false;
}



///////////////////////////////////////////////////////////////////////////////
/// Checks if a key that is supposed in a specific section exists in ini file. 
/// If the key does not exists an error message is generated and the programm 
/// is quit.
/// @param [in] CfgFile Reference to the loaded ini file. 
/// @param [in] key the key that is to be checked.
/// @param [in] section the section in which the key is supposed to be. 
///////////////////////////////////////////////////////////////////////////////
bool CheckCfgParameterKeyExist(CDataFile CfgFile, string key, string section)
{   
    if (CfgFile.GetString(key,section).length() == 0)
		return false;
	else
		return true;
}
