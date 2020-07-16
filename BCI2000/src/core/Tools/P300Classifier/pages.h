#ifndef PAGES_H
#define PAGES_H

#include <QtWidgets>
#include <QTextStream>
#include <QThread>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <time.h>

#include "ReadIniParameters.h"
#include "BCI2000FileReader.h"
#include "BCIStream.h"

#include "ap.h"
#include "load_BCIdat.h"
#include "CARfilter.h"
#include "GetP3Responses.h"
#include "SWLDA.h"
#include "GetScore.h"
#include "P300_Classify.h"
#include "StimulusPresentation_Classify.h"
#include "CDataFile.h"
//#include "get_cmd_parameters.h"
//#include "Check_Consistency_Files.h"
#include "WritePRM.h"

class DataPage;
class ParametersPage;
class DetailsPage;
class GenerateFeatureWeightsThread;
class ApplyFeatureWeightsThread;


using namespace std;

class ApplyFeatureWeightsThread : public QThread
{

 Q_OBJECT

public:
	void run();

signals:
	void signalProgressBar(int done, int total, int stage);
	void signalProgressText(QString message);

};

class GenerateFeatureWeightsThread : public QThread
{

 Q_OBJECT

public:
	void run();
    void signalProgressTextCallBack(QString message);

signals:
	void signalProgressBar(int done, int total, int stage);
	void signalProgressText(QString message);
	void errorMessage(QString errorMessage);
};

class DataPage : public QWidget
{
	Q_OBJECT

public:

    DataPage(QWidget *parent = 0);

	QGroupBox       *TrainingDataGroup;
    QGroupBox       *TestingDataGroup;
    QGroupBox       *inicfgGroup;
	QGroupBox		*WritePRMGroup;
	QGroupBox		*OverAllProgressGroup;

    QVBoxLayout     *TrainingDataLayout;
    QVBoxLayout     *TestingDataLayout;
    QVBoxLayout     *inicfgLayout;
	QVBoxLayout		*WritePRMLayout;
	QVBoxLayout		*OverAllProgressLayout;

    QLineEdit       *inicfgFileLineEdit;
	QLineEdit       *WritePRMLineEdit;

    QPushButton     *LoadTrainingDataFilesButton;
    QPushButton     *LoadTestingDataFilesButton;
    QPushButton     *GenerateFeatureWeights;
	QPushButton     *ApplyFeatureWeights;
	QPushButton		*LoadIniFileButton;
	QPushButton		*FileWritePRMButton;

	QProgressBar	*progressBarOverall;

	QListWidget		*TrainingDataFilesList;
	QListWidget		*TestingDataFilesList;

	class GenerateFeatureWeightsThread	 GenerateFeatureWeightsThread;
	class ApplyFeatureWeightsThread		 ApplyFeatureWeightsThread;

	QString			pathTrainingData;
	QString			pathTestingData;
	QString			pathIniData;

	vector<string>	fPathArr_TrainingData;
	vector<string>	fPathArr_TestingData;
	int				numSamples_TrainingData;
	int				numChannels_TrainingData;
	int				mode_TrainingData;
	int				numSamples_TestingData;
	int				numChannels_TestingData;
	int				mode_TestingData;
	bool			IfMUD;
	bool			IfTrueTrainingDataFiles;
	bool			IfTrueTestingDataFiles;
	bool			IfGenerateFeatureWeightsThread;
	bool			IfMultipleMenusTrainingData;
	bool			IfMultipleMenusTestingData;
	bool			mAutoWrite;

	// structures
	InitialParameter IniParam;
	parameters tMUD;

	void setParametersPage(ParametersPage *parametersPage);

private:
	ParametersPage *pParametersPage;


public slots:
	void LoadSettings();
	void ValidateSettings();
	void LoadTrainingButton();
	void LoadTestingButton();
	void LoadIniButton();
	void ReadVector(QString inputvector, QVector<int> *poutputvector);
	bool ValidateTrainingDataFiles(vector<string> &fPathArr, int &numSamples, int &numChannels, int &mode);
	bool ValidateTestingDataFiles(vector<string> &fPathArr, int &numSamples, int &numChannels, int &mode);
	void GenerateFeatures();
	void ApplyFeatures();
    void slotProgressText(QString message);
	void slotProgressBar(int done, int total, int stage);
	void slotErrorMessageDialog(QString errorMessage);
	void WriteParameterFragment( bool showDialog = true );
	bool ValidateLineEdit(QLineEdit *lineEdit);
	void ShowToolTip(QLineEdit *lineEdit, QString message);
	void MarkEditColor(QLineEdit *lineEdit, bool result);

private:
  bool ValidateStimulusFrequencies( BCI2000FileReader&, std::string& inFileName, std::ostream& ioErrors );
};

class ParametersPage : public QWidget
{
	Q_OBJECT

public:
    ParametersPage(QWidget *parent = 0);

	QLineEdit			*RespWindLineEdit;
	QLineEdit			*DecFreLineEdit;
	QLineEdit			*MaxModFeaLineEdit;
	QLineEdit			*ChSetLineEdit;
	QLineEdit			*PenterLineEdit;
	QLineEdit			*PremoveLineEdit;

	QComboBox			*SpatFilterComboBox;

	QLabel				*RespWindLabel;
	QLabel				*DecFreLabel;
	QLabel				*MaxModFeaLabel;
	QLabel				*QLabelChSetLabel;
	QLabel				*PenterLabel;
	QLabel				*PremoveLabel;
	QLabel				*SpatFilterLabel;
	QLabel				*ChSetLabel;

    QGroupBox *ClassifierSettingsGroup;
	QGroupBox *FilterSettingsGroup;
	QGroupBox *SpatialSettingsGroup;
	QGroupBox *TemporalSettingsGroup;

	void setDataPage(DataPage *dataPage);

public slots:
	void TextHasChanged();
	void ChSetTextHasChanged();


private:
	DataPage *pDataPage;


};

class DetailsPage : public QWidget
{
	Q_OBJECT

public:
    DetailsPage(QWidget *parent = 0);
    void Clear();

	QGroupBox		*ProgressGroup;
	QGroupBox		*FileInfoGroup;

	QLabel		*SamplingRateLabel;
	QLabel		*SourChListLabel;
	QLabel		*ClassifierLabel;
	QLabel		*ApplicationFilterChainLabel;
	QLabel		*InterpretModeLabel;
	QLabel		*TotalNumberofSamplesLabel;

	QLineEdit	*SamplingRateLineEdit;
	QLineEdit	*SourChListLineEdit;
	QLineEdit	*ClassifierLineEdit;
	QLineEdit	*ApplicationFilterChainLineEdit;
	QLineEdit	*InterpretModeLineEdit;
	QLineEdit	*TotalNumberofSamplesLineEdit;

	QVBoxLayout		*OverAllProgressLayout;
	QGridLayout		*FileInfoLayout;

	QTextEdit		*ProgressTextEdit;

	QProgressBar	*progressBarOverall;

};

#endif

