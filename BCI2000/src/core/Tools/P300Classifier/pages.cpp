#include "configdialog.h"
#include "pages.h"
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <set>
#include "FileUtils.h"
#include "ProcessUtils.h"

GenerateFeatureWeightsThread *g_pGenerateFeatureWeightsThread = NULL;
DataPage *g_pDataPage = NULL;
ParametersPage *g_pParametersPage = NULL;
DetailsPage *g_pDetailsPage = NULL;
QErrorMessage *errorMessageDialog = NULL;

// jm, Oct 24, 2012
static int FixupStimulusCodes( ap::template_1d_array<unsigned short int, true>& ioCodes )
{
  set<int> codes;
  for( int i = 0; i <= ioCodes.gethighbound(0); ++i ) 
    codes.insert( ioCodes( i ) );
  codes.erase( 0 );
  if( !codes.empty() )
  {
    int minCode = *codes.begin();
    if( minCode > 1 )
      for( int i = 0; i <= ioCodes.gethighbound(0); ++i ) 
        ioCodes( i ) -= ( minCode - 1 );
  }
  return static_cast<int>( codes.size() );
}

static void Reset()
{
  g_pDetailsPage->Clear();
  g_pDataPage->findChild<QProgressBar*>()->setValue( 0 );
}

static ostream&
PrintClassificationResults( ostream& oss, const vector<string>& predicted, const vector<double>& vresult )
{
  if( vresult.empty() )
    return oss << "No classification results available (unknown target specification).\n\n";

  // Display classification results
  std::ios_base::fmtflags savedFlags = oss.flags();
  oss.precision(0);
  oss.setf(ios::fixed, ios::floatfield);
  oss.setf(ios::left, ios::adjustfield);
  int width = static_cast<int>(predicted.size())/static_cast<int>(vresult.size());
  oss << "\nClassifying Responses...\n\n";
  oss << setw(12) << "Flashes";
  oss << setw(12) << "% Correct";
  oss << setw(width) << "Predicted Symbols\n";

  for (size_t k=0; k<vresult.size(); k++)
  {
    oss << setw(12) << k+1;
    oss << setw(12) << vresult[k];
    for (size_t j=k*width; j<(k+1)*width; j++)
      oss << predicted[j];
    oss << "\n";
  }
  oss.flags( savedFlags );
  return oss << "\n";
}

// end jm

void callback_status(string message)
{
  if (g_pGenerateFeatureWeightsThread)
    emit g_pGenerateFeatureWeightsThread->signalProgressTextCallBack(message.c_str());
}

