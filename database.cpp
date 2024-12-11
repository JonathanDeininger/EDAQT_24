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

bool DataBase::open() {
    if (!db.isOpen()) {
        if (!db.open()) {
            qDebug() << "Error: connection with database failed:" << db.lastError();
            return false;
        }
    }
    qDebug() << "Database: connection ok";
    return true;
}

void DataBase::close() {
    db.close();
}

bool DataBase::createTableMediathek() {
    if (!open()) {
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

bool DataBase::createTablePathlist() {
    if (!open()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection
    bool success = query.exec("CREATE TABLE IF NOT EXISTS Pathlist ("
                              "ID INTEGER PRIMARY KEY AUTOINCREMENT," // Change Index to ID and ensure auto-increment
                              "MusikPfad TEXT)");
    if (!success) {
        qDebug() << "Fehler beim Erstellen der Tabelle 'Pathlist':" << query.lastError();
    } else {
        qDebug() << "Tabelle 'Pathlist' wurde erfolgreich erstellt oder existiert bereits.";
    }
    return success;
}

bool DataBase::createTablePlaylists() {
    if (!open()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection
    bool success = query.exec("CREATE TABLE IF NOT EXISTS Playlists ("
                              "PlaylistID INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "Name TEXT)");
    if (!success) {
        qDebug() << "Fehler beim Erstellen der Tabelle 'Playlists':" << query.lastError();
    } else {
        qDebug() << "Tabelle 'Playlists' wurde erfolgreich erstellt oder existiert bereits.";
    }
    return success;
}

bool DataBase::createTablePlaylistTracks() {
    if (!open()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection
    bool success = query.exec("CREATE TABLE IF NOT EXISTS PlaylistTracks ("
                              "PlaylistID INTEGER, "
                              "TrackID INTEGER, "
                              "FOREIGN KEY (PlaylistID) REFERENCES Playlists(PlaylistID), "
                              "FOREIGN KEY (TrackID) REFERENCES Mediathek(TrackID))");
    if (!success) {
        qDebug() << "Fehler beim Erstellen der Tabelle 'PlaylistTracks':" << query.lastError();
    } else {
        qDebug() << "Tabelle 'PlaylistTracks' wurde erfolgreich erstellt oder existiert bereits.";
    }
    return success;
}

bool DataBase::insertData(const QString &filePath, const QString &interpret, const QString &album, const QString &titel, int spielzeit, int sampleRate, int sampleCount, const QByteArray &hash) {
    if (!open()) {
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

bool DataBase::insertPath(const QString &path) {
    if (!open()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }

    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qDebug() << "SQLite-Treiber ist nicht verfügbar.";
        return false;
    }

    QSqlQuery query(db); // Use the correct database connection

    query.prepare("INSERT INTO Pathlist (MusikPfad) VALUES (:MusikPfad)");
    query.bindValue(":MusikPfad", path);

    qDebug() << "SQL Query:" << query.executedQuery();
    qDebug() << "Bound Values:" << query.boundValues();

    if (!query.exec()) {
        qDebug() << "Fehler beim Einfügen von Daten in Pathlist:" << query.lastError();
        return false;
    }

    qDebug() << "Pfad erfolgreich in Pathlist eingefügt!";
    return true;
}

bool DataBase::insertPlaylist(const QString &playlistName) {
    if (!open()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection

    query.prepare("INSERT INTO Playlists (Name) VALUES (:name)");
    query.bindValue(":name", playlistName);

    if (!query.exec()) {
        qDebug() << "Fehler beim Einfügen von Daten in Playlists:" << query.lastError();
        return false;
    }

    qDebug() << "Playlist erfolgreich eingefügt!";
    return true;
}

bool DataBase::insertPlaylistTrack(int playlistID, int trackID) {
    if (!open()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection

    query.prepare("INSERT INTO PlaylistTracks (PlaylistID, TrackID) VALUES (:playlistID, :trackID)");
    query.bindValue(":playlistID", playlistID);
    query.bindValue(":trackID", trackID);

    if (!query.exec()) {
        qDebug() << "Fehler beim Einfügen von Daten in PlaylistTracks:" << query.lastError();
        return false;
    }

    qDebug() << "Track erfolgreich zur Playlist hinzugefügt!";
    return true;
}

bool DataBase::tableExists(const QString &tableName) {
    if (!open()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }
    QSqlQuery query(db);
    query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name=:tableName");
    query.bindValue(":tableName", tableName);
    if (!query.exec()) {
        qDebug() << "Fehler beim Überprüfen der Tabelle:" << query.lastError();
        return false;
    }
    return query.next();
}

QSqlDatabase& DataBase::getDatabase() {
    return db;
}

Playlist DataBase::getPlaylist(const QString &name) {
    Playlist playlist;
    playlist.setName(name);

    if (!open()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return playlist;
    }

    QSqlQuery query(db);
    query.prepare("SELECT TrackID FROM PlaylistTracks "
                  "JOIN Playlists ON PlaylistTracks.PlaylistID = Playlists.PlaylistID "
                  "WHERE Playlists.Name = :name");
    query.bindValue(":name", name);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve playlist tracks from database:" << query.lastError();
        return playlist;
    }

    while (query.next()) {
        int trackID = query.value(0).toInt();
        QSqlQuery trackQuery(db);
        trackQuery.prepare("SELECT FilePath FROM Mediathek WHERE TrackID = :trackID");
        trackQuery.bindValue(":trackID", trackID);

        if (trackQuery.exec() && trackQuery.next()) {
            QString filePath = trackQuery.value(0).toString();
            playlist.addFile(filePath);
        }
    }

    return playlist;
}

std::vector<QString> DataBase::getAllPlaylists() {
    std::vector<QString> playlists;

    if (!open()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return playlists;
    }

    QSqlQuery query(db);
    query.prepare("SELECT Name FROM Playlists");
    if (!query.exec()) {
        qDebug() << "Failed to retrieve playlists from database:" << query.lastError();
        return playlists;
    }

    while (query.next()) {
        playlists.push_back(query.value(0).toString());
    }

    return playlists;
}

bool DataBase::removePlaylist(const QString &playlistName) {
    if (!open()) {
        qDebug() << "Datenbankverbindung konnte nicht geöffnet werden.";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM Playlists WHERE Name = :name");
    query.bindValue(":name", playlistName);

    if (!query.exec()) {
        qDebug() << "Fehler beim Löschen der Playlist:" << query.lastError();
        return false;
    }

    query.prepare("DELETE FROM PlaylistTracks WHERE PlaylistID = (SELECT PlaylistID FROM Playlists WHERE Name = :name)");
    query.bindValue(":name", playlistName);

    if (!query.exec()) {
        qDebug() << "Fehler beim Löschen der Playlist-Tracks:" << query.lastError();
        return false;
    }

    qDebug() << "Playlist erfolgreich gelöscht!";
    return true;
}

