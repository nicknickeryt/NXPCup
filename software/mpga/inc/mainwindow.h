#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "connectevent.h"

enum Device {NONE, PIXY, PIXY_DFU};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//    ConnectEvent *m_connect;

private slots:
    void on_pushButtonClose_clicked();

private:

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
