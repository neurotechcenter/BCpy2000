#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QString>
#include "pages.h"

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog();

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
	void SetFiles(QStringList TrainingDataFiles, QStringList TestingDataFiles, QString inicfgFile, const QString& classifierOutputFile);

private:
    void createIcons();

    QListWidget		*contentsWidget;
    QStackedWidget	*pagesWidget;

	DataPage		*pDataPage;
    ParametersPage  *pParametersPage;
    DetailsPage	    *pDetailsPage;

    QStringList     TrainingDataFiles;
    QStringList     TestingDataFiles;
    QString			inicfgFile;
};

#endif