DataPage::DataPage(QWidget *parent)
  : QWidget(parent),
    mAutoWrite( false )
{
  errorMessageDialog = new QErrorMessage(this);

  // Groups
  TrainingDataGroup      = new QGroupBox(tr("[-TrainingDataFiles] Training Data Files"));
  TestingDataGroup       = new QGroupBox(tr("[-TestingDataFiles] Testing Data Files"));
  inicfgGroup            = new QGroupBox(tr("[-inicfg] Feature Extraction INI File"));
  WritePRMGroup          = new QGroupBox(tr("[-ClassifierOutputFile] Write *.prm File"));
  OverAllProgressGroup   = new QGroupBox(tr("Overall Progress"));

  // objects
  inicfgFileLineEdit    = new QLineEdit;
  WritePRMLineEdit      = new QLineEdit;
  inicfgFileLineEdit    ->setReadOnly(true);
  WritePRMLineEdit      ->setReadOnly(false);

  progressBarOverall  = new QProgressBar();
  progressBarOverall    ->setRange(0,100);
  progressBarOverall    ->setTextVisible(false);
  progressBarOverall    ->setValue(0);

  LoadTrainingDataFilesButton   = new QPushButton(tr("Load Training Data Files"));
  LoadTestingDataFilesButton    = new QPushButton(tr("Load Testing Data Files"));
  LoadIniFileButton             = new QPushButton(tr("Load Ini File"));
  GenerateFeatureWeights        = new QPushButton(tr("Generate Feature Weights"));
  ApplyFeatureWeights           = new QPushButton(tr("Apply Feature Weights"));
  FileWritePRMButton            = new QPushButton(tr("Write *.prm File"));
  GenerateFeatureWeights        ->setEnabled(false);
  ApplyFeatureWeights           ->setEnabled(false);
  FileWritePRMButton            ->setEnabled(false);

  TrainingDataFilesList   = new QListWidget;

  TestingDataFilesList    = new QListWidget;

  // Conect the signals
  connect(LoadTrainingDataFilesButton,  SIGNAL(clicked()),this, SLOT(LoadTrainingButton()));
  connect(LoadTestingDataFilesButton,   SIGNAL(clicked()),this, SLOT(LoadTestingButton()));
  connect(LoadIniFileButton,            SIGNAL(clicked()),this, SLOT(LoadIniButton()));
  connect(GenerateFeatureWeights,       SIGNAL(clicked()),this, SLOT(GenerateFeatures()));
  connect(ApplyFeatureWeights,          SIGNAL(clicked()),this, SLOT(ApplyFeatures()));
  connect(FileWritePRMButton,           SIGNAL(clicked()),this, SLOT(WriteParameterFragment()));

  connect(&GenerateFeatureWeightsThread,  SIGNAL(signalProgressText(QString)),this,  SLOT(slotProgressText(QString)));
  connect(&GenerateFeatureWeightsThread,  SIGNAL(signalProgressBar(int, int, int)),this,  SLOT(slotProgressBar(int, int, int)));
  connect(&GenerateFeatureWeightsThread,  SIGNAL(errorMessage(QString)),this,  SLOT(slotErrorMessageDialog(QString)));

  connect(&ApplyFeatureWeightsThread, SIGNAL(signalProgressText(QString)),this,  SLOT(slotProgressText(QString)));
  connect(&ApplyFeatureWeightsThread, SIGNAL(signalProgressBar(int, int, int)),this,  SLOT(slotProgressBar(int, int, int)));


  // layout manager
  TrainingDataLayout     = new QVBoxLayout;
  TestingDataLayout      = new QVBoxLayout;
  inicfgLayout           = new QVBoxLayout;
  WritePRMLayout         = new QVBoxLayout;
  OverAllProgressLayout  = new QVBoxLayout;

  // wire them together
  TrainingDataLayout    ->addWidget(TrainingDataFilesList);
  TrainingDataLayout    ->addSpacing(12);
  TrainingDataLayout    ->addWidget(LoadTrainingDataFilesButton);
  TestingDataLayout     ->addWidget(TestingDataFilesList);
  TestingDataLayout     ->addSpacing(12);
  TestingDataLayout     ->addWidget(LoadTestingDataFilesButton);
  inicfgLayout          ->addWidget(inicfgFileLineEdit);
  inicfgLayout          ->addSpacing(12);
  inicfgLayout          ->addWidget(LoadIniFileButton);
  WritePRMLayout        ->addWidget(WritePRMLineEdit);
  WritePRMLayout        ->addWidget(FileWritePRMButton);
  OverAllProgressLayout ->addWidget(progressBarOverall);
  OverAllProgressLayout ->addWidget(GenerateFeatureWeights);
  OverAllProgressLayout ->addWidget(ApplyFeatureWeights);

  TrainingDataGroup     ->setLayout(TrainingDataLayout);
  TestingDataGroup      ->setLayout(TestingDataLayout);
  inicfgGroup           ->setLayout(inicfgLayout);
  WritePRMGroup         ->setLayout(WritePRMLayout);
  OverAllProgressGroup  ->setLayout(OverAllProgressLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout  ->addWidget(TrainingDataGroup);
  mainLayout  ->addWidget(TestingDataGroup);
  mainLayout  ->addWidget(inicfgGroup);
  mainLayout  ->addSpacing(12);
  mainLayout  ->addWidget(OverAllProgressGroup);
  mainLayout  ->addSpacing(12);
  mainLayout  ->addWidget(WritePRMGroup);
  mainLayout  ->addStretch(1);
  setLayout(mainLayout);

  g_pDataPage = this;
  g_pGenerateFeatureWeightsThread = &GenerateFeatureWeightsThread;

  pathIniData = QString::fromLocal8Bit( ProcessUtils::InitialProcessWD().c_str() );
  string path = FileUtils::InstallationDirectory() +  "../../data";
  if( !FileUtils::IsDirectory( path ) )
    path = FileUtils::InstallationDirectory();
  pathTrainingData = QString::fromLocal8Bit( path.c_str() );
  pathTestingData = pathTrainingData;
}

ParametersPage::ParametersPage(QWidget *parent)
  : QWidget(parent)
{
  // groups
  ClassifierSettingsGroup = new QGroupBox(tr("Classifier Settings"));
  FilterSettingsGroup     = new QGroupBox(tr("Filter Settings"));
  SpatialSettingsGroup    = new QGroupBox(tr("Spatial Settings"));
  TemporalSettingsGroup   = new QGroupBox(tr("Temporal Settings"));

  // objects
  RespWindLineEdit  = new QLineEdit;
  DecFreLineEdit    = new QLineEdit;
  MaxModFeaLineEdit = new QLineEdit;
  ChSetLineEdit     = new QLineEdit;
  PenterLineEdit    = new QLineEdit;
  PremoveLineEdit   = new QLineEdit;

  SpatFilterComboBox  = new QComboBox;
  SpatFilterComboBox  ->addItem(tr("Raw"));
  SpatFilterComboBox  ->addItem(tr("CAR"));

  RespWindLabel     = new QLabel(tr("Response Window [begin end] (ms):"));
  DecFreLabel       = new QLabel(tr("Decimation Frequency (Hz):"));
  MaxModFeaLabel    = new QLabel(tr("Max Model Features:"));
  ChSetLabel        = new QLabel(tr("Channel Set:"));
  PenterLabel       = new QLabel(tr("Penter:"));
  PremoveLabel      = new QLabel(tr("Premove:"));
  SpatFilterLabel   = new QLabel(tr("Spatial Filter:"));

  // regular expressions
  QRegExp regExpFloat         ("([0]+)\\.{,1}([0-9]*)");
  QRegExp regExpInt           ("(([0-9]|[1-9][0-9]|[1-9][0-9][0-9])\\s)*");
  QRegExp regExpIntMF         ("^([1-9]|[1-9][0-9]|[1-9][0-9][0-9])$");
  QRegExp regExpIntDF         ("^([1-9]|[1-9][0-9])$");
  QRegExp regExpIntRW         ("((([0-9])|([1-9][0-9])|([1-9][0-9][0-9])|([1-9][0-9][0-9][0-9]))\\s(([1-9])|([1-9][0-9])|([1-9][0-9][0-9])|([1-9][0-9][0-9][0-9])))");

  // validators
  QValidator *validatorMaxModelFeatures = new QRegExpValidator(regExpIntMF, MaxModFeaLineEdit);
  QValidator *validatorDecFreq          = new QRegExpValidator(regExpIntDF, DecFreLineEdit);
  QValidator *validatorChSet            = new QRegExpValidator(regExpInt, ChSetLineEdit);
  QValidator *validatorPenter           = new QRegExpValidator(regExpFloat, PenterLineEdit);
  QValidator *validatorPremove          = new QRegExpValidator(regExpFloat, PremoveLineEdit);
  QValidator *validatorRespWind         = new QRegExpValidator(regExpIntRW, RespWindLineEdit);

  MaxModFeaLineEdit                 ->setValidator(validatorMaxModelFeatures);
  DecFreLineEdit                    ->setValidator(validatorDecFreq);
  ChSetLineEdit                     ->setValidator(validatorChSet);
  PenterLineEdit                    ->setValidator(validatorPenter);
  PremoveLineEdit                   ->setValidator(validatorPremove);
  RespWindLineEdit                  ->setValidator(validatorRespWind);

  // track changes
  connect(MaxModFeaLineEdit,      SIGNAL(textEdited(const QString &)),this, SLOT(TextHasChanged()));
  connect(DecFreLineEdit,         SIGNAL(textEdited(const QString &)),this, SLOT(TextHasChanged()));
  connect(PenterLineEdit,         SIGNAL(textEdited(const QString &)),this, SLOT(TextHasChanged()));
  connect(PremoveLineEdit,        SIGNAL(textEdited(const QString &)),this, SLOT(TextHasChanged()));
  connect(RespWindLineEdit,       SIGNAL(textEdited(const QString &)),this, SLOT(TextHasChanged()));
  connect(SpatFilterComboBox,     SIGNAL(activated(int)),this, SLOT(TextHasChanged()));
  connect(ChSetLineEdit,          SIGNAL(textEdited(const QString &)),this, SLOT(ChSetTextHasChanged()));

  // layout manager
  QGridLayout *ClassifierLayout  = new QGridLayout;
  QGridLayout *FilterLayout      = new QGridLayout;
  QGridLayout *SpatialLayout     = new QGridLayout;
  QGridLayout *TemporalLayout    = new QGridLayout;

  // wire them together
  ClassifierLayout  ->addWidget(MaxModFeaLabel,     0, 0);
  ClassifierLayout  ->addWidget(MaxModFeaLineEdit,  0, 1);
  ClassifierLayout  ->addWidget(PenterLabel,        2, 0);
  ClassifierLayout  ->addWidget(PenterLineEdit,     2, 1);
  ClassifierLayout  ->addWidget(PremoveLabel,       3, 0);
  ClassifierLayout  ->addWidget(PremoveLineEdit,    3, 1);

  FilterLayout      ->addWidget(SpatFilterLabel,    0, 0);
  FilterLayout      ->addWidget(SpatFilterComboBox, 0, 1);
  FilterLayout      ->addWidget(DecFreLabel,        1, 0);
  FilterLayout      ->addWidget(DecFreLineEdit,     1, 1);

  SpatialLayout     ->addWidget(ChSetLabel,         0, 0);
  SpatialLayout     ->addWidget(ChSetLineEdit,      0, 1);

  TemporalLayout    ->addWidget(RespWindLabel,      0, 0);
  TemporalLayout    ->addWidget(RespWindLineEdit,   0, 1);

  ClassifierSettingsGroup ->setLayout(ClassifierLayout);
  FilterSettingsGroup     ->setLayout(FilterLayout);
  SpatialSettingsGroup    ->setLayout(SpatialLayout);
  TemporalSettingsGroup   ->setLayout(TemporalLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(ClassifierSettingsGroup);
  mainLayout->addWidget(FilterSettingsGroup);
  mainLayout->addWidget(SpatialSettingsGroup);
  mainLayout->addWidget(TemporalSettingsGroup);

  setLayout(mainLayout);

  g_pParametersPage = this;

}

DetailsPage::DetailsPage(QWidget *parent)
  : QWidget(parent)
{
  // Groups
  ProgressGroup           = new QGroupBox(tr("Overall Progress"));
  FileInfoGroup           = new QGroupBox(tr("BCI2000 FileInfo"));

  // Progress Bar
  progressBarOverall  = new QProgressBar();
  progressBarOverall  ->setRange(0,100);
  progressBarOverall  ->setTextVisible(false);
  progressBarOverall  ->setValue(0);

  // Text Edit
  ProgressTextEdit    = new QTextEdit;
  ProgressTextEdit    ->setReadOnly(true);
  ProgressTextEdit    ->setCurrentFont(QFont("Courier", 10));

  // Labels
  SamplingRateLabel            = new QLabel(tr("Sampling Rate (Hz):"));
  SourChListLabel              = new QLabel(tr("Number of Channels:"));
  ClassifierLabel              = new QLabel(tr("Classifier:"));
  ApplicationFilterChainLabel  = new QLabel(tr("Application:"));
  InterpretModeLabel           = new QLabel(tr("Interpret Mode:"));
  TotalNumberofSamplesLabel    = new QLabel(tr("Duration (s):"));

  // Line Edit
  SamplingRateLineEdit            = new QLineEdit;
  SourChListLineEdit              = new QLineEdit;
  ClassifierLineEdit              = new QLineEdit;
  ApplicationFilterChainLineEdit  = new QLineEdit;
  InterpretModeLineEdit           = new QLineEdit;
  TotalNumberofSamplesLineEdit    = new QLineEdit;

  SamplingRateLineEdit            ->setReadOnly(true);
  SourChListLineEdit              ->setReadOnly(true);
  ClassifierLineEdit              ->setReadOnly(true);
  ApplicationFilterChainLineEdit  ->setReadOnly(true);
  InterpretModeLineEdit           ->setReadOnly(true);
  TotalNumberofSamplesLineEdit    ->setReadOnly(true);

  // Layouts
  OverAllProgressLayout  = new QVBoxLayout;
  FileInfoLayout       = new QGridLayout;

  // wire them together
  OverAllProgressLayout ->addWidget(progressBarOverall);
  OverAllProgressLayout ->addWidget(ProgressTextEdit);
  FileInfoLayout        ->addWidget(SamplingRateLabel, 0, 0);
  FileInfoLayout        ->addWidget(SamplingRateLineEdit, 0, 1);
  FileInfoLayout        ->addWidget(SourChListLabel, 1, 0);
  FileInfoLayout        ->addWidget(SourChListLineEdit, 1, 1);
  FileInfoLayout        ->addWidget(ClassifierLabel, 2, 0);
  FileInfoLayout        ->addWidget(ClassifierLineEdit, 2, 1);
  FileInfoLayout        ->addWidget(ApplicationFilterChainLabel, 3, 0);
  FileInfoLayout        ->addWidget(ApplicationFilterChainLineEdit, 3, 1);
  FileInfoLayout        ->addWidget(InterpretModeLabel, 4, 0);
  FileInfoLayout        ->addWidget(InterpretModeLineEdit, 4, 1);
  FileInfoLayout        ->addWidget(TotalNumberofSamplesLabel, 5, 0);
  FileInfoLayout        ->addWidget(TotalNumberofSamplesLineEdit, 5, 1);

  ProgressGroup     ->setLayout(OverAllProgressLayout);
  FileInfoGroup     ->setLayout(FileInfoLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout  ->addWidget(FileInfoGroup);
  mainLayout  ->addWidget(ProgressGroup);

  setLayout(mainLayout);

  g_pDetailsPage = this;
}

void
DetailsPage::Clear()
{
  QList<QLineEdit*> fields = findChildren<QLineEdit*>();
  for( int i = 0; i < fields.size(); ++i )
    fields[i]->setText( "" );
  QList<QTextEdit*> fields2 = findChildren<QTextEdit*>();
  for( int i = 0; i < fields2.size(); ++i )
    fields2[i]->setText( "" );
  findChild<QProgressBar*>()->setValue( 0 );
}


void ParametersPage::setDataPage(DataPage *dataPage)
{
  pDataPage = dataPage;
}

void DataPage::setParametersPage(ParametersPage *parametersPage)
{
  pParametersPage = parametersPage;
}

void ParametersPage::TextHasChanged()
{
  if (pDataPage->IfGenerateFeatureWeightsThread)
    pDataPage->ApplyFeatureWeights->setEnabled(false);

  pDataPage->ValidateSettings();
}

void ParametersPage::ChSetTextHasChanged()
{
  if (pDataPage->IfGenerateFeatureWeightsThread)
    pDataPage->ApplyFeatureWeights->setEnabled(false);

  QString ch  = this->ChSetLineEdit->text();
  QVector<int> *ChSet = new QVector<int>;
  pDataPage->ReadVector(ch, ChSet);
  pDataPage->IniParam.channel_set.clear();

  if (ChSet->size() > 0)
  {
    for (int i=0; i<ChSet->size(); i++)
      if ((ChSet->at(i) > pDataPage->numChannels_TrainingData) | (ChSet->at(i) == 0))
      {
        ostringstream oss;
        oss << "The Channel Set provided is not a subset of the channels contained in the input file";
        pDataPage->ShowToolTip(ChSetLineEdit, oss.str().c_str());
        pDataPage->MarkEditColor(ChSetLineEdit, false);
        pDataPage->GenerateFeatureWeights->setEnabled(false);
        return;
      }
      else
      {
        pDataPage->MarkEditColor(ChSetLineEdit, true);
        pDataPage->IniParam.channel_set.push_back(ChSet->at(i));
        pDataPage->GenerateFeatureWeights->setEnabled(true);
      }
  }
  else
  {
    ostringstream oss;
    oss << "Channel Set must have at least one channel.";
    pDataPage->ShowToolTip(ChSetLineEdit, oss.str().c_str());
    pDataPage->MarkEditColor(ChSetLineEdit, false);
    pDataPage->GenerateFeatureWeights->setEnabled(false);
  }

  if (pDataPage->fPathArr_TrainingData.size() == 0)
    pDataPage->GenerateFeatureWeights->setEnabled(false);

  if (pDataPage->fPathArr_TrainingData.size()>0)
    if (!pDataPage->IfTrueTrainingDataFiles)
    {
      ostringstream oss;
      oss << "Load the Training Data Files again";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
        pDataPage->GenerateFeatureWeights->setEnabled(false);
    }
}

void DataPage::LoadSettings()
{
  Reset();
  
  QString inicfgFile = this->inicfgFileLineEdit->text();
  string szFile = this->inicfgFileLineEdit->text().toLocal8Bit().constData();
  char format = 'f';
  int precision = 4;

  bool IsFile = ReadIniParameters(szFile, IniParam);

  if (!IsFile)
  {
    IniParam.maxiter = 60;
    IniParam.penter = 0.1000f;
    IniParam.premove = 0.1500f;
    IniParam.Decimation_Frequency = 20;
    for (int i=0; i<8; i++)
      IniParam.channel_set.push_back(i+1);

    QPalette palette(pParametersPage->ChSetLineEdit->palette() );
    palette.setColor( QPalette::Base, QColor("lightgreen") );
    pParametersPage->ChSetLineEdit->setPalette(palette);

    IniParam.windlen.push_back(0);
    IniParam.windlen.push_back(800);
    IniParam.SF = 1;
  }

  pParametersPage->MaxModFeaLineEdit->setText(QString::number(IniParam.maxiter));
  pParametersPage->PenterLineEdit->setText(QString::number(IniParam.penter, format, precision));
  pParametersPage->PremoveLineEdit->setText(QString::number(IniParam.premove, format, precision));
  pParametersPage->DecFreLineEdit->setText(QString::number(IniParam.Decimation_Frequency));

  QString chset;
  QString respwind;

  for (size_t i=0; i<static_cast<int>(IniParam.channel_set.size()); i++)
    if (i != static_cast<int>(IniParam.channel_set.size())-1)
      QTextStream(&chset) << IniParam.channel_set[i] << " ";
    else
      QTextStream(&chset) << IniParam.channel_set[i];

  for (size_t i=0; i<static_cast<int>(IniParam.windlen.size()); i++)
    QTextStream(&respwind) << IniParam.windlen[i] << " ";

  pParametersPage->ChSetLineEdit->setText(chset);
  pParametersPage->RespWindLineEdit->setText(respwind);

  if (IniParam.SF == 1)
    pParametersPage->SpatFilterComboBox->setCurrentIndex(0);

  if (IniParam.SF == 2)
    pParametersPage->SpatFilterComboBox->setCurrentIndex(1);
}

void DataPage::LoadTrainingButton()
{
  QStringList files = QFileDialog::getOpenFileNames(
                                this, tr("Load Training Data Files"),
                                pathTrainingData,
                                tr("Data Files (*.dat)"));

  if (files.count()>0)
  {
    Reset();
    this->TrainingDataFilesList->clear();
    fPathArr_TrainingData.clear();

    QString openFilesPath;
    for (int i=0; i<files.count(); i++)
    {
      openFilesPath = files[i];
      fPathArr_TrainingData.push_back(openFilesPath.toLocal8Bit().constData());
      this->TrainingDataFilesList->addItem(openFilesPath);
    }
    this->IfTrueTrainingDataFiles = ValidateTrainingDataFiles(fPathArr_TrainingData,
                    numSamples_TrainingData, numChannels_TrainingData, mode_TrainingData);
    if (!IfTrueTrainingDataFiles)
      GenerateFeatureWeights->setEnabled(false);
    else
      GenerateFeatureWeights->setEnabled(true);

    pathTrainingData = openFilesPath;

    pParametersPage->ChSetTextHasChanged();
    pParametersPage->TextHasChanged();
  }
}

void DataPage::LoadTestingButton()
{
  QString openFilesPath;
  QStringList files = QFileDialog::getOpenFileNames(
                                this, tr("Load Testing Data Files"),
                                pathTestingData,
                                tr("Data Files (*.dat)"));

  if (files.count()>0)
  {
    this->TestingDataFilesList->clear();
    fPathArr_TestingData.clear();

    for (int i=0; i<files.count(); i++)
    {
      openFilesPath = files[i];
      fPathArr_TestingData.push_back(openFilesPath.toLocal8Bit().constData());
      this->TestingDataFilesList->addItem(openFilesPath);
    }

    this->IfTrueTestingDataFiles = ValidateTestingDataFiles(fPathArr_TestingData,
                    numSamples_TestingData, numChannels_TestingData, mode_TestingData);

    if (!IfTrueTestingDataFiles)
      ApplyFeatureWeights->setEnabled(false);
    else
      if (IfMUD)
        ApplyFeatureWeights->setEnabled(true);

    pathTestingData = openFilesPath;
  }
}

void DataPage::LoadIniButton()
{
  QString filename;
  QString file = QFileDialog::getOpenFileName(
                                this, tr("Load Ini File"),
                                pathIniData,
                                tr("Ini Files (*.ini)"));
  if (file.count()>0)
  {
    inicfgFileLineEdit->setText(file);
    LoadSettings();
    ValidateSettings();
    pathIniData = file;
    if (IfMUD)
      ApplyFeatureWeights->setEnabled(false);
  }
}

bool DataPage::ValidateLineEdit(QLineEdit *lineEdit)
{
  bool result;

  QString text = lineEdit->text();
  const QValidator *validator = lineEdit->validator();

  if (validator) {
      int pos;
      if (validator->validate(text, pos) != QValidator::Acceptable) {
        result = false;
      } else {
        result = true;
      }
  } else {
    result = false;
  }

  //MarkEditColor(lineEdit,result);

  return result;
}

void DataPage::ShowToolTip(QLineEdit *lineEdit, QString message)
{
  QPoint pos(lineEdit->x(), lineEdit->y());
  pos.setX(lineEdit->parentWidget()->mapToGlobal(pos).x());
  pos.setY(lineEdit->parentWidget()->mapToGlobal(pos).y()+10);

  QRect rect(lineEdit->pos().x(), lineEdit->pos().y(), 100, 100);
  QToolTip::showText(pos, message, lineEdit, rect);
}

void DataPage::MarkEditColor(QLineEdit *lineEdit, bool result)
{
  if (result)
  {
    QPalette palette(lineEdit->palette() );
    palette.setColor( QPalette::Base, QColor("lightgreen") );
    lineEdit->setPalette(palette);
  }
  else
  {
    QPalette palette(lineEdit->palette() );
    palette.setColor( QPalette::Base, QColor("lightpink") );
    lineEdit->setPalette(palette);
  }
}

void DataPage::ValidateSettings()
{
  bool result = true;

  //result &= ValidateLineEdit(pParametersPage->MaxModFeaLineEdit);

  QString MaxModFeat  = pParametersPage->MaxModFeaLineEdit->text();
  QString penter    = pParametersPage->PenterLineEdit->text();
  QString premove   = pParametersPage->PremoveLineEdit->text();
  QString DecFre    = pParametersPage->DecFreLineEdit->text();
  QString RespWind  = pParametersPage->RespWindLineEdit->text();


  this->IniParam.SF = pParametersPage->SpatFilterComboBox->currentIndex() + 1;

  bool ok;
  bool validMaxModFeat, validPenterPremove, validDecFre, validRespWind;

  // Validate Max Model Features
  if (MaxModFeat.toInt(&ok, 10) <= 0)
  {
    ShowToolTip(pParametersPage->MaxModFeaLineEdit, tr("Max Model Features must be > 0"));
    MarkEditColor(pParametersPage->MaxModFeaLineEdit, false);
  }
  if (MaxModFeat == "")
  {
    ShowToolTip(pParametersPage->MaxModFeaLineEdit, tr("Max Model Features must have a value"));
    MarkEditColor(pParametersPage->MaxModFeaLineEdit, false);
  }
  if ((MaxModFeat.toInt(&ok, 10) <= 0) | (MaxModFeat == ""))
    validMaxModFeat = false;
  else
  {
    validMaxModFeat = true;
    this->IniParam.maxiter = MaxModFeat.toInt(&ok, 10);
    MarkEditColor(pParametersPage->MaxModFeaLineEdit, true);
  }

  // Validate penter and premove
  if ((penter.toFloat(&ok) > premove.toFloat(&ok)))
  {
    ShowToolTip(pParametersPage->PenterLineEdit, tr("Penter must be less than Premove"));
    MarkEditColor(pParametersPage->PenterLineEdit, false);
    MarkEditColor(pParametersPage->PremoveLineEdit, false);
  }
  if (penter == "")
  {
    ShowToolTip(pParametersPage->PenterLineEdit, tr("Penter must have a value"));
    MarkEditColor(pParametersPage->PenterLineEdit, false);
  }
  if (premove == "")
  {
    ShowToolTip(pParametersPage->PenterLineEdit, tr("Premove must have a value"));
    MarkEditColor(pParametersPage->PremoveLineEdit, false);
  }

  if ((penter.toFloat(&ok) > premove.toFloat(&ok)) | (penter == "") | (premove == ""))
    validPenterPremove = false;
  else
  {
    validPenterPremove = true;
    this->IniParam.penter = penter.toFloat(&ok);
    this->IniParam.premove = premove.toFloat(&ok);
    MarkEditColor(pParametersPage->PenterLineEdit, true);
    MarkEditColor(pParametersPage->PremoveLineEdit, true);
  }

  // Validate Decimation Frequency
  if (DecFre.toInt(&ok, 10) <= 0)
  {
    ShowToolTip(pParametersPage->DecFreLineEdit, tr("Decimation Frequency must be > 0"));
    MarkEditColor(pParametersPage->DecFreLineEdit, false);
  }
  if (DecFre == "")
  {
    ShowToolTip(pParametersPage->DecFreLineEdit, tr("Decimation Frequency must have a value"));
    MarkEditColor(pParametersPage->DecFreLineEdit, false);
  }

  if ((DecFre.toInt(&ok, 10) <= 0) | (DecFre == ""))
    validDecFre = false;
  else
  {
    validDecFre = true;
    this->IniParam.Decimation_Frequency = DecFre.toInt(&ok, 10);
    MarkEditColor(pParametersPage->DecFreLineEdit, true);
  }

  QVector<int> *wind = new QVector<int>;
  this->ReadVector(RespWind, wind);
  // Validate Response window
  this->IniParam.windlen.clear();
  if (wind->size() == 2)
  {
    if (wind->at(0) > wind->at(1))
    {
      ShowToolTip(pParametersPage->RespWindLineEdit, tr("[Begin] must be less than [End] in the Response Window"));
      MarkEditColor(pParametersPage->RespWindLineEdit, false);
      validRespWind = false;
    }
    else
    {
      validRespWind = true;
      this->IniParam.windlen.push_back(wind->at(0));
      this->IniParam.windlen.push_back(wind->at(1));
      MarkEditColor(pParametersPage->RespWindLineEdit, true);
    }
  }
  else
  {
    validRespWind = false;
    ShowToolTip(pParametersPage->RespWindLineEdit, tr("Response Window must have [Begin] and [End]"));
    MarkEditColor(pParametersPage->RespWindLineEdit, false);
  }

  if (validMaxModFeat && validPenterPremove && validDecFre && validRespWind)
    if (IfTrueTrainingDataFiles)
      this->GenerateFeatureWeights->setEnabled(true);
  else
    this->GenerateFeatureWeights->setEnabled(false);

  /*if((static_cast<int>(this->fPathArr_TrainingData.size()) == 0) && (static_cast<int>(this->fPathArr_TestingData.size()) == 0))
    this->GenerateFeatureWeights->setEnabled(false);*/

  if(static_cast<int>(this->fPathArr_TrainingData.size()) == 0)
    this->GenerateFeatureWeights->setEnabled(false);

}

void DataPage::ReadVector(QString inputvector, QVector<int> *poutputvector)
{
  int idx         = 0;
  bool success;

  if (inputvector.length() > 0)
  {
    QStringList elements = inputvector.simplified().split(QString(" "));
    poutputvector->resize(static_cast<int>(elements.size()));

    for (idx=0; idx<static_cast<int>(elements.size()); idx++)
    {
      (*poutputvector)[idx] = elements.value(idx).toInt(&success);

      if (success == false)
      {
        // report error
        break;
      }
    }
  }
  else
  {
      // report error
  }
}

bool DataPage::ValidateTrainingDataFiles(vector<string> &fPathArr, int &numSamples, int &numChannels, int &mode)
{
///////////////////////////////////////////////////////////////////////////////
// Section: Define variables
  int files, i;
  numSamples = 0;
  numChannels = 0;
  vector<int> channelsInFile;
  vector<int> samplesInFile;
  vector<int> SamplingRate;
  vector<int> NumberOfSequences;
  vector<int> NumMatrixRows;
  vector<int> NumMatrixColumns;
  vector<int> NumStimuli;
  vector<int> InterpretMode;
  vector<string> ApplicationFilterChain;
  vector<string> Experiment;

///////////////////////////////////////////////////////////////////////////////
// Section: Open BCI2000 File, check if the file is open, and check compatibility
// among the files
  BCI2000FileReader CurrentFile;
  Experiment.push_back("P3SpellerTask");
  Experiment.push_back("StimulusPresentationTask");

  // Check if the BCI2000 file exists
  fPathArr.clear();
  QListWidgetItem *item = new QListWidgetItem;
  for (files=0; files<this->TrainingDataFilesList->count(); files++)
  {
    item = this->TrainingDataFilesList->item(files);
    fPathArr.push_back(item->text().toLocal8Bit().constData());
    CurrentFile.Open(fPathArr[files].c_str());
    if( !CurrentFile.IsOpen() )
    {
      fPathArr[files] = fPathArr[files] + ".dat";
      CurrentFile.Open( fPathArr[files].c_str() );
    }
    if( !CurrentFile.IsOpen() )
    {
      ostringstream oss;
      oss << "Could not open: \n" << fPathArr[files].c_str() << "\" as a BCI2000 data file.";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
        this->GenerateFeatureWeights->setEnabled(false);
        return false;
      }
    }
    // Check whether states are compatible. There is a bug in this code. Fix it!!!! May 12, 2010
  // I need to check that all states are the same across all files.
  // try the following code
  // if (files == 0)
  //  const StateList* statelist = CurrentFile.States();
  // end
  // then for files>0, test it

    const StateList* statelist = CurrentFile.States();
    for (i=0; i<statelist->Size(); i++ )
      if( !CurrentFile.States()->Exists((*statelist).ByIndex(i).Name()))
      {
        ostringstream oss;
        oss << "Incompatible states information for " << fPathArr[files].c_str();
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }

    channelsInFile.push_back(CurrentFile.SignalProperties().Channels());
    samplesInFile.push_back(CurrentFile.NumSamples());

    // Check the Interpret Mode
    if (CurrentFile.Parameters()->Exists("InterpretMode"))
    {
      InterpretMode.push_back(CurrentFile.Parameter("InterpretMode"));
      if (InterpretMode[0] == 0)
      {
        ostringstream oss;
        oss << "The mode of data file " << fPathArr[files].c_str()<< " is not supported";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
    }
    else
    {
      ostringstream oss;
      oss << "Interpret mode of file " << fPathArr[files].c_str()<< " does not exist";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
        this->GenerateFeatureWeights->setEnabled(false);
        return false;
      }
    }

    // ********************************************************************
    // Modification Cristhian, Sep 11, 2009

    if(CurrentFile.States()->Exists("SelectedTarget") &&
       CurrentFile.States()->Exists("SelectedRow") &&
       CurrentFile.States()->Exists("SelectedColumn"))
    {
      ApplicationFilterChain.push_back("P3SpellerTask");
    }
    else if (CurrentFile.States()->Exists("SelectedStimulus"))
    {
      ApplicationFilterChain.push_back("StimulusPresentationTask");
    }
    else
    {
      ostringstream oss;
      oss << "The Application of data file " << fPathArr[files].c_str()<< " does not exist or is not supported.";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
        this->GenerateFeatureWeights->setEnabled(false);
        return false;
      }
    }
    // ********************************************************************

    // Check the Application Filter Chain
    /*if (CurrentFile.Parameters()->Exists("ApplicationFilterChain"))
    {
      ApplicationFilterChain.push_back(CurrentFile.Parameter("ApplicationFilterChain")(1));
      if ((ApplicationFilterChain[files] != Experiment[0]) && (ApplicationFilterChain[files] != Experiment[1]))
      {
        ostringstream oss;
        oss << "The Application of data file " << fPathArr[files].c_str()<< " is not supported. The aplications must be either 'P300SpellerTask' or 'StimulusPresentationTask'";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }

      }
    }
    else
    {
      ostringstream oss;
      oss << "Application Filter Chain of file " << fPathArr[files].c_str()<< " does not exist";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
        this->GenerateFeatureWeights->setEnabled(false);
        return false;
      }
    }*/

    // Check the Sampling Rate
    if (CurrentFile.Parameters()->Exists("SamplingRate"))
      SamplingRate.push_back(CurrentFile.SamplingRate());
    else
    {
      ostringstream oss;
      oss << "The sampling rate of file " << fPathArr[files].c_str()<< " does not exist";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
        this->GenerateFeatureWeights->setEnabled(false);
        return false;
      }
    }

	// Check the Parameter Number of Sequences - Modifiction Cristhian Potes - January 27, 2013
	if (CurrentFile.Parameters()->Exists("NumberOfSequences"))
	{
	}  
    else
    {
      ostringstream oss;
      oss << "The parameter: Number Of Sequences of file " << fPathArr[files].c_str()<< " does not exist";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
        this->GenerateFeatureWeights->setEnabled(false);
        return false;
      }
    }

    IfMultipleMenusTrainingData = false; // make sure it is set correctly for StimulusPresentation files, jm
    // Check the NumMatrixRows. Cristhian Modification Oct 9, 2009
    if (CurrentFile.Parameters()->Exists("NumMatrixRows"))
    {
      if (CurrentFile.Parameter("NumMatrixRows")->NumValues() != 1)
      {
        NumMatrixRows.push_back(0);
        IfMultipleMenusTrainingData = true;
      }
      else
      {
        NumMatrixRows.push_back(CurrentFile.Parameter("NumMatrixRows"));
        IfMultipleMenusTrainingData = false;
      }
    }
    else
    {
      if (ApplicationFilterChain[files] == Experiment[0])
      {
        ostringstream oss;
        oss << "The number of rows for the P3 Speller Task of file " << fPathArr[files].c_str()<< " does not exist";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
    }

    // Check the NumMatrixColumns
    if (CurrentFile.Parameters()->Exists("NumMatrixColumns"))
    {
      if (CurrentFile.Parameter("NumMatrixColumns")->NumValues() != 1)
      {
        NumMatrixColumns.push_back(0);
        IfMultipleMenusTrainingData = true;
      }
      else
      {
        NumMatrixColumns.push_back(CurrentFile.Parameter("NumMatrixColumns"));
        IfMultipleMenusTrainingData = false;
      }
    }
    else
    {
      if (ApplicationFilterChain[files] == Experiment[0])
      {
        ostringstream oss;
        oss << "The number of columns for the P3 Speller Task of file " << fPathArr[files].c_str()<< " does not exist";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
    }

    // Check the number of stimulus in the Stimulus Presentation Task
    if (CurrentFile.Parameters()->Exists("Stimuli"))
    {
      ParamRef parameter = CurrentFile.Parameter("Stimuli");
      NumStimuli.push_back(parameter->NumColumns());
    }
    else
    {
      if (ApplicationFilterChain[files] == Experiment[1])
      {
        ostringstream oss;
        oss << "The state stimuli for the Stimulus Presentation Task of file " << fPathArr[files].c_str()<< " does not exist";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
    }

    // Validate stimulus frequencies, jm Mar 21, 2011
    if( ApplicationFilterChain[files] == Experiment[1] )
    {
      ostringstream errors;
      if( !ValidateStimulusFrequencies( CurrentFile, fPathArr[ files ], errors ) )
      {
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), errors.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
    }

    if (files>0)
    {
      if (channelsInFile[files-1] != channelsInFile[files])
      {
        ostringstream oss;
        oss << "All input files must have the same Numbers of Channels";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TrainingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
          this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
      if (SamplingRate[files-1] != SamplingRate[files])
      {
        ostringstream oss;
        oss << "All input files must have the same Sampling Rate";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TrainingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
          this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
      if (InterpretMode[files-1] != InterpretMode[files])
      {
        ostringstream oss;
        oss << "All input files must have the same Interpret Mode";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TrainingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
          this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
      if (ApplicationFilterChain[files-1] != ApplicationFilterChain[files])
      {
        ostringstream oss;
        oss << "All input files must be recorded using the same experiment";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TrainingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
          this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
      if (ApplicationFilterChain[files] == Experiment[0])
        if (NumMatrixRows[files-1] != NumMatrixRows[files])
        {
          ostringstream oss;
          oss << "All input files must be recorded using the same experiment";
          QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
          oss.str().c_str(), 0, this);
          msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
          if (msgBox.exec() == QMessageBox::AcceptRole)
          {
            this->TrainingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
            this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
            this->GenerateFeatureWeights->setEnabled(false);
            return false;
          }
        }
      if (ApplicationFilterChain[files] == Experiment[0])
        if (NumMatrixColumns[files-1] != NumMatrixColumns[files])
        {
          ostringstream oss;
          oss << "All input files must be recorded using the same experiment";
          QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
          oss.str().c_str(), 0, this);
          msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
          if (msgBox.exec() == QMessageBox::AcceptRole)
          {
            this->TrainingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
            this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
            this->GenerateFeatureWeights->setEnabled(false);
            return false;
          }
        }
      if (ApplicationFilterChain[files] == Experiment[1])
        if (NumStimuli[files-1] != NumStimuli[files])
        {
          ostringstream oss;
          oss << "All input files must be recorded using the same experiment";
          QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
          oss.str().c_str(), 0, this);
          msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
          if (msgBox.exec() == QMessageBox::AcceptRole)
          {
            this->TrainingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
            this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
            this->GenerateFeatureWeights->setEnabled(false);
            return false;
          }
        }
    }
    numSamples += samplesInFile[files];
    this->TrainingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightgreen")));
  }

  // Check that the channel set provided is a subset of the channels contained in the input files
  numChannels = channelsInFile[0];
  QVector<int> *ChannelSet = new QVector<int>;
  QString Ch  = pParametersPage->ChSetLineEdit->text();
  this->ReadVector(Ch, ChannelSet);

  for (i=0; i<ChannelSet->size(); i++)
  {
    if (ChannelSet->at(i) > numChannels)
    {
      ostringstream oss;
      oss << "The Channel Set provided is not a subset of the channels contained in the input file. ";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      QPalette palette( pParametersPage->ChSetLineEdit->palette() );

      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        palette.setColor( QPalette::Base, QColor("lightpink") );
        pParametersPage->ChSetLineEdit->setPalette(palette);
        this->GenerateFeatureWeights->setEnabled(false);
        return false;
      }
    }
  }

  QPalette palette( pParametersPage->ChSetLineEdit->palette() );
  palette.setColor( QPalette::Base, QColor("lightgreen") );
  pParametersPage->ChSetLineEdit->setPalette(palette);
  this->GenerateFeatureWeights->setEnabled(true);

// Set mode according to the Interpret Mode and Application Filter Chain parameters
// mode = 1   ->    P3SpellerTask/Online Free Mode
// mode = 2   ->    P3SpellerTask/Copy Mode
// mode = 3   ->    StimulusPresentationTask/Online Free Mode
// mode = 4   ->    StimulusPresentationTask/Copy Mode

  if ((Experiment[0] == ApplicationFilterChain[0]) && InterpretMode[0] == 1)
    mode = 1;
  if ((Experiment[0] == ApplicationFilterChain[0]) && InterpretMode[0] == 2)
    mode = 2;
  if ((Experiment[1] == ApplicationFilterChain[0]) && InterpretMode[0] == 1)
    mode = 3;
  if ((Experiment[1] == ApplicationFilterChain[0]) && InterpretMode[0] == 2)
    mode = 4;


  // Fill the Detail Page with the BCI2000 FileInfo
  char format = 'f';
  int precision = 2;
  g_pDetailsPage->SamplingRateLineEdit->setText(QString::number(SamplingRate[0]));
  g_pDetailsPage->SourChListLineEdit->setText(QString::number(channelsInFile[0]));
  g_pDetailsPage->ClassifierLineEdit->setText(tr("Stepwise Linear Regression"));

  if (ApplicationFilterChain[0] == Experiment[0])
    g_pDetailsPage->ApplicationFilterChainLineEdit->setText(Experiment[0].c_str());

  if (ApplicationFilterChain[0] == Experiment[1])
    g_pDetailsPage->ApplicationFilterChainLineEdit->setText(Experiment[1].c_str());

  if (InterpretMode[0] == 1)
    g_pDetailsPage->InterpretModeLineEdit->setText(tr("Online Free Mode"));

  if (InterpretMode[0] == 2)
    g_pDetailsPage->InterpretModeLineEdit->setText(tr("Copy Mode"));

  g_pDetailsPage->TotalNumberofSamplesLineEdit->setText(QString::number(((double)numSamples/(double)SamplingRate[0]), format, precision));

  return true;
}


bool DataPage::ValidateTestingDataFiles(vector<string> &fPathArr, int &numSamples, int &numChannels, int &mode)
{
///////////////////////////////////////////////////////////////////////////////
// Section: Define variables
  int files, i;
  numSamples = 0;
  numChannels = 0;
  vector<int> channelsInFile;
  vector<int> samplesInFile;
  vector<int> SamplingRate;
  vector<int> NumberOfSequences;
  vector<int> NumMatrixRows;
  vector<int> NumMatrixColumns;
  vector<int> NumStimuli;
  vector<int> InterpretMode;
  vector<string> ApplicationFilterChain;
  vector<string> Experiment;

///////////////////////////////////////////////////////////////////////////////
// Section: Open BCI2000 File, check if the file is open, and check compatibility
// among the files
  BCI2000FileReader CurrentFile;
  Experiment.push_back("P3SpellerTask");
  Experiment.push_back("StimulusPresentationTask");

  // Check if the BCI2000 file exists
  fPathArr.clear();
  QListWidgetItem *item = new QListWidgetItem;
  for (files=0; files<this->TestingDataFilesList->count(); files++)
  {
    item = this->TestingDataFilesList->item(files);
    fPathArr.push_back(item->text().toLocal8Bit().constData());
    CurrentFile.Open(fPathArr[files].c_str());
    if( !CurrentFile.IsOpen() )
    {
      fPathArr[files] = fPathArr[files] + ".dat";
      CurrentFile.Open( fPathArr[files].c_str() );
    }
    if( !CurrentFile.IsOpen() )
    {
      ostringstream oss;
      oss << "Could not open: \n" << fPathArr[files].c_str() << "\" as a BCI2000 data file.";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
        this->GenerateFeatureWeights->setEnabled(false);
        return false;
      }
    }
    // Check whether states are compatible.
    const StateList* statelist = CurrentFile.States();
    for (i=0; i<statelist->Size(); i++ )
      if( !CurrentFile.States()->Exists((*statelist).ByIndex(i).Name()))
      {
        ostringstream oss;
        oss << "Incompatible states information for " << fPathArr[files].c_str();
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }

    channelsInFile.push_back(CurrentFile.SignalProperties().Channels());
    samplesInFile.push_back(CurrentFile.NumSamples());

    // Check the Interpret Mode
    if (CurrentFile.Parameters()->Exists("InterpretMode"))
    {
      InterpretMode.push_back(CurrentFile.Parameter("InterpretMode"));
      if (InterpretMode[0] == 0)
      {
        ostringstream oss;
        oss << "The mode of data file " << fPathArr[files].c_str()<< " is not supported";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
    }
    else
    {
      ostringstream oss;
      oss << "Interpret mode of file " << fPathArr[files].c_str()<< " does not exist";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
        this->ApplyFeatureWeights->setEnabled(false);
        return false;
      }
    }

    // ********************************************************************
    // Modification Cristhian, Sep 11, 2009

    if(CurrentFile.States()->Exists("SelectedTarget") &&
       CurrentFile.States()->Exists("SelectedRow") &&
       CurrentFile.States()->Exists("SelectedColumn"))
    {
      ApplicationFilterChain.push_back("P3SpellerTask");
    }
    else if (CurrentFile.States()->Exists("SelectedStimulus"))
    {
      ApplicationFilterChain.push_back("StimulusPresentationTask");
    }
    else
    {
      ostringstream oss;
      oss << "The Application of data file " << fPathArr[files].c_str()<< " does not exist or is not supported.";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
        this->GenerateFeatureWeights->setEnabled(false);
        return false;
      }
    }
    // ********************************************************************
    // Check the Application Filter Chain
    /*if (CurrentFile.Parameters()->Exists("ApplicationFilterChain"))
    {
      ApplicationFilterChain.push_back(CurrentFile.Parameter("ApplicationFilterChain")(1));
      if ((ApplicationFilterChain[files] != Experiment[0]) && (ApplicationFilterChain[files] != Experiment[1]))
      {
        ostringstream oss;
        oss << "The Application of data file " << fPathArr[files].c_str()<< " is not supported. The aplications must be either 'P300SpellerTask' or 'StimulusPresentationTask'";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->ApplyFeatureWeights->setEnabled(false);
          return false;
        }

      }
    }
    else
    {
      ostringstream oss;
      oss << "Application Filter Chain of file " << fPathArr[files].c_str()<< " does not exist";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
        this->GenerateFeatureWeights->setEnabled(false);
        return false;
      }
    }*/
    // ********************************************************************

    // Check the Sampling Rate
    if (CurrentFile.Parameters()->Exists("SamplingRate"))
      SamplingRate.push_back(CurrentFile.SamplingRate());
    else
    {
      ostringstream oss;
      oss << "The sampling rate of file " << fPathArr[files].c_str()<< " does not exist";
      QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
      oss.str().c_str(), 0, this);
      msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
        this->GenerateFeatureWeights->setEnabled(false);
        return false;
      }
    }

    IfMultipleMenusTestingData = false; // make sure it is set correctly for StimulusPresentation files, jm
    // Check the NumMatrixRows
    if (CurrentFile.Parameters()->Exists("NumMatrixRows"))
    {
      if (CurrentFile.Parameter("NumMatrixRows")->NumValues() != 1)
      {
        NumMatrixRows.push_back(0);
        IfMultipleMenusTestingData = true;
      }
      else
      {
        NumMatrixRows.push_back(CurrentFile.Parameter("NumMatrixRows"));
        IfMultipleMenusTestingData = false;
      }
    }
    else
    {
      if (ApplicationFilterChain[files] == Experiment[0])
      {
        ostringstream oss;
        oss << "The number of rows for the P3 Speller Task of file " << fPathArr[files].c_str()<< " does not exist";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
    }

    // Check the NumMatrixColumns
    if (CurrentFile.Parameters()->Exists("NumMatrixColumns"))
    {
      if (CurrentFile.Parameter("NumMatrixColumns")->NumValues() != 1)
      {
        NumMatrixColumns.push_back(0);
        IfMultipleMenusTestingData = true;
      }
      else
      {
        NumMatrixColumns.push_back(CurrentFile.Parameter("NumMatrixColumns"));
        IfMultipleMenusTestingData = false;
      }
    }
    else
    {
      if (ApplicationFilterChain[files] == Experiment[0])
      {
        ostringstream oss;
        oss << "The number of columns for the P3 Speller Task of file " << fPathArr[files].c_str()<< " does not exist";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
    }

    // Check the number of stimulus in the Stimulus Presentation Task
    if (CurrentFile.Parameters()->Exists("Stimuli"))
    {
      ParamRef parameter = CurrentFile.Parameter("Stimuli");
      NumStimuli.push_back(parameter->NumColumns());
    }
    else
    {
      if (ApplicationFilterChain[files] == Experiment[1])
      {
        ostringstream oss;
        oss << "The parameter stimuli for the Stimulus Presentation Task of file " << fPathArr[files].c_str()<< " does not exist";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
    }

    // Validate stimulus frequencies, jm Mar 21, 2011
    if( ApplicationFilterChain[files] == Experiment[1] )
    {
      ostringstream errors;
      if( !ValidateStimulusFrequencies( CurrentFile, fPathArr[ files ], errors ) )
      {
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), errors.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightpink")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
    }

    if (files>0)
    {
      if (channelsInFile[files-1] != channelsInFile[files])
      {
        ostringstream oss;
        oss << "All input files must have the same Numbers of Channels";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TestingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
          this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
      if (SamplingRate[files-1] != SamplingRate[files])
      {
        ostringstream oss;
        oss << "All input files must have the same Sampling Rate";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TestingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
          this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
      if (InterpretMode[files-1] != InterpretMode[files])
      {
        ostringstream oss;
        oss << "All input files must have the same Interpret Mode";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TestingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
          this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
      if (ApplicationFilterChain[files-1] != ApplicationFilterChain[files])
      {
        ostringstream oss;
        oss << "All input files must be recorded using the same experiment";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
        oss.str().c_str(), 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
          this->TestingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
          this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
          this->GenerateFeatureWeights->setEnabled(false);
          return false;
        }
      }
      if (ApplicationFilterChain[files] == Experiment[0])
        if (NumMatrixRows[files-1] != NumMatrixRows[files])
        {
          ostringstream oss;
          oss << "All input files must be recorded using the same experiment";
          QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
          oss.str().c_str(), 0, this);
          msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
          if (msgBox.exec() == QMessageBox::AcceptRole)
          {
            this->TestingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
            this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
            this->GenerateFeatureWeights->setEnabled(false);
            return false;
          }
        }
      if (ApplicationFilterChain[files] == Experiment[0])
        if (NumMatrixColumns[files-1] != NumMatrixColumns[files])
        {
          ostringstream oss;
          oss << "All input files must be recorded using the same experiment";
          QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
          oss.str().c_str(), 0, this);
          msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
          if (msgBox.exec() == QMessageBox::AcceptRole)
          {
            this->TestingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
            this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
            this->GenerateFeatureWeights->setEnabled(false);
            return false;
          }
        }
      if (ApplicationFilterChain[files] == Experiment[1])
        if (NumStimuli[files-1] != NumStimuli[files])
        {
          ostringstream oss;
          oss << "All input files must be recorded using the same experiment";
          QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
          oss.str().c_str(), 0, this);
          msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
          if (msgBox.exec() == QMessageBox::AcceptRole)
          {
            this->TestingDataFilesList->item(files-1)->setBackgroundColor(QColor(tr("yellow")));
            this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("yellow")));
            this->GenerateFeatureWeights->setEnabled(false);
            return false;
          }
        }
    }
    numSamples += samplesInFile[files];
    this->TestingDataFilesList->item(files)->setBackgroundColor(QColor(tr("lightgreen")));
  }
  numChannels = channelsInFile[0];
