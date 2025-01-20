#include <QApplication>
#include <QFile>
#include <QSqlQuery>
#include "installerdialog.h"
#include "mainwindow.h"

/**
 * Prüft, ob die notwendigen Tabellen in der Datenbank existieren.
 * @param db Referenz auf das Datenbank-Objekt.
 * @return true, wenn alle Tabellen existieren, sonst false.
 */
bool tablesExist(DataBase &db)
{
    return db.tableExists("Mediathek") && db.tableExists("Pathlist") && db.tableExists("Playlists")
    && db.tableExists("PlaylistTracks");
}

/**
 * Hauptfunktion der Anwendung.
 * Initialisiert die Anwendung, prüft die Datenbank und startet das Hauptfenster.
 * @param argc Anzahl der Argumente.
 * @param argv Array mit den Argumenten.
 * @return Rückgabewert der Anwendung.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataBase db;

    // Überprüft, ob die Datenbankdatei existiert.
    QString dbPath = "MusicPlayerSQLDB.db";
    bool showInstaller = !QFile::exists(dbPath) || !db.open() || !tablesExist(db);

    if (showInstaller) {
        // Zeigt den Installationsdialog an, wenn die Datenbank fehlt oder Tabellen nicht existieren.
        InstallerDialog installerDialog;
        if (installerDialog.exec() != QDialog::Accepted) {
            return 0; // Beendet die Anwendung, falls der Installationsdialog abgelehnt wird.
        }
    } else {
        // Überprüft und bereinigt verwaiste Dateien in der Datenbank.
        db.checkFiles();
    }

    // Startet die Hauptanwendung.
    MainWindow w;
    w.show();
    return a.exec();
}
