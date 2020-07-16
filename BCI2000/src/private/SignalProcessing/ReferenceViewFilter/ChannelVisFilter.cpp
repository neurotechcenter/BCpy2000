////////////////////////////////////////////////////////////////////////////////
// Authors:
// Description: ChannelVisFilter implementation
////////////////////////////////////////////////////////////////////////////////
#include "ChannelVisFilter.h"
#include "GenericVisualization.h"
#include "BitmapImage.h"

#include <QPainter>

using namespace std;

#define PARAM( x ) auto x = Parameter( #x );
#define TRUE_PARAM( x ) auto True_##x = ActualParameter( #x );
RegisterFilter( ChannelVisFilter, 2.B );

struct ChannelVisFilter::Private
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
  
  int mNum_centers;
  struct Corrdinate
  {
	  int x;
	  int y;
  };
  std::vector<Corrdinate> mCenters;
  float mPropotionToSig;
  std::vector<int> mBadChannels;
  private: // Qt-dependent members
  QImage mImage;
  QRectF Rectangle;
  QRectF TextRect;
};

void
ChannelVisFilter::Private::initVisualization()
{
  mImage = QImage(mWidth, mHeight, QImage::Format_RGB16);
  QPainter painter(&mImage);
  painter.fillRect(mImage.rect(), mBackground.ToQRgb());
  mVis.Send(CfgID::Visible, mHeight*mWidth != 0);
  mVis.SendReferenceFrame(mImage);
  TextRect =  QRectF(5,0,60,40);
}

void
ChannelVisFilter::Private::updateVisualization(const GenericSignal& Input)
{
	
  //create painter object
  QPainter painter(&mImage);
  //specify the dimention of the window
  painter.fillRect(mImage.rect(), mBackground.ToQRgb());
  //create the container for the current max signal value
  std::vector<double> AverageSig(mNum_centers);
  for (int ch = 0; ch < mNum_centers; ch++)
  {
	  AverageSig[ch] = 0;
  }
  for (int ch = 0; ch < mNum_centers; ch++)
  {
	for (int el = 0; el < Input.Elements(); el++)
	{	  
	  AverageSig[ch] += Input(ch,el);	 
	}
  }

  for (int ch = 0; ch < mNum_centers; ch++)
	  AverageSig[ch]/=Input.Elements();
  //calculate the size and color of every dot according to input  
  for (int index = 0; index < mNum_centers ; index++)
  {
	  AverageSig[index]/=mPropotionToSig;
      int Transparency= floor(255-7*AverageSig[index]);
	  if (Transparency<0)
		  Transparency=0;
	
		  painter.setBrush(QColor(0,255,255,Transparency));
		  Rectangle = QRectF(mCenters[index].x -  AverageSig[index]/2,
		  mCenters[index].y - AverageSig[index]/2,
		  AverageSig[index],AverageSig[index]);	  
	 painter.drawEllipse(Rectangle);
  }
  for (int ch = 0; ch < mBadChannels.size(); ch++)
  {
	  int Transparency= floor(255-7*AverageSig[mBadChannels[ch]]);
	  if (Transparency<0)
		  Transparency=0;
	  painter.setBrush(QColor(255,0,0,Transparency));
	  Rectangle = QRectF(mCenters[mBadChannels[ch]].x -  AverageSig[mBadChannels[ch]]/2,
		  mCenters[mBadChannels[ch]].y - AverageSig[mBadChannels[ch]]/2,
		  AverageSig[mBadChannels[ch]],AverageSig[mBadChannels[ch]]);	 
	  painter.drawEllipse(Rectangle);
  }
  //draw avg dot
  double AvgSize = 0;
  for (int ch = 0; ch < mNum_centers; ch++)
	  AvgSize+=AverageSig[ch];
  AvgSize/=mNum_centers;
  painter.setBrush(QColor(0,255,255,255));
   Rectangle = QRect(30 - AvgSize/2,45 - AvgSize/2,AvgSize,AvgSize);
   painter.drawEllipse(Rectangle);
   QString tr = "Avg Sig\n";
   tr+=QString::number(AvgSize);
   tr+="uV";
   painter.drawText(TextRect, Qt::AlignCenter, tr);
   mVis.SendReferenceFrame(mImage);
}

ChannelVisFilter::ChannelVisFilter()
: p(new Private)
{
}

ChannelVisFilter::~ChannelVisFilter()
{
  delete p;
}



void ChannelVisFilter::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS
  "Filtering:ChannelVisFilter int NoiseWindowWidth= 500 % 0 % // native image width",
  "Filtering:ChannelVisFilter int NoiseWindowHeight= 500 % 0 % // native image height",
  "Filtering:ChannelVisFilter int NoiseWindowBackground= 0x808080 % // image background color (color)",
  "Filtering:ChannelVisFilter int NoiseWindowDecimation= 4 % 1 % // update on every nth frame only",
  "Filtering:ChannelVisFilter float NoiseScaleFactor= 20 20 1 % // the proportion of the size of blue dot to input ",
  "Filtering:ChannelVisFilter matrix ElectrodeLocation= "
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

void ChannelVisFilter::Preflight(
  const SignalProperties& Input,
        SignalProperties& Output) const
{
  Output = Input;
  Parameter("NoiseWindowBackground");
  Parameter("ElectrodeLocation");
  ParamRef FrequencyCenter = Parameter( "FrequencyCenter" );
  PARAM(BadChList);
}

void ChannelVisFilter::Initialize(
  const SignalProperties& Input,
  const SignalProperties& Output)
{

  string WindowName = "Signal Noise (";
  ParamRef FrequencyCenter = Parameter( "FrequencyCenter" );
  for (int index = 0; index < FrequencyCenter->NumRows(); index++)
  {
	  WindowName+=(" "+FrequencyCenter->RowLabels()[index]);
  }
  WindowName+=" )";
  p->mVis.SetVisID(WindowName);
  p->mWidth = Parameter("NoiseWindowWidth");
  p->mHeight = Parameter("NoiseWindowHeight");
  p->mBackground = RGBColor(Parameter("NoiseWindowBackground"));
  p->mDecimation = Parameter("NoiseWindowDecimation");
  p->initVisualization();
  p->mNum_centers = Parameter("ElectrodeLocation")->NumRows();
  p->mPropotionToSig = Parameter("NoiseScaleFactor");

  for (int index = 0; index < p->mNum_centers; index++)
  {
	  ChannelVisFilter::Private::Corrdinate Center;
	  Center.x = atof(Parameter("ElectrodeLocation")->Value(index,1).c_str()) /20 +250;
      Center.y = atof(Parameter("ElectrodeLocation")->Value(index,2).c_str()) /20 +250;
	  p->mCenters.push_back(Center);
	 
  }

  PARAM(BadChList);
  p->mBadChannels.clear();
  for (int ch = 0; ch < BadChList->NumValues(); ch++)
  {
	  p->mBadChannels.push_back(BadChList(ch)-1);
	  bciout<<"bad channel"<<(BadChList(ch)-1)<<endl;
  }
  

}

void ChannelVisFilter::Process(
  const GenericSignal& Input,
        GenericSignal& Output)
{

	 
  if(p->mDecimationCounter == 0)
    p->updateVisualization(Input);
  ++p->mDecimationCounter %= p->mDecimation;
  

 
}

void ChannelVisFilter::StartRun()
{
	
  p->mDecimationCounter = 0;
  p->initVisualization();
}

void ChannelVisFilter::StopRun()
{
  p->initVisualization();
}