// Set mode according to the Interpret Mode and Application Filter Chain parameters
// mode = 1   ->    P3SpellerTask/Online Free Mode
// mode = 2   ->    P3SpellerTask/Copy Mode
// mode = 3   ->    StimulusPresentationTask/Online Free Mode
// mode = 4   ->    StimulusPresentationTask/Copy Mode

  if ((Experiment[0] == ApplicationFilterChain[0]) && InterpretMode[0] == 1)
    mode = 1;
  if ((Experiment[0] == ApplicationFilterChain[0]) && InterpretMode[0] == 2)
    mode = 2;
  if ((Experiment[1] == ApplicationFilterChain[0]) && InterpretMode[0] == 1)
    mode = 3;
  if ((Experiment[1] == ApplicationFilterChain[0]) && InterpretMode[0] == 2)
    mode = 4;

  return true;
}

void DataPage::GenerateFeatures()
{
  if (IfMultipleMenusTrainingData)
  {
    ostringstream oss;
    oss << "This program does not support nested matrices. Therefore, it won't display the classification accuracies in the details panel.";
    QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
    oss.str().c_str(), 0, this);
    msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
    if (msgBox.exec() == QMessageBox::AcceptRole)
    {
    }
  }

  g_pDetailsPage->progressBarOverall->setValue(0);
  g_pDataPage->progressBarOverall->setValue(0);
  this->GenerateFeatureWeights->setEnabled(false);
  GenerateFeatureWeightsThread.start();

  if (!IfGenerateFeatureWeightsThread)
  {
    g_pDetailsPage->progressBarOverall->setValue(0);
    g_pDataPage->progressBarOverall->setValue(0);
  }
}

