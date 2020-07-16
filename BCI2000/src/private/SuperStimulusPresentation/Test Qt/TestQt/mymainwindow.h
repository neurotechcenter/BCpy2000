#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MyMainWindow;
}

class MyMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MyMainWindow(QWidget *parent = 0);
  ~MyMainWindow();
};

#endif // MYMAINWINDOW_H
