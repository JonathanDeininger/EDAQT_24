#include "DataBase.h"
#include <QDir>
#include "playlist.h"

DataBase::DataBase() {
    // Überprüfen, ob bereits eine Verbindung mit dem Namen "MusicPlayerConnection" existiert
    if (QSqlDatabase::contains("MusicPlayerConnection")) {
        db = QSqlDatabase::database("MusicPlayerConnection");  // Bestehende Verbindung verwenden
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "MusicPlayerConnection");  // Neue Verbindung erstellen
        db.setDatabaseName("MusicPlayerSQLDB.db");

        if (!db.open()) {
            return;
        }
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
        return false;
    }

    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection

    bool success = query.prepare("CREATE TABLE IF NOT EXISTS Mediathek ("
                                 "TrackID INTEGER PRIMARY KEY AUTOINCREMENT, " // Ensure TrackID is auto-incremented
                                 "FilePath TEXT, "
                                 "Interpret TEXT, "
                                 "Titel TEXT, "
                                 "Album TEXT, "
                                 "Spielzeit INT, "
                                 "SampleRate INT)");
    query.exec();
    return success;
}

bool DataBase::createTablePathlist() {
    if (!open()) {
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection
    bool success = query.exec("CREATE TABLE IF NOT EXISTS Pathlist ("
                              "ID INTEGER PRIMARY KEY AUTOINCREMENT," // Change Index to ID and ensure auto-increment
                              "MusikPfad TEXT)");
    return success;
}

bool DataBase::createTablePlaylists() {
    if (!open()) {
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection
    bool success = query.exec("CREATE TABLE IF NOT EXISTS Playlists ("
                              "PlaylistID INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "Name TEXT)");
    return success;
}

bool DataBase::createTablePlaylistTracks() {
    if (!open()) {
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection
    bool success = query.exec("CREATE TABLE IF NOT EXISTS PlaylistTracks ("
                              "PlaylistID INTEGER, "
                              "TrackID INTEGER, "
                              "FOREIGN KEY (PlaylistID) REFERENCES Playlists(PlaylistID), "
                              "FOREIGN KEY (TrackID) REFERENCES Mediathek(TrackID))");

    //füge alle Songs aus der Mediathek in die Playlist "Alle Songs" ein
    query.prepare("SELECT TrackID FROM Mediathek");
    if (query.exec()) {
        while (query.next()) {
            query.prepare("INSERT INTO PlaylistTracks (PlaylistID, TrackID) VALUES (1, :trackID)");
            query.bindValue(":trackID", query.value(0).toInt());
            if (!query.exec()) {
                return false;
            }
        }
    } else {
        return false;
    }
    return success;
}

bool DataBase::insertData(const QString &filePath, const QString &interpret, const QString &album, const QString &titel, int spielzeit, int sampleRate) {
    if (!open()) {
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection

    // SQL-Befehl zum Einfügen von Daten
    query.prepare("INSERT INTO Mediathek (FilePath, Interpret, Album, Titel, Spielzeit, SampleRate) "
                  "VALUES (:filePath, :interpret, :album, :titel, :spielzeit, :sampleRate)");

    // Bindung der Werte
    query.bindValue(":filePath", filePath);
    query.bindValue(":interpret", interpret);
    query.bindValue(":album", album);
    query.bindValue(":titel", titel);
    query.bindValue(":spielzeit", spielzeit);
    query.bindValue(":sampleRate", sampleRate);
    // Ausführen der Abfrage und Überprüfen auf Fehler
    if (!query.exec()) {
        return false;
    }

    return true;
}

bool DataBase::insertPath(const QString &path) {
    if (!open()) {
        return false;
    }

    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        return false;
    }

    QSqlQuery query(db); // Use the correct database connection

    query.prepare("INSERT INTO Pathlist (MusikPfad) VALUES (:MusikPfad)");
    query.bindValue(":MusikPfad", path);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool DataBase::insertPlaylist(const QString &playlistName) {
    if (!open()) {
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection

    query.prepare("INSERT INTO Playlists (Name) VALUES (:name)");
    query.bindValue(":name", playlistName);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool DataBase::insertPlaylistTrack(int playlistID, int trackID) {
    if (!open()) {
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection

    query.prepare("INSERT INTO PlaylistTracks (PlaylistID, TrackID) VALUES (:playlistID, :trackID)");
    query.bindValue(":playlistID", playlistID);
    query.bindValue(":trackID", trackID);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool DataBase::tableExists(const QString &tableName) {
    if (!open()) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name=:tableName");
    query.bindValue(":tableName", tableName);
    if (!query.exec()) {
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
            return playlist;
        }
    }

    QSqlQuery query(db);
    query.prepare("SELECT PlaylistID FROM Playlists WHERE Name = :name");
    query.bindValue(":name", playlistName);
    if (!query.exec() || !query.next()) {
        return playlist;
    }
    int playlistID = query.value(0).toInt();

    // Clear the playlist before adding tracks
    playlist.setTracks(std::vector<Track>());

    query.prepare("SELECT Mediathek.TrackID, Mediathek.FilePath, Mediathek.Interpret, Mediathek.Titel, Mediathek.Album, Mediathek.Spielzeit, Mediathek.SampleRate "
                  "FROM PlaylistTracks "
                  "JOIN Mediathek ON PlaylistTracks.TrackID = Mediathek.TrackID "
                  "WHERE PlaylistTracks.PlaylistID = :playlistID");
    query.bindValue(":playlistID", playlistID);
    if (query.exec()) {
        while (query.next()) {
            Track track;
            track.setTrackID(query.value("TrackID").toInt());
            track.setFilePath(query.value("FilePath").toString());
            track.setArtist(query.value("Interpret").toString());
            track.setTitle(query.value("Titel").toString());
            track.setAlbum(query.value("Album").toString());
            track.setDuration(query.value("Spielzeit").toInt());
            track.setSampleRate(query.value("SampleRate").toInt());
            playlist.addTrack(track);
        }
    }
    if (playlist.getTracks().empty()) {
        qWarning() << "Playlist" << playlistName << "does not exist or has no tracks.";
    }

    return playlist;
}

std::vector<QString> DataBase::getAllPlaylists() {
    std::vector<QString> playlists;

    if (!open()) {
        return playlists;
    }

    QSqlQuery query(db);
    query.prepare("SELECT Name FROM Playlists");
    if (!query.exec()) {
        return playlists;
    }

    while (query.next()) {
        playlists.push_back(query.value(0).toString());
    }

    return playlists;
}

bool DataBase::removePlaylist(const QString &playlistName) {
    if (!open()) {
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM Playlists WHERE Name = :name");
    query.bindValue(":name", playlistName);

    if (!query.exec()) {
        return false;
    }

    query.prepare("DELETE FROM PlaylistTracks WHERE PlaylistID = (SELECT PlaylistID FROM Playlists WHERE Name = :name)");
    query.bindValue(":name", playlistName);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool DataBase::createAllSongsPlaylist() {
    if (!db.isOpen()) {
        if (!open()) {
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
        return false;
    }

    // Insert all song IDs into PlaylistTracks with PlaylistID = 1
    query.prepare("INSERT INTO PlaylistTracks (PlaylistID, TrackID) VALUES (1, :trackID)");
    for (int trackID : songIDs) {
        query.bindValue(":trackID", trackID);
        if (!query.exec()) {
            return false;
        }
    }

    return true;
}

Track DataBase::getTrack(const QString &filePath) {
    QSqlQuery query(db);
    query.prepare("SELECT TrackID, FilePath, Interpret, Titel, Album, Spielzeit, SampleRate FROM Mediathek WHERE FilePath = :filePath");
    query.bindValue(":filePath", filePath);
    if (query.exec() && query.next()) {
        Track track;
        track.setTrackID(query.value("TrackID").toInt());
        track.setFilePath(query.value("FilePath").toString());
        track.setArtist(query.value("Interpret").toString());
        track.setTitle(query.value("Titel").toString());
        track.setAlbum(query.value("Album").toString());
        track.setDuration(query.value("Spielzeit").toInt());
        track.setSampleRate(query.value("SampleRate").toInt());
        return track;
    }
    return Track(); // Return an empty Track object if not found
}


