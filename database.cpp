#include "DataBase.h"
#include <QDebug>
#include <QDir>

DataBase::DataBase() {
    qDebug() << "Datenbank Konstruktor gestartet";

    // Überprüfen, ob bereits eine Verbindung mit dem Namen "MusicPlayerConnection" existiert
    if (QSqlDatabase::contains("MusicPlayerConnection")) {
        db = QSqlDatabase::database("MusicPlayerConnection");  // Bestehende Verbindung verwenden
        qDebug() << "Datenbankverbindung bereits vorhanden.";
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "MusicPlayerConnection");  // Neue Verbindung erstellen
        db.setDatabaseName("MusicPlayerSQLDB.db");

        if (!db.open()) {
            qDebug() << "Fehler beim Öffnen der Datenbank:" << db.lastError();
            return;
        }

        qDebug() << "Datenbank erfolgreich geöffnet!";
    }
}

DataBase::~DataBase() {
    // Datenbankverbindung schließen
    db.close();
}

bool DataBase::createTableMediathek() {
    if (!db.open()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qDebug() << "SQLite-Treiber ist nicht verfügbar.";
    }
    QSqlQuery query;

    bool success = query.prepare("CREATE TABLE IF NOT EXISTS Mediathek ("
               "TrackID SERIAL PRIMARY KEY, "
               "Interpret TEXT, "
               "Titel TEXT, "
               "Album TEXT, "
               "Spielzeit TEXT");
    query.exec();
    if (!success) {
        qDebug() << "Fehler beim Erstellen der Tabelle:" << query.lastError();
    } else {
        qDebug() << "Tabelle 'Mediathek' wurde erfolgreich erstellt oder existiert bereits.";
    }
    return success;
}
bool DataBase::createTableOptionen() {
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS OPTIONEN ("
                              "Index SERIAL PRIMARY KEY,"
                              "Lautstaerke FLOAT,"
                              "MusikPfad TEXT,"
                              "NutzerName TEXT,");
    if (!success) {
        qDebug() << "Fehler beim Erstellen der Tabelle:" << query.lastError();
    } else {
        qDebug() << "Tabelle 'Mediathek' wurde erfolgreich erstellt oder existiert bereits.";
    }
    return success;
}

bool DataBase::insertData(const QString &interpret, const QString &album, const QString &titel, int spielzeit) {
    QSqlQuery query;

    // SQL-Befehl zum Einfügen von Daten
    query.prepare("INSERT INTO Mediathek (Interpret, Album, Titel, Spielzeit) "
                  "VALUES (:interpret, :album, :titel, :spielzeit)");

    // Bindung der Werte
    query.bindValue(":interpret", interpret);
    query.bindValue(":album", album);
    query.bindValue(":titel", titel);
    query.bindValue(":spielzeit", QString::number(spielzeit)); // Spielzeit als TEXT speichern (Umwandlung in QString)
    // Ausführen der Abfrage und Überprüfen auf Fehler
    if (!query.exec()) {
        qDebug() << "Fehler beim Einfügen von Daten:" << query.lastError();
        return false;
    }

    qDebug() << "Daten erfolgreich eingefügt!";
    return true;
}
bool DataBase::insertDataIntoOptions(const float &LautStaerke, QDir &MusikPfad, QString &userName)
{
    QSqlQuery query;

    // SQL-Befehl zum Einfügen von Daten
    query.prepare("INSERT INTO OPTIONEN (Lautstaerke, MusikPfad,NutzerName) "
                  "VALUES (:LautStaerke, :MusikPfad, :NutzerName)");

    // Bindung der Werte
    query.bindValue(":LautStaerke", LautStaerke);
    query.bindValue(":MusikPfad", MusikPfad.absolutePath());
    query.bindValue(":NutzerName",userName);
    // Ausführen der Abfrage und Überprüfen auf Fehler
    if (!query.exec()) {
        qDebug() << "Fehler beim Einfügen von Daten in OPTIONEN:" << query.lastError();
        return false;
    }

    qDebug() << "Daten erfolgreich in OPTIONEN eingefügt!";
    return true;
}

void DataBase::queryData() {
    QSqlQuery query("SELECT * FROM Mediathek");

    while (query.next()) {
        QString interpret = query.value(0).toString();
        QString album = query.value(1).toString();
        QString titel = query.value(2).toString();
        int spielzeit = query.value(3).toInt();
        qDebug() << "Interpret:" << interpret << ", Album:" << album << ", Titel:" << titel << ", Spielzeit:" << spielzeit;
    }
}
bool DataBase::doesUserNameExists(const QString &userName)
{
    // Bereite die SQL-Abfrage vor
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM OPTIONEN WHERE NutzerName = :userName");

    // Binde den Benutzernamen an die Abfrage
    query.bindValue(":userName", userName);

    // Führe die Abfrage aus
    if (!query.exec()) {
        qWarning() << "Fehler beim Ausführen der Abfrage:" << query.lastError().text();
        return false;
    }

    // Hole das Ergebnis der Abfrage (Anzahl der Datensätze)
    if (query.next()) {
        int count = query.value(0).toInt();
        return count > 0;  // Gibt true zurück, wenn der Benutzername existiert, sonst false
    }

    return false;  // Falls kein Ergebnis vorhanden ist
}
bool DataBase::open() {
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        return false;
    } else {
        qDebug() << "Database: connection ok";
        return true;
    }
}

void DataBase::close() {
    db.close();
}

// bool DataBase::savePlaylist(const std::vector<QString> &playlist) {
//     QSqlQuery query;
//     query.prepare("INSERT INTO playlists (name) VALUES (?)");
//     for (const QString &file : playlist) {
//         query.addBindValue(file);
//         if (!query.exec()) {
//             qDebug() << "Error: failed to insert data -" << query.lastError();
//             return false;
//         }
//     }
//     return true;
// }

// bool DataBase::loadPlaylist(std::vector<QString> &playlist) {
//     QSqlQuery query("SELECT name FROM playlists");
//     while (query.next()) {
//         QString name = query.value(0).toString();
//         playlist.push_back(name);
//     }
//     return true;
// }

// bool DataBase::initialize() {
//     QSqlQuery query;
//     query.prepare("CREATE TABLE IF NOT EXISTS playlists (id INTEGER PRIMARY KEY, name TEXT)");
//     if (!query.exec()) {
//         qDebug() << "Error: failed to create table -" << query.lastError();
//         return false;
//     }
//     return true;
// }
