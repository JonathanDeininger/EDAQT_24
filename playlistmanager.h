
#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QString>
#include <vector>
#include "database.h"
#include "playlist.h"
#include "track.h"

class PlaylistManager {
public:
    PlaylistManager(DataBase &db);
    bool addPlaylist(const QString &name);
    bool addSongToPlaylist(const QString &playlistName, const Track &track);
    Playlist getPlaylist(const QString &name);
    std::vector<QString> getAllPlaylists();

private:
    DataBase &db;
};

#endif // PLAYLISTMANAGER_H