void DataPage::ApplyFeatures()
{
  if (IfMultipleMenusTestingData)
  {
    ostringstream oss;
    oss << "This program does not support nested matrices. Therefore, it won't display the classification accuracies in the details panel.";
    QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
    oss.str().c_str(), 0, this);
    msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
    if (msgBox.exec() == QMessageBox::AcceptRole)
    {
    }
  }
  g_pDataPage->progressBarOverall->setValue(0);
  g_pDetailsPage->progressBarOverall->setValue(0);
  this->ApplyFeatureWeights->setEnabled(false);
  ApplyFeatureWeightsThread.start();
}

void GenerateFeatureWeightsThread::run()
{
  ap::template_2d_array<float, true> signal;
  ap::template_2d_array<float,true> signal_tmp;
  ap::template_2d_array<float,true> signal_all_files;
  ap::template_1d_array<double,true> windowlen;
  ap::template_1d_array<double,true> chset;
  ap::real_2d_array pscore;
  double NumberOfChoices;
  int row_signal, row_chset,
    files, DF, MA, numSequences = 1e6, maxValue = 0,
    Target, nonTarget;
  vector<string> file;
  vector<int> opt;
  vector<float> signal_vector;
  vector<double> vresult;
  vector<short int> trialnr;
  vector<unsigned short int> Type;
  vector<unsigned short int> Code;
  vector<unsigned char> Flashing;
  vector<string> predicted;
  prm parms;
  ste state;
  ste state_tmp;

  ostringstream oss;

  clock_t Clock0;
  Clock0 = clock(); // start timer

  windowlen.setbounds(0, static_cast<int>(g_pDataPage->IniParam.windlen.size())-1);

  //g_pParametersPage->
  chset.setbounds(0, static_cast<int>(g_pDataPage->IniParam.channel_set.size())-1);

  for (int i=0; i<static_cast<int>(g_pDataPage->IniParam.channel_set.size()); i++)
      chset(i) = g_pDataPage->IniParam.channel_set[i];

  row_chset = chset.gethighbound(1)+1;

  if (g_pDataPage->IniParam.SF == 1)
  {
    oss << "***** " << "Generating Feature Weights " << "*****" << endl;
    oss << "        (Spatial Filter = Raw)" << endl;
    emit signalProgressText(oss.str().c_str());
    oss.str("");
  }

  if (g_pDataPage->IniParam.SF == 2)
  {
    oss << "***** " << "Generating Feature Weights " << "*****" << endl;
    oss << "        (Spatial Filter = CAR)" << endl;
    emit signalProgressText(oss.str().c_str());
    oss.str("");
  }

  int TotalFiles = static_cast<int>(g_pDataPage->fPathArr_TrainingData.size());
  for (files=0; files<TotalFiles; files++)
  {
    oss << "Loading file " << "[" << files+1 << "/" << TotalFiles << "]" << "...";
    emit signalProgressText(oss.str().c_str());
    oss.str("");
    // Load signal, parameters, and states from the BCI2000 data file
    load_BCIdat(g_pDataPage->fPathArr_TrainingData[files], g_pDataPage->mode_TrainingData, signal, parms, state_tmp);
    emit signalProgressBar(files+1, TotalFiles, 1);

    // Take the minimum number of sequences that a file has
    if (parms.NumberOfSequences < numSequences)
      numSequences = parms.NumberOfSequences;

    // Check if the channel set given in the initial parameter is a subset of
    // the channels recorded in the BCI2000 data file

    windowlen(0) = ap::round(g_pDataPage->IniParam.windlen[0]*parms.SamplingRate/1000);
    windowlen(1) = ap::round(g_pDataPage->IniParam.windlen[1]*parms.SamplingRate/1000);
    DF = ap::iceil((double)parms.SamplingRate/(g_pDataPage->IniParam.Decimation_Frequency+0.000001));
    MA = DF;

    if (g_pDataPage->IniParam.SF == 2)
    // Apply common average reference to the channels
      CARfilter(signal);

    row_signal = signal.gethighbound(1)+1;

    // Extract from signal only the channels specified in the initial parameter channel set
    signal_tmp.setbounds(0, row_signal-1, 0, row_chset-1);
    for (int i=0; i<row_chset; i++)
      ap::vmove(signal_tmp.getcolumn(i, 0, row_signal-1), signal.getcolumn(chset(i)-1, 0, row_signal-1));

    oss << "Collecting Responses from file " << "[" << files+1 << "/" << TotalFiles << "]" << "...\n";
    oss << "Filtering and Decimating file " << "[" << files+1 << "/" << TotalFiles << "]" << "...\n";
    emit signalProgressText(oss.str().c_str());
    oss.str("");

    // The signal is filtered and decimated before the SWLDA is applied */
    GetP3Responses(signal_tmp, state_tmp.trialnr, windowlen, state_tmp.StimulusCode,
             state_tmp.StimulusType, state_tmp.Flashing, chset, MA, DF);
    emit signalProgressBar(files+1, TotalFiles, 2);

    // Concatenate signal and states
    // Cristhian modification, Sep 11, 2009
    //if (files == 0)
    //  maxValue = state_tmp.trialnr(signal_tmp.gethighbound(1));

    for (int i=0; i<signal_tmp.gethighbound(1)+1; i++)
    {
      for (int j=0; j<signal_tmp.gethighbound(0)+1; j++)
        signal_vector.push_back(signal_tmp(i,j));

      Code.push_back(state_tmp.StimulusCode(i));
      Type.push_back(state_tmp.StimulusType(i));
      Flashing.push_back(state_tmp.Flashing(i));
      //trialnr.push_back(state_tmp.trialnr(i)+(files*maxValue));
      trialnr.push_back(state_tmp.trialnr(i) + maxValue);
    }
    maxValue = *max_element(trialnr.begin(), trialnr.end());
  }
  parms.NumberOfSequences = numSequences;
  signal_all_files.setbounds(0, static_cast<int>(trialnr.size())-1, 0, signal_tmp.gethighbound(0));
  state.trialnr.setbounds(0, static_cast<int>(trialnr.size())-1);
  state.StimulusCode.setbounds(0, static_cast<int>(trialnr.size())-1);
  state.StimulusType.setbounds(0, static_cast<int>(trialnr.size())-1);
  state.Flashing.setbounds(0, static_cast<int>(trialnr.size())-1);
  state.TargetDefinitions = state_tmp.TargetDefinitions;
  Target = 0;
  nonTarget = 0;

  int l = 0;
  for (int i=0; i<static_cast<int>(trialnr.size()); i++)
  {
    for (int j=0; j<signal_tmp.gethighbound(0)+1; j++)
    {
      signal_all_files(i,j) = signal_vector[l];
      l++;
    }
    state.trialnr(i) = trialnr[i];
    state.StimulusCode(i) = Code[i];
    state.StimulusType(i) = Type[i];
    state.Flashing(i) = Flashing[i];

    if (Type[i] == 0)
      nonTarget++;

    if (Type[i] == 1)
      Target++;
  }
  oss << "Targets: " << Target << endl;
  oss << "Non Targets: " << nonTarget << endl << endl;
  oss << "Applying Stepwisefit classifier...\n";
  emit signalProgressText(oss.str().c_str());
  oss.str("");

  // Check that the signal has more rows than columns (overdetermined system)
  if (signal_all_files.gethighbound(1)<signal_all_files.gethighbound(0))
  {
    oss << "Signal must have rows > cols. Linear classifier is valid only for overdetermined systems." << endl;
    emit errorMessage(oss.str().c_str());
    g_pDataPage->IfGenerateFeatureWeightsThread = false;
    return;
  }

  // Perform the Stepwise Linear Discriminant Analysis
  if (! SWLDA(signal_all_files, state.StimulusType, state.trialnr, windowlen, chset, MA, DF, g_pDataPage->IniParam.SF,
      parms.SamplingRate, g_pDataPage->IniParam.penter, g_pDataPage->IniParam.premove, g_pDataPage->IniParam.maxiter,
      parms.SoftwareCh, g_pDataPage->tMUD, &callback_status))
  {
    oss << "Unable to generate usable weights, try different parameters or dataset." << endl;
    emit errorMessage(oss.str().c_str());
    g_pDataPage->IfGenerateFeatureWeightsThread = false;
    return;
  }
  emit signalProgressBar(1, 5, 3);

  // jm Oct 24, 2012
  ap::template_1d_array<unsigned short int, true> stimCode_GetScore = state.StimulusCode;
  NumberOfChoices = FixupStimulusCodes( stimCode_GetScore );
  
  if (!GetScore(signal_all_files, stimCode_GetScore, g_pDataPage->tMUD, state.trialnr, windowlen, row_chset,
      parms.NumberOfSequences, NumberOfChoices, g_pDataPage->mode_TrainingData, pscore))
  {
  // Cristhian modification April 17, 2010
  oss << "Dataset is corrupted. Try another dataset." << endl;
    emit errorMessage(oss.str().c_str());
    g_pDataPage->IfGenerateFeatureWeightsThread = false;
    return;
  }

  emit signalProgressBar(2, 5, 4);

  if (!g_pDataPage->IfMultipleMenusTrainingData)
  {
    if (g_pDataPage->mode_TrainingData == 1 || g_pDataPage->mode_TrainingData == 2)
      // Classify the P3 Speller Task
      P300_Classify(pscore, state.StimulusCode, state.StimulusType, state.trialnr,
              parms.NumberOfSequences, parms.NumMatrixRows, parms.NumMatrixColumns,
              state.TargetDefinitions, vresult, predicted);

    if (g_pDataPage->mode_TrainingData == 3 || g_pDataPage->mode_TrainingData == 4)
      // Classify the Stimulus Presentation Task
      StimulusPresentation_Classify(pscore, state.StimulusCode, state.StimulusType,
                      state.trialnr, parms.NumberOfSequences, NumberOfChoices,
                      state.TargetDefinitions, vresult, predicted);
    emit signalProgressBar(3, 5, 5);
    signal_vector.clear();
    trialnr.clear();
    Code.clear();
    Type.clear();
    Flashing.clear();
    state.TargetDefinitions.clear();
    state_tmp.TargetDefinitions.clear();
    numSequences = 1e6;

    PrintClassificationResults( oss, predicted, vresult );
  }
  oss << "Done!\n\n";
  oss << "Time elapsed [s]: " << ((clock()-Clock0)/(float) CLOCKS_PER_SEC) << endl << endl;

  QTextCodec* codec = QTextCodec::codecForLocale();
  // Cristhian Modification, May 04, 2010
  // Modification to display classification results in unicode
  //QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
  QString unicodeString = codec->toUnicode(oss.str().c_str());
  emit signalProgressText(unicodeString);
  //emit signalProgressText(oss.str().c_str());
  oss.str("");
  vresult.clear();
  predicted.clear();
  emit signalProgressBar(5, 5, 7);

  // Write the suggested name for the *.prm file
  if( g_pDataPage->WritePRMLineEdit->text().length() == 0 )
  {
	  size_t found;
	  string filedata = g_pDataPage->fPathArr_TrainingData[0];
	  found = filedata.find_last_of("/\\");
	  string fileSug = ( found == string::npos ? string() : filedata.substr(0, found + 1) ) + "P3Classifier_ChS1_";
	  if (g_pDataPage->IniParam.SF == 1)
		fileSug += "RAW_";
	  else if (g_pDataPage->IniParam.SF == 2)
		fileSug += "CAR_";
	  fileSug += "SW.prm";

	  g_pDataPage->WritePRMLineEdit->setText(fileSug.c_str());
  }
  g_pDataPage->IfGenerateFeatureWeightsThread = true;
  if( g_pDataPage->mAutoWrite )
  {
    g_pDataPage->WriteParameterFragment( false );
	QApplication::quit();
  }
}

