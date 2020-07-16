#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
class BCI2000FileReader;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void dragEnterEvent( QDragEnterEvent* );
    void dropEvent( QDropEvent* );
    void ChannelNames_handleFiles( const QStringList& );
    void ImportStates_handleFiles( const QStringList& );
    void DropArea_handleFiles( const QStringList& );

private slots:
    void on_FileOpen();
    void on_FileQuit();
    void on_HelpBCI2000Help();
    void on_HelpAbout();

private:
    void EnableAll();
    void DisableAll();
    void SaveSettings() const;
    void ReadSettings();

private:
    struct KeyPressListWidget: public QListWidget
    {
      void keyPressEvent( QKeyEvent* );
    };
    KeyPressListWidget* mpImportStates;
    QTextEdit*   mpChannelNames;
    QLabel*      mpDropArea;
    QComboBox*   mpOutputFormat;
    QLabel*      mpStatusLabel;
};

#endif // MAINWINDOW_H
