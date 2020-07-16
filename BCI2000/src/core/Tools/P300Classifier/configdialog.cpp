#include <QtGui>
#include "configdialog.h"
#include "pages.h"

ConfigDialog::ConfigDialog()
{
	// Define pointer for all the pages
	pDataPage = new DataPage;
	pParametersPage = new ParametersPage;
	pDetailsPage = new DetailsPage;

	// Pass pointers between pages to share information
	pParametersPage->setDataPage(pDataPage);
	pDataPage->setParametersPage(pParametersPage);

    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(128);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget(pDataPage);
    pagesWidget->addWidget(pParametersPage);
    pagesWidget->addWidget(pDetailsPage);

	pagesWidget->setFixedWidth(400);

    QPushButton *closeButton = new QPushButton(tr("Close"));

    createIcons();
    contentsWidget->setCurrentRow(0);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("P300 Classifier GUI"));
}

void ConfigDialog::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    configButton->setIcon(QIcon(":/images/config.png"));
    configButton->setText(tr("Data"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *updateButton = new QListWidgetItem(contentsWidget);
    updateButton->setIcon(QIcon(":/images/update.png"));
    updateButton->setText(tr("Parameters"));
    updateButton->setTextAlignment(Qt::AlignHCenter);
    updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *queryButton = new QListWidgetItem(contentsWidget);
    queryButton->setIcon(QIcon(":/images/query.png"));
    queryButton->setText(tr("Details"));
    queryButton->setTextAlignment(Qt::AlignHCenter);
    queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void ConfigDialog::SetFiles(QStringList TrainingDataFiles, QStringList TestingDataFiles, QString inicfgFile, const QString& inClassifierOutputFile)
{
	pDataPage->IfMUD = false;
	pDataPage->IfTrueTrainingDataFiles = false;
	pDataPage->IfTrueTestingDataFiles = false;

	pDataPage->inicfgFileLineEdit	->setText(inicfgFile);
	pDataPage->TrainingDataFilesList->addItems(TrainingDataFiles);
	pDataPage->TestingDataFilesList	->addItems(TestingDataFiles);
	pDataPage->WritePRMLineEdit->setText( inClassifierOutputFile );

	pDataPage->LoadSettings();
	pDataPage->ValidateSettings();

	if (TrainingDataFiles.size() != 0)
	{
		pDataPage->IfTrueTrainingDataFiles = pDataPage->ValidateTrainingDataFiles(pDataPage->fPathArr_TrainingData,
										  pDataPage->numSamples_TrainingData,
										  pDataPage->numChannels_TrainingData,
										  pDataPage->mode_TrainingData);
		if (!pDataPage->IfTrueTrainingDataFiles)
			pDataPage->GenerateFeatureWeights->setEnabled(false);
		else
			pDataPage->GenerateFeatureWeights->setEnabled(true);
	}
	else
		pDataPage->GenerateFeatureWeights->setEnabled(false);

	if (TestingDataFiles.size() != 0)
	{
		pDataPage->IfTrueTestingDataFiles = pDataPage->ValidateTestingDataFiles(pDataPage->fPathArr_TestingData,
										  pDataPage->numSamples_TestingData,
										  pDataPage->numChannels_TestingData,
										  pDataPage->mode_TestingData);
		if (!pDataPage->IfTrueTestingDataFiles)
			pDataPage->ApplyFeatureWeights->setEnabled(false);
	}
	else
		pDataPage->ApplyFeatureWeights->setEnabled(false);

	// Modification Cristhian Potes
	if (pDataPage->IfTrueTrainingDataFiles && TrainingDataFiles.count() > 0 )
	{
		pDataPage->mAutoWrite = ( inClassifierOutputFile.length() > 0 );
		pDataPage->GenerateFeatureWeightsThread.start();
	}
}
