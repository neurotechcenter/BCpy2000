////////////////////////////////////////////////////////////////////////////////
// Authors: Zhiyang Xu
// Description: StimResponseVis implementation
////////////////////////////////////////////////////////////////////////////////
#include "StimResponseVis.h"
#include "GenericVisualization.h"
#include "BitmapImage.h"

#include <QPainter>

RegisterFilter(StimResponseVis, 2.Z);
#define PARAM( x ) auto x = Parameter( #x );
#define TRUE_PARAM( x ) auto True_##x = ActualParameter( #x );
struct StimResponseVis::Private
{
  // generic members
  // parameters
  RGBColor mBackground;
  int mHeight, mWidth, mDecimation;
  // state
  BitmapVisualization mVis;
  int mDecimationCounter;

  void initVisualization();
  void updateVisualization(const GenericSignal&);
  
  int num_centers;
  struct Corrdinate
  {
	  int x;
	  int y;
  };
  std::vector<Corrdinate> Centers;
  std::vector<int> BadChannels;
  int NumIter;
  std::vector<double> Signals;
  int NumIteBefAvg;
  float ActiveThreshold;
  float PropotionToSig;
  int IterToRestart;
  private: // Qt-dependent members
  QImage mImage;
  QRectF Rectangle;
  QRectF TextRect;
};

void
StimResponseVis::Private::initVisualization()
{
  mImage = QImage(mWidth, mHeight, QImage::Format_RGB16);
  QPainter painter(&mImage);
  painter.fillRect(mImage.rect(), mBackground.ToQRgb());
  mVis.Send(CfgID::Visible, mHeight*mWidth != 0);
  mVis.SendReferenceFrame(mImage);
  Signals = std::vector<double>(num_centers);
  NumIter=0;
  TextRect =  QRectF(0,0,40,40);
}

void
StimResponseVis::Private::updateVisualization(const GenericSignal& Input)
{
  //increment the iteration counter  
  NumIter++;	
  //if current number of iteration equals to the number of iteration to restart 
  if (NumIter==IterToRestart)
  {   
	  //initialize the counter again
	  NumIter=1;
	  for (int ch = 0; ch < num_centers; ch++)
	  {
		  //initialize signal again 
		  Signals[ch]=0;
	  }
  }
  //create painter object
  QPainter painter(&mImage);
  //specify the dimention of the window
  painter.fillRect(mImage.rect(), mBackground.ToQRgb());
  //create the container for the current max signal value
  std::vector<double>CurrentMaxSig(num_centers);
  for (int el = 0; el < Input.Elements(); el++)
  {
	for (int ch = 0; ch < num_centers+1; ch++)
	{	  
	  if (Input(ch,el)>CurrentMaxSig[ch])
		   CurrentMaxSig[ch]=Input(ch,el);
		 
	 }
  }
  //turn off the bad channels
  for (int ch = 0; ch < BadChannels.size(); ch++)
  {
	  CurrentMaxSig[BadChannels[ch]] = 0;
  }
  //start average signal
  if (NumIter>NumIteBefAvg)
  {
	  for (int ch = 0; ch < num_centers; ch++)
	  {
		 Signals[ch]+=CurrentMaxSig[ch];
	     CurrentMaxSig[ch] = Signals[ch]/(NumIter-NumIteBefAvg);
      }
  }
  //calculate the size and color of every dot according to input  
  for (int index = 0; index < num_centers ; index++)
  {
	  CurrentMaxSig[index]/=PropotionToSig;
      int Transparency= floor(255-7*CurrentMaxSig[index]);
	  if (Transparency<0)
		  Transparency=0;
	  if(CurrentMaxSig[index]<ActiveThreshold+1){
		  painter.setBrush(QColor(0,0,0,255));
		   Rectangle = QRectF(Centers[index].x-ActiveThreshold/2,Centers[index].y-ActiveThreshold/2,ActiveThreshold,ActiveThreshold);
	  }
	  else
	  {
		  painter.setBrush(QColor(255,0,0,Transparency));
		  Rectangle = QRectF(Centers[index].x -  CurrentMaxSig[index]/2,
		  Centers[index].y - CurrentMaxSig[index]/2,
		  CurrentMaxSig[index],CurrentMaxSig[index]);
	  }
	 painter.drawEllipse(Rectangle);
	 QString tr = "curr iter\n";
	 tr+=QString::number(NumIter);
	 painter.drawText(TextRect, Qt::AlignCenter, tr);
	 
  }
  mVis.SendReferenceFrame(mImage);
}

