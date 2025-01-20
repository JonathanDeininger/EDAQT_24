#include "DataBase.h"
#include <QDir>
#include "playlist.h"
#include <QFile>
#include <QSqlQuery>
#include <QDebug>
#include <qdebug.h>
#include <qdialogbuttonbox.h>
#include <qlogging.h>
#include <QMessageBox>
#include <QObject>
#include <qmessagebox.h>

DataBase::DataBase() {
    if (QSqlDatabase::contains("MusicPlayerConnection")) {
        db = QSqlDatabase::database("MusicPlayerConnection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "MusicPlayerConnection");
        db.setDatabaseName("MusicPlayerSQLDB.db");

        if (!db.open()) {
            return;
        }
    }

    createTableMediathek();
    createTablePathlist();
    createTablePlaylists();
    createTablePlaylistTracks();
}

DataBase::~DataBase() {
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
    QSqlQuery query(db);

    bool success = query.prepare("CREATE TABLE IF NOT EXISTS Mediathek ("
                                 "TrackID INTEGER PRIMARY KEY AUTOINCREMENT, "
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
    QSqlQuery query(db);
    bool success = query.exec("CREATE TABLE IF NOT EXISTS Pathlist ("
                              "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "MusikPfad TEXT)");
    return success;
}

bool DataBase::createTablePlaylists() {
    if (!open()) {
        return false;
    }
    QSqlQuery query(db);
    bool success = query.exec("CREATE TABLE IF NOT EXISTS Playlists ("
                              "PlaylistID INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "Name TEXT)");
    return success;
}

bool DataBase::createTablePlaylistTracks() {
    if (!open()) {
        return false;
    }
    QSqlQuery query(db);
    bool success = query.exec("CREATE TABLE IF NOT EXISTS PlaylistTracks ("
                              "PlaylistID INTEGER, "
                              "TrackID INTEGER, "
                              "FOREIGN KEY (PlaylistID) REFERENCES Playlists(PlaylistID), "
                              "FOREIGN KEY (TrackID) REFERENCES Mediathek(TrackID))");
    return success;
}

bool DataBase::insertData(const QString &filePath, const QString &interpret, const QString &album, const QString &titel, int spielzeit, int sampleRate) {
    if (!open()) {
        return false;
    }
    QSqlQuery query(db);

    query.prepare("SELECT TrackID FROM Mediathek WHERE FilePath = :filePath");
    query.bindValue(":filePath", filePath);
    if (query.exec() && query.next()) {
        return false;
    }

    query.prepare("INSERT INTO Mediathek (FilePath, Interpret, Album, Titel, Spielzeit, SampleRate) "
                  "VALUES (:filePath, :interpret, :album, :titel, :spielzeit, :sampleRate)");

    query.bindValue(":filePath", filePath);
    query.bindValue(":interpret", interpret);
    query.bindValue(":album", album);
    query.bindValue(":titel", titel);
    query.bindValue(":spielzeit", spielzeit);
    query.bindValue(":sampleRate", sampleRate);
    if (!query.exec()) {
        return false;
    }

    int trackID = query.lastInsertId().toInt();

    return insertPlaylistTrack(1, trackID);
}

bool DataBase::insertPath(const QString &path) {
    if (!open()) {
        return false;
    }

    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        return false;
    }

    QSqlQuery query(db);

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
    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM Playlists WHERE Name = :name");
    query.bindValue(":name", playlistName);
    if (query.exec() && query.next()) {
        if (query.value(0).toInt() > 0) {
            return false;
        }
    }

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
    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM PlaylistTracks WHERE PlaylistID = :playlistID AND TrackID = :trackID");
    query.bindValue(":playlistID", playlistID);
    query.bindValue(":trackID", trackID);
    if (query.exec() && query.next()) {
        if (query.value(0).toInt() > 0) {
            QMessageBox::warning(nullptr, "Error", "Track gibts schon in der Playlist.");
            return false;
        }
    }

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

    QSqlQuery query(db);
    qDebug() << "removeTrack";
    query.prepare("DELETE FROM PlaylistTracks WHERE PlaylistID = :playlistID AND TrackID = :trackID");
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

    playlist.setPlaylistID(playlistID);
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
        playlists.push_back(playlist);
    }

    return playlists;
}

//löscht eine playlist aus der datenbank

bool DataBase::removePlaylist(const QString &playlistName) {
    if (playlistName == "Alle Songs") {
        return false;
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
    return Track();
}

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
                QSqlQuery deleteQuery(getDatabase());
                deleteQuery.prepare("DELETE FROM Mediathek WHERE TrackID = :trackID");
                deleteQuery.bindValue(":trackID", trackID);
                deleteQuery.exec();

                deleteQuery.prepare("DELETE FROM PlaylistTracks WHERE TrackID = :trackID");
                deleteQuery.bindValue(":trackID", trackID);
                deleteQuery.exec();
            }
        }
    }

    query.prepare("SELECT MusikPfad FROM Pathlist");
    if (query.exec()) {
        while (query.next()) {
            QString directoryPath = query.value("MusikPfad").toString();
            QDir directory(directoryPath);
            if (directory.exists()) {
                processDirectory(directory);
            } else {
                QSqlQuery deleteQuery(getDatabase());
                deleteQuery.prepare("DELETE FROM Pathlist WHERE MusikPfad = :directoryPath");
                deleteQuery.bindValue(":directoryPath", directoryPath);
                deleteQuery.exec();
            }
        }
    }
}

void DataBase::processFiles(const QString &basePath, const QStringList &fileList) {
    foreach (const QString &fileName, fileList) {
        Track track(QDir(basePath).absoluteFilePath(fileName));
        if (!insertData(track.getFilePath(), track.getArtist(), track.getAlbum(), track.getTitle(), track.getDuration(), track.getSampleRate())) {
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