void DataPage::slotProgressBar(int done, int total, int stage)
{
  if (stage == 1 || stage == 2)
  {
    g_pDetailsPage->progressBarOverall->setValue(((float) done / (float) total)*30.0);
    g_pDataPage->progressBarOverall->setValue(((float) done / (float) total)*30.0);
  }
  else
  {
    g_pDetailsPage->progressBarOverall->setValue(((float) done / (float) total)*100.0);
    g_pDataPage->progressBarOverall->setValue(((float) done / (float) total)*100.0);
  }

  if (stage == 7)
  {
    GenerateFeatureWeights  ->setEnabled(true);
    FileWritePRMButton    ->setEnabled(true);
    IfMUD = true;

    if ((static_cast<int>(fPathArr_TestingData.size()) > 0) & (IfTrueTestingDataFiles))
      ApplyFeatureWeights   ->setEnabled(true);
  }
}


void DataPage::slotProgressText(QString message)
{
    g_pDetailsPage->ProgressTextEdit->append(message);
}

// Work on this
void DataPage::slotErrorMessageDialog(QString errorMessage)
{
  errorMessageDialog->showMessage(errorMessage);
  errorMessageDialog->exec();
  g_pDetailsPage->progressBarOverall->setValue(0);
}

void ApplyFeatureWeightsThread::run()
{
  ap::template_2d_array<float, true> signal;
  ap::template_2d_array<float,true> signal_tmp;
  ap::template_2d_array<float,true> signal_all_files;
  ap::template_1d_array<double,true> windowlen;
  ap::template_1d_array<double,true> chset;
  ap::real_2d_array pscore;
  double NumberOfChoices;
  int row_signal, row_chset,
    files, DF, MA, numSequences = 1e6, maxValue = 0,
    Target, nonTarget;
  vector<string> file;
  vector<int> opt;
  vector<float> signal_vector;
  vector<double> vresult;
  vector<short int> trialnr;
  vector<unsigned short int> Type;
  vector<unsigned short int> Code;
  vector<unsigned char> Flashing;
  vector<string> predicted;
  prm parms;
  ste state;
  ste state_tmp;

  ostringstream oss;

  clock_t Clock0;
  Clock0 = clock(); // start timer

  windowlen.setbounds(0, static_cast<int>(g_pDataPage->IniParam.windlen.size())-1);
  chset.setbounds(0, static_cast<int>(g_pDataPage->IniParam.channel_set.size())-1);

  for (int i=0; i<static_cast<int>(g_pDataPage->IniParam.channel_set.size()); i++)
      chset(i) = g_pDataPage->IniParam.channel_set[i];

  row_chset = chset.gethighbound(1)+1;

  if (g_pDataPage->IniParam.SF == 1)
  {
    oss << "***** " << "Applying Feature Weights " << "*****" << endl;
    oss << "       (Spatial Filter = Raw)" << endl;
    emit signalProgressText(oss.str().c_str());
    oss.str("");
  }

  if (g_pDataPage->IniParam.SF == 2)
  {
    oss << "***** " << "Applying Feature Weights " << "*****" << endl;
    oss << "       (Spatial Filter = CAR)" << endl;
    emit signalProgressText(oss.str().c_str());
    oss.str("");
  }

  int TotalFiles = static_cast<int>(g_pDataPage->fPathArr_TestingData.size());
  for (files=0; files<TotalFiles; files++)
  {
    oss << "Loading file " << "[" << files+1 << "/" << TotalFiles << "]" << "...";
    emit signalProgressText(oss.str().c_str());
    oss.str("");
    // Load signal, parameters, and states from the BCI2000 data file
    load_BCIdat(g_pDataPage->fPathArr_TestingData[files], g_pDataPage->mode_TestingData, signal, parms, state_tmp);
    emit signalProgressBar(files+1, TotalFiles, 1);

    // Take the minimum number of sequences that a file has
    if (parms.NumberOfSequences < numSequences)
      numSequences = parms.NumberOfSequences;

    // Check if the channel set given in the initial parameter is a subset of
    // the channels recorded in the BCI2000 data file

    windowlen(0) = ap::round(g_pDataPage->IniParam.windlen[0]*parms.SamplingRate/1000);
    windowlen(1) = ap::round(g_pDataPage->IniParam.windlen[1]*parms.SamplingRate/1000);
    DF = ap::iceil((double)parms.SamplingRate/(g_pDataPage->IniParam.Decimation_Frequency+0.000001));
    MA = DF;

    if (g_pDataPage->IniParam.SF == 2)
    // Apply common average reference to the channels
      CARfilter(signal);

    row_signal = signal.gethighbound(1)+1;

    // Extract from signal only the channels specified in the initial parameter channel set
    signal_tmp.setbounds(0, row_signal-1, 0, row_chset-1);
    for (int i=0; i<row_chset; i++)
      ap::vmove(signal_tmp.getcolumn(i, 0, row_signal-1), signal.getcolumn(chset(i)-1, 0, row_signal-1));

    oss << "Collecting Responses from file " << "[" << files+1 << "/" << TotalFiles << "]" << "...\n";
    oss << "Filtering and Decimating file " << "[" << files+1 << "/" << TotalFiles << "]" << "...\n";
    emit signalProgressText(oss.str().c_str());
    oss.str("");

    // The signal is filtered and decimated before the SWLDA is applied */
    GetP3Responses(signal_tmp, state_tmp.trialnr, windowlen, state_tmp.StimulusCode,
             state_tmp.StimulusType, state_tmp.Flashing, chset, MA, DF);
    emit signalProgressBar(files+1, TotalFiles, 2);

    // Concatenate signal and states
    // Cristhian modification, Sep 11, 2009

    //if (files == 0)
    //  maxValue = state_tmp.trialnr(signal_tmp.gethighbound(1));

    for (int i=0; i<signal_tmp.gethighbound(1)+1; i++)
    {
      for (int j=0; j<signal_tmp.gethighbound(0)+1; j++)
        signal_vector.push_back(signal_tmp(i,j));

      Code.push_back(state_tmp.StimulusCode(i));
      Type.push_back(state_tmp.StimulusType(i));
      Flashing.push_back(state_tmp.Flashing(i));
      trialnr.push_back(state_tmp.trialnr(i) + maxValue);
    }
    maxValue = *max_element(trialnr.begin(), trialnr.end());
  }
  parms.NumberOfSequences = numSequences;
  signal_all_files.setbounds(0, static_cast<int>(trialnr.size())-1, 0, signal_tmp.gethighbound(0));
  state.trialnr.setbounds(0, static_cast<int>(trialnr.size())-1);
  state.StimulusCode.setbounds(0, static_cast<int>(trialnr.size())-1);
  state.StimulusType.setbounds(0, static_cast<int>(trialnr.size())-1);
  state.Flashing.setbounds(0, static_cast<int>(trialnr.size())-1);
  state.TargetDefinitions = state_tmp.TargetDefinitions;

  Target = 0; nonTarget = 0;
  int l = 0;
  for (int i=0; i<static_cast<int>(trialnr.size()); i++)
  {
    for (int j=0; j<signal_tmp.gethighbound(0)+1; j++)
    {
      signal_all_files(i,j) = signal_vector[l];
      l++;
    }
    state.trialnr(i) = trialnr[i];
    state.StimulusCode(i) = Code[i];
    state.StimulusType(i) = Type[i];
    state.Flashing(i) = Flashing[i];

    if (Type[i] == 0)
      nonTarget++;

    if (Type[i] == 1)
      Target++;
  }

  oss << "Targets: " << Target << endl;
  oss << "Non Targets: " << nonTarget << endl;
  emit signalProgressText(oss.str().c_str());
  oss.str("");

  // jm Oct 24, 2012
  ap::template_1d_array<unsigned short int, true> stimCode_GetScore = state.StimulusCode;
  NumberOfChoices = FixupStimulusCodes( stimCode_GetScore );

  GetScore(signal_all_files, stimCode_GetScore, g_pDataPage->tMUD, state.trialnr, windowlen, row_chset,
      parms.NumberOfSequences, NumberOfChoices, g_pDataPage->mode_TrainingData, pscore);

  emit signalProgressBar(2, 5, 4);

  if (!g_pDataPage->IfMultipleMenusTestingData)
  {
    if (g_pDataPage->mode_TrainingData == 1 || g_pDataPage->mode_TrainingData == 2)
      // Classify the P3 Speller Task
      P300_Classify(pscore, state.StimulusCode, state.StimulusType, state.trialnr,
              parms.NumberOfSequences, parms.NumMatrixRows, parms.NumMatrixColumns,
              state.TargetDefinitions, vresult, predicted);

    if (g_pDataPage->mode_TrainingData == 3 || g_pDataPage->mode_TrainingData == 4)
      // Classify the Stimulus Presentation Task
      StimulusPresentation_Classify(pscore, state.StimulusCode, state.StimulusType,
                      state.trialnr, parms.NumberOfSequences, NumberOfChoices,
                      state.TargetDefinitions, vresult, predicted);
    emit signalProgressBar(3, 5, 5);
    signal_vector.clear();
    trialnr.clear();
    Code.clear();
    Type.clear();
    Flashing.clear();
    state.TargetDefinitions.clear();
    state_tmp.TargetDefinitions.clear();
    numSequences = 1e6;
    PrintClassificationResults( oss, predicted, vresult );
  }
  oss << "Done!\n\n";
  oss << "Time elapsed [s]: " << ((clock()-Clock0)/(float) CLOCKS_PER_SEC) << endl << endl;

  QTextCodec* codec = QTextCodec::codecForLocale();
  // Cristhian Modification, May 04, 2010
  // Modification to display classification results in unicode
  //QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
  QString unicodeString = codec->toUnicode(oss.str().c_str());
  emit signalProgressText(unicodeString);
  //emit signalProgressText(oss.str().c_str());
  oss.str("");
  vresult.clear();
  predicted.clear();
  emit signalProgressBar(5, 5, 7);
}