// Filter boilerplate code
StimResponseVis::StimResponseVis()
: p(new Private)
{
}

StimResponseVis::~StimResponseVis()
{
  delete p;
}

void StimResponseVis::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS
  "StimResponseVis int VisImageWidth= 1000 % 0 % // native image width",
  "StimResponseVis int VisImageHeight= 1000 % 0 % // native image height",
  "StimResponseVis int VisImageBackground= 0x808080 % // image background color (color)",
  "StimResponseVis int VisImageDecimation= 4 % 1 % // update on every nth frame only",
  "StimResponseVis float ActivateThreshold= 4 4 0 % // Threshold to determine if the input is activated",
  "StimResponseVis float ProportionToInputSignal= 2 2 0 % // the proportion of the size of red dot to input ",
  "StimResponseVis list BadChList= 0 // bad channels, 0 or empty for unipolar measurement",
  "StimResponseVis int NumIterBeforeAvg= 1 1 0 % // number of iteration before doing avg",
  "StimResponseVis int IterationBeforeRestartAvg= 201 201 0 % // number of iteration to restart avg",
  "StimResponseVis matrix ElectrodeLocation= "
      "{ 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 } " // row labels
      "{ label x y } " // column labels
      "Fp1.	-12546	38613	"
      "Fp2.	12546	38613	"
      "F3..	-15773	22527	"
      "F4..	15773	22527	"
      "C3..	-20300	00000	"
      "C4..	20300	00000	"
      "P3..	-15773	-22527	"
      "P4..	15773	-22527	"
      "T7..	-40600	00000	"
      "T8..	40600	00000	"
      "P7..	-32846	-23864	"
      "P8..	32846 -23864	"
      "Fz..	00000  20300	"
      "Pz..	00000 -18100	"
      "Cz..	00000  00000	"
      "Oz..	00000 -40600	"
      " // electrodes Location ",
 END_PARAMETER_DEFINITIONS
}

void StimResponseVis::Preflight(
  const SignalProperties& Input,
        SignalProperties& Output) const
{
  PARAM( BadChList );
  Output = Input;
  Parameter("VisImageBackground");
  Parameter("ElectrodeLocation");
  Parameter("NumIterBeforeAvg");
  Parameter("IterationBeforeRestartAvg");
  State("update");
}

void StimResponseVis::Initialize(
  const SignalProperties& Input,
  const SignalProperties& Output)
{
  p->mVis.SetVisID("Demo Visualization of signal \"" + Input.Name() + "\"");
  p->mWidth = Parameter("VisImageWidth");
  p->mHeight = Parameter("VisImageHeight");
  p->mBackground = RGBColor(Parameter("VisImageBackground"));
  p->mDecimation = Parameter("VisImageDecimation");
  p->initVisualization();
  p->num_centers = Parameter("ElectrodeLocation")->NumRows();
  p->NumIteBefAvg = Parameter("NumIterBeforeAvg");
  p->ActiveThreshold = Parameter("ActivateThreshold");
  p->PropotionToSig = Parameter("ProportionToInputSignal");
  p->IterToRestart = Parameter("IterationBeforeRestartAvg");
  //bciout<<"propotion to sig "<<p->PropotionToSig<<" active threshold "<<p->ActiveThreshold<<std::endl;
  for (int index = 0; index < p->num_centers; index++)
  {
	  StimResponseVis::Private::Corrdinate Center;
	  Center.x = atof(Parameter("ElectrodeLocation")->Value(index,1).c_str()) /20 +250;
      Center.y = atof(Parameter("ElectrodeLocation")->Value(index,2).c_str()) /20 +250;
	  p->Centers.push_back(Center);
	  //bciout<<Center.x<<" "<<Center.y<<std::endl;
  }
   PARAM( BadChList);
    for (int i = 0; i < BadChList->NumValues(); i++)
	  p->BadChannels.push_back(BadChList(i)-1);
 

}

void StimResponseVis::Process(
  const GenericSignal& Input,
        GenericSignal& Output)
{
	//bciout<<State("update")<<std::endl;
	if (State("update")==1)
	{

	 
	//Output = Input;
  if(p->mDecimationCounter == 0)
    p->updateVisualization(Input);
  ++p->mDecimationCounter %= p->mDecimation;
  
  }
 
}

void StimResponseVis::StartRun()
{
  p->mDecimationCounter = 0;
  p->initVisualization();
}

void StimResponseVis::StopRun()
{
  p->initVisualization();
}
