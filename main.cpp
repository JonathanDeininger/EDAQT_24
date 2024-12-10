#include <QDebug>
#include "installerdialog.h"
#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QSqlQuery>

bool tablesExist(DataBase &db) {
    QSqlQuery query(db.getDatabase());
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='Mediathek'");
    if (!query.next()) {
        return false;
    }
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='OPTIONEN'");
    return query.next();
}

int main(int argc, char *argv[]) {
    qDebug() << "Hello World";
    QApplication a(argc, argv);
    DataBase db;

    bool successOpenDB = db.open();
    if (successOpenDB) {
        if (!tablesExist(db)) {
            qDebug() << "Database tables do not exist. Running installer.";
            InstallerDialog installerDialog;
            installerDialog.exec();
        } else {
            qDebug() << "Database tables exist.";
        }
    } else {
        qDebug() << "Failed to open the database.";
    }

    MainWindow w;
    w.show();
    return a.exec();
}
