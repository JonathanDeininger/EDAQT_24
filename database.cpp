#include "DataBase.h"
#include <QDebug>
#include <QDir>
#include <qdebug.h>

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
    if (!db.isOpen()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }

    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qDebug() << "SQLite-Treiber ist nicht verfügbar.";
    }
    QSqlQuery query(db); // Use the correct database connection

    bool success = query.prepare("CREATE TABLE IF NOT EXISTS Mediathek ("
               "TrackID INTEGER PRIMARY KEY AUTOINCREMENT, " // Ensure TrackID is auto-incremented
               "FilePath TEXT, "
               "Interpret TEXT, "
               "Titel TEXT, "
               "Album TEXT, "
               "Spielzeit INT, "
               "SampleRate INT, "
               "SampleCount INT, "
               "Hash TEXT)");
    query.exec();
    if (!success) {
        qDebug() << "Fehler beim Erstellen der Tabelle:" << query.lastError();
    } else {
        qDebug() << "Tabelle 'Mediathek' wurde erfolgreich erstellt oder existiert bereits.";
    }
    return success;
}

bool DataBase::createTableOptionen() {
    if (!db.isOpen()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection
    bool success = query.exec("CREATE TABLE IF NOT EXISTS OPTIONEN ("
                              "ID SERIAL PRIMARY KEY," // Change Index to ID
                              "Lautstaerke FLOAT,"
                              "MusikPfad TEXT)");
    if (!success) {
        qDebug() << "Fehler beim Erstellen der Tabelle:" << query.lastError();
    } else {
        qDebug() << "Tabelle 'OPTIONEN' wurde erfolgreich erstellt oder existiert bereits.";
    }
    return success;
}

bool DataBase::insertData(const QString &filePath, const QString &interpret, const QString &album, const QString &titel, int spielzeit, int sampleRate, int sampleCount, const QByteArray &hash) {
    if (!db.isOpen()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection

    // SQL-Befehl zum Einfügen von Daten
    query.prepare("INSERT INTO Mediathek (FilePath, Interpret, Album, Titel, Spielzeit, SampleRate, SampleCount, Hash) "
                  "VALUES (:filePath, :interpret, :album, :titel, :spielzeit, :sampleRate, :sampleCount, :hash)");

    // Bindung der Werte
    query.bindValue(":filePath", filePath);
    query.bindValue(":interpret", interpret);
    query.bindValue(":album", album);
    query.bindValue(":titel", titel);
    query.bindValue(":spielzeit", spielzeit);
    query.bindValue(":sampleRate", sampleRate);
    query.bindValue(":sampleCount", sampleCount);
    query.bindValue(":hash", hash);
    // Ausführen der Abfrage und Überprüfen auf Fehler
    if (!query.exec()) {
        qDebug() << "Fehler beim Einfügen von Daten:" << query.lastError();
        return false;
    }

    qDebug() << "Daten erfolgreich eingefügt!";
    return true;
}

bool DataBase::insertOptions(const QList<QPair<float, QString>> &options) {
    if (!db.isOpen()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection

    query.prepare("INSERT INTO OPTIONEN (Lautstaerke, MusikPfad) VALUES (:Lautstaerke, :MusikPfad)");

    for (const auto &option : options) {
        query.bindValue(":Lautstaerke", option.first);
        query.bindValue(":MusikPfad", option.second);

        if (!query.exec()) {
            qDebug() << "Fehler beim Einfügen von Daten in OPTIONEN:" << query.lastError();
            return false;
        }
    }

    qDebug() << "Daten erfolgreich in OPTIONEN eingefügt!";
    return true;
}

void DataBase::queryData() {
    if (!db.isOpen()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return;
    }
    QSqlQuery query("SELECT * FROM Mediathek");

    while (query.next()) {
        QString interpret = query.value(0).toString();
        QString album = query.value(1).toString();
        QString titel = query.value(2).toString();
        int spielzeit = query.value(3).toInt();
        qDebug() << "Interpret:" << interpret << ", Album:" << album << ", Titel:" << titel << ", Spielzeit:" << spielzeit;
    }
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

QSqlDatabase& DataBase::getDatabase() {
    return db;
}

