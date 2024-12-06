#include "installerdialog.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataBase db;
    bool successOpenDB = db.open();
    if (successOpenDB){
        QString userName = QProcessEnvironment::systemEnvironment().value("USERNAME");
        if (!db.doesUserNameExists(userName)){
            InstallerDialog installerDialog;
            installerDialog.exec();}
    }
    db.close();

    MainWindow w;
    w.show();
    return a.exec();
}
