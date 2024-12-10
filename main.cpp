#include <QDebug>
#include "installerdialog.h"
#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QSqlQuery>

bool tablesExist(DataBase &db) {
    return db.tableExists("Mediathek") && db.tableExists("Pathlist") && db.tableExists("Playlists") && db.tableExists("PlaylistTracks");
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    DataBase db;

    // Check if the database file exists
    QString dbPath = "MusicPlayerSQLDB.db";
    bool showInstaller = !QFile::exists(dbPath) || !db.open() || !tablesExist(db);

    if (showInstaller) {
        // Show the installer dialog if the database does not exist or tables are missing
        InstallerDialog installerDialog;
        if (installerDialog.exec() != QDialog::Accepted) {
            return 0; // Exit the application if the installer dialog is not accepted
        }
    }

    // Proceed to the main application
    MainWindow w;
    w.show();
    return a.exec();
}
