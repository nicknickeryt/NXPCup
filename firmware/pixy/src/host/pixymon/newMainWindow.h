#ifndef NEWMAINWINDOW_H
#define NEWMAINWINDOW_H


#include <QMainWindow>
#include <qcoreapplication.h>

class newMainWindow : public QMainWindow {
//    Q_OBJECT

public:
    explicit newMainWindow(int argc, char *argv[], QWidget *parent = 0) {
        QCoreApplication::setOrganizationName("AGH");
        QCoreApplication::setApplicationName("MAKE PIXY GREAT AGAIN");


        setFixedSize(1000, 600);
    }

    ~newMainWindow() {

    }
};



#endif // NEWMAINWINDOW_H
