
#include "playlistmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PlaylistManager::PlaylistManager(DataBase &db) : db(db) {}

bool PlaylistManager::addPlaylist(const QString &name) {
    return db.insertPlaylist(name);
}

bool PlaylistManager::addSongToPlaylist(const QString &playlistName, const Track &track) {
    return db.addSongToPlaylist(playlistName, track);
}

Playlist PlaylistManager::getPlaylist(const QString &name) {
    Playlist playlist;
    playlist.setName(name);

    QSqlQuery query(db.getDatabase());
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
        QSqlQuery trackQuery(db.getDatabase());
        trackQuery.prepare("SELECT FilePath FROM Mediathek WHERE TrackID = :trackID");
        trackQuery.bindValue(":trackID", trackID);

        if (trackQuery.exec() && trackQuery.next()) {
            QString filePath = trackQuery.value(0).toString();
            playlist.addFile(filePath);
        }
    }

    return playlist;
}

std::vector<QString> PlaylistManager::getAllPlaylists() {
    std::vector<QString> playlists;

    QSqlQuery query(db.getDatabase());
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