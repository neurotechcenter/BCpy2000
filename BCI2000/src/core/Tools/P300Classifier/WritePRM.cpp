#include "WritePRM.h"

///////////////////////////////////////////////////////////////////////////////
/// Generate prm fragment
/// @param [in] filename		String containing the directory where you want to save the prm
/// @param [in] tMUD			Structure containing all the parameters.
/// @param [in] opt				Specify the type of spatial filter (SF). If opt = 1; then SF is RAW
///								If opt = 2; then SF is CAR.
/// \author Cristhian Potes
/// \date July 22, 2009

void WritePRM(string filename, parameters tMUD, int opt)
{
	int numCh, i, j;
	numCh = tMUD.channels.gethighbound(1)+1;
	ofstream fout(filename.c_str());

	if (fout.is_open())
	{
		fout <<	"Filtering:LinearClassifier matrix Classifier= " << tMUD.MUD.gethighbound(1)+1 << " { input%20channel input%20element%20(bin) output%20channel weight } ";
		for (i=0; i<tMUD.MUD.gethighbound(1)+1; i++)
		{
			for (j=0; j<tMUD.MUD.gethighbound(0)+1; j++)
			{
				if (j==2)
					fout << int (1) << " ";
				
				fout << tMUD.MUD(i,j) << " ";
			}
		}
		fout << "// Linear classification matrix in sparse representation" << endl; 
		fout << "Filtering:P3TemporalFilter int EpochLength= " << ap::ifloor(tMUD.windowlen(1)*1000/tMUD.smprate) << "ms 500ms 0 % // Length of data epoch from stimulus onset" << endl;
//		fout << "Source:gUSBampADC intlist SourceChDevices= 1 " << tMUD.softwarech << " " << tMUD.softwarech << " 1 " << tMUD.softwarech << " // number of digitized channels total" << endl;
		if (opt == 2)
		{
			fout << "Filtering:SpatialFilter int SpatialFilterType= " << int (3) << " 2 0 3 // spatial filter type 0: none, 1: full matrix, 2: sparse matrix, 3: common average reference (CAR) (enumeration)" << endl;
			/*fout << "Filtering:SpatialFilter matrix SpatialFilter= " << tMUD.softwarech << " " << tMUD.softwarech << " ";
			for (i=0; i<tMUD.softwarech; i++)
			{
				for (j=0; j<tMUD.softwarech; j++)
				{
					if (i == j)
						fout << float(1) - 1/ float(tMUD.softwarech) << " ";
					else
						fout << - 1/float(tMUD.softwarech) << " ";
				}
			}*/
		}
		if (opt == 1)
		{
			fout << "Filtering:SpatialFilter int SpatialFilterType= " << int (0) << " 2 0 3 // spatial filter type 0: none, 1: full matrix, 2: sparse matrix, 3: common average reference (CAR) (enumeration)" << endl;
			/*fout << "Filtering:SpatialFilter matrix SpatialFilter= " << numCh << " " << numCh << " ";
			for (i=0; i<numCh; i++)
			{
				for (j=0; j<numCh; j++)
				{
					if (i == j)
						fout << float (1) << " ";
					else
						fout << float (0) << " ";
				}
			}*/
		}
		//fout << "// columns represent input channels, rows represent output channels" << endl;
		fout << "Source:Online%20Processing list TransmitChList= " << numCh << " ";
		for (i=0; i<numCh; i++)
			// Cristhian modification April 17, 2010
			fout << tMUD.chSet(i) << " ";
		
		fout << "1 1 128 // list of transmitted channels" << endl;

		fout.close();
	}
	else
	printf("Failed in writing %s", filename.c_str());
}
