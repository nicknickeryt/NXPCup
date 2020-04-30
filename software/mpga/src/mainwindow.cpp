#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    m_connect = new ConnectEvent(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonClose_clicked()
{   
    qDebug() << "Wcisnąłeś przycisk OK";
}
