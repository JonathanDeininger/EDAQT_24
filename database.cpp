#include "DataBase.h"
#include <QDir>
#include "playlist.h"
#include <QFile>
#include <QSqlQuery>
#include <QDebug>
#include <qdebug.h>

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

    // Check if the file path already exists in the database
    query.prepare("SELECT TrackID FROM Mediathek WHERE FilePath = :filePath");
    query.bindValue(":filePath", filePath);
    if (query.exec() && query.next()) {
        qDebug() << "File path already exists in the database:" << filePath;
        return false; // File path already exists
    }

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

    // Get the TrackID of the newly inserted track
    int trackID = query.lastInsertId().toInt();

    // Add the track to the "Alle Songs" playlist<
    return insertPlaylistTrack(1, trackID);
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

    // Check if the playlist already exists
    query.prepare("SELECT COUNT(*) FROM Playlists WHERE Name = :name");
    query.bindValue(":name", playlistName);
    if (query.exec() && query.next()) {
        if (query.value(0).toInt() > 0) {
            return false; // Playlist already exists
        }
    }

    // Insert the new playlist
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

bool DataBase::removeTrackFromPlaylist(int playlistID, int trackID) {
    if (!open()) {
        return false;
    }
    QSqlQuery query(db); // Use the correct database connection

    query.prepare("DELETE FROM PlaylistTracks WHERE PlaylistID = :playlistID AND TrackID = :trackID");
    query.bindValue(":playlistID", playlistID);
    query.bindValue(":trackID", trackID);

    if (!query.exec()) {
        return false;
        qDebug() << "Track konnte nicht entfernt werden.";
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

    playlist.setPlaylistID(playlistID);
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

std::vector<Playlist> DataBase::getAllPlaylists() {
    std::vector<Playlist> playlists;

    if (!open()) {
        return playlists;
    }

    QSqlQuery query(db);
    query.prepare("SELECT PlaylistID, Name FROM Playlists");
    if (!query.exec()) {
        return playlists;
    }

    while (query.next()) {
        int playlistID = query.value("PlaylistID").toInt();
        QString playlistName = query.value("Name").toString();
        Playlist playlist;
        playlist.setPlaylistID(playlistID);
        playlist.setName(playlistName);
        playlists.push_back(playlist); // Push the Playlist object into the vector
    }

    // Debug output for all playlist IDs and names
    for (const auto &playlist : playlists) {
        qDebug() << "Playlist ID:" << playlist.getPlaylistID() << "Name:" << playlist.getName();
    }

    return playlists;
}

bool DataBase::removePlaylist(const QString &playlistName) {
    if (playlistName == "Alle Songs") {
        return false; // Prevent deletion of the "Alle Songs" playlist
    }

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

//diese funktion überprüft ob die Dateien noch vorhanden sind und durchsucht die Pfade in der Pathlist nach neuen Dateien

void DataBase::checkFiles() {
    if (!open()) {
        return;
    }
    QSqlQuery query(getDatabase());
    query.prepare("SELECT TrackID, FilePath FROM Mediathek");
    if (query.exec()) {
        while (query.next()) {
            int trackID = query.value("TrackID").toInt();
            QString filePath = query.value("FilePath").toString();
            if (!QFile::exists(filePath)) {
                qDebug() << "File missing:" << filePath;

                // Delete the track from Mediathek
                QSqlQuery deleteQuery(getDatabase());
                deleteQuery.prepare("DELETE FROM Mediathek WHERE TrackID = :trackID");
                deleteQuery.bindValue(":trackID", trackID);
                deleteQuery.exec();

                // Delete the track from PlaylistTracks
                deleteQuery.prepare("DELETE FROM PlaylistTracks WHERE TrackID = :trackID");
                deleteQuery.bindValue(":trackID", trackID);
                deleteQuery.exec();
                qDebug() << "Track removed from database:" << trackID;
            }
        }
    }

    // Check directories in Pathlist for new files
    query.prepare("SELECT MusikPfad FROM Pathlist");
    if (query.exec()) {
        while (query.next()) {
            QString directoryPath = query.value("MusikPfad").toString();
            QDir directory(directoryPath);
            if (directory.exists()) {
                processDirectory(directory);
            } else {
                // Remove the directory from Pathlist if it no longer exists
                QSqlQuery deleteQuery(getDatabase());
                deleteQuery.prepare("DELETE FROM Pathlist WHERE MusikPfad = :directoryPath");
                deleteQuery.bindValue(":directoryPath", directoryPath);
                deleteQuery.exec();
                qDebug() << "Directory removed from Pathlist:" << directoryPath;
            }
        }
    }
}

void DataBase::processFiles(const QString &basePath, const QStringList &fileList) {
    foreach (const QString &fileName, fileList) {
        Track track(QDir(basePath).absoluteFilePath(fileName));
        // Daten in die Datenbank einfügen
        if (!insertData(track.getFilePath(), track.getArtist(), track.getAlbum(), track.getTitle(), track.getDuration(), track.getSampleRate())) {
            // Handle data insertion failure
        }
    }
}

void DataBase::processDirectory(const QDir &directory) {
    QStringList audioFiles = directory.entryList(QStringList() << "*.mp3" << "*.wav" << "*.flac" << "*.aac", QDir::Files);
    processFiles(directory.absolutePath(), audioFiles);

    QStringList subDirs = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &subDir, subDirs) {
        QDir subDirectory(directory.absoluteFilePath(subDir));
        processDirectory(subDirectory);
    }
}