void DataPage::WriteParameterFragment( bool showDialog )
{
  QString fileName = WritePRMLineEdit->text();
  if( showDialog )
    fileName = QFileDialog::getSaveFileName(this,
                                            tr("Write *.prm File"),
                                            fileName,
                                            tr("Parameter Files (*.prm)"));
  if (!fileName.isEmpty())
  {
    WritePRMLineEdit->setText(fileName);
    // Write PRM
    WritePRM(fileName.toLocal8Bit().constData(), tMUD, IniParam.SF);
  }
}

bool DataPage::ValidateStimulusFrequencies( BCI2000FileReader& ioFile, string& inFileName, ostream& ioErrors )
{
  bool errorOccurred = false;
  const char* parameters[] =
  {
    "Stimuli",
    "SequenceType",
    "Sequence",
  };
  for( int i = 0; i < sizeof( parameters ) / sizeof( *parameters ); ++i )
    if( !ioFile.Parameters()->Exists( parameters[i] ) )
    {
      ioErrors << "Parameter \"" << parameters[i] << "\" does not exist in file " << inFileName << ".\n";
      errorOccurred = true;
    }
  if( errorOccurred )
    return false;

  int numStimuli = ioFile.Parameter( "Stimuli" )->NumColumns();
  vector<int> stimulusFrequencies( numStimuli, 0 );
  ParamRef Sequence = ioFile.Parameter( "Sequence" );
  if( ioFile.Parameter( "SequenceType" ) == 0 ) // deterministic sequence
  {
    for( int i = 0; i < Sequence->NumValues(); ++i )
    {
      if( Sequence( i ) >= 1 && Sequence( i ) <= numStimuli )
      {
        ++stimulusFrequencies[static_cast<unsigned int>( Sequence( i ) ) - 1];
      }
      else
      {
        ioErrors << "Invalid stimulus code in the \"Sequence\" parameter of file " << inFileName << ".\n";
        return false;
      }
    }
  }
  else if( ioFile.Parameter( "SequenceType" ) == 1 ) // random sequence
  {
    for( int i = 0; i < min( numStimuli, Sequence->NumValues() ); ++i )
      stimulusFrequencies[i] = Sequence( i );
  }
  else if( ioFile.Parameter( "SequenceType" ) == 2 ) // P3Speller compatible
  {
    for( int i = 0; i < numStimuli; ++i )
      stimulusFrequencies[i] = 1;
  }
  else // unknown sequence type
  {
    ioErrors << "Unknown sequence type in file " << inFileName << ".\n";
    return false;
  }

  // Error if stimulus frequencies are not 1
  for( int i = 0; i < numStimuli; ++i )
    if( stimulusFrequencies[i] != 1 )
    {
      ioErrors << "All stimuli defined in the \"Stimuli\" parameter "
               << "must appear exactly once per sequence "
               << "(check the \"Sequence\" parameter in file " << inFileName << ").\n";
      return false;
    }

  return true;
}

void GenerateFeatureWeightsThread::signalProgressTextCallBack(QString message)
{
  emit signalProgressText(message);

}
