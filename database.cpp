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

    // Create necessary tables
    createTableMediathek();
    createTablePathlist();
    createTablePlaylists();
    createTablePlaylistTracks();
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

Playlist DataBase::getPlaylist(const QString &playlistName) {
    Playlist playlist;
    playlist.setName(playlistName);

    if (!db.isOpen()) {
        if (!open()) {
            qDebug() << "Failed to open the database.";
            return playlist;
        }
    }

    int playlistID = getPlaylistID(playlistName);
    if (playlistID == -1) {
        qDebug() << "Playlist not found in the database.";
        return playlist;
    }

    QSqlQuery query(db);
    query.prepare("SELECT Mediathek.FilePath FROM PlaylistTracks "
                  "JOIN Mediathek ON PlaylistTracks.TrackID = Mediathek.TrackID " // Correct column name
                  "WHERE PlaylistTracks.PlaylistID = :playlistID");
    query.bindValue(":playlistID", playlistID);
    if (query.exec()) {
        while (query.next()) {
            QString filePath = query.value(0).toString();
            playlist.addFile(filePath);
        }
    } else {
        qDebug() << "Failed to execute query:" << query.lastError().text();
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

bool DataBase::createAllSongsPlaylist() {
    if (!db.isOpen()) {
        if (!open()) {
            qDebug() << "Failed to open the database.";
            return false;
        }
    }

    std::vector<int> songIDs;
    QSqlQuery query(db);

    // Retrieve all song IDs from Mediathek
    query.prepare("SELECT TrackID FROM Mediathek");
    if (query.exec()) {
        while (query.next()) {
            songIDs.push_back(query.value(0).toInt());
        }
    } else {
        qDebug() << "Failed to retrieve song IDs:" << query.lastError();
        return false;
    }

    // Insert all song IDs into PlaylistTracks with PlaylistID = 1
    query.prepare("INSERT INTO PlaylistTracks (PlaylistID, TrackID) VALUES (1, :trackID)");
    for (int trackID : songIDs) {
        query.bindValue(":trackID", trackID);
        if (!query.exec()) {
            qDebug() << "Failed to insert track ID into PlaylistTracks:" << query.lastError();
            return false;
        }
    }

    qDebug() << "All songs successfully added to AllSongsPlaylist!";
    return true;
}

int DataBase::getPlaylistID(const QString &playlistName) {
    QSqlQuery query(db);
    query.prepare("SELECT PlaylistID FROM Playlists WHERE Name = :name"); // Correct column name
    query.bindValue(":name", playlistName);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1; // Return -1 if not found
}

int DataBase::getTrackID(const QString &filePath) {
    QSqlQuery query(db);
    query.prepare("SELECT ID FROM Mediathek WHERE FilePath = :filePath");
    query.bindValue(":filePath", filePath);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1; // Return -1 if not found
}

