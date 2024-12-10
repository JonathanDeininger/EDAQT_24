#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <QDir>
#include "track.h"
#include "playlist.h"

class Playlist; // Forward declaration of Playlist class

class DataBase {
public:
    DataBase();
    ~DataBase();
    bool createTableMediathek();
    bool createTablePathlist();
    bool createTablePlaylists();
    bool createTablePlaylistTracks();
    bool insertData(const QString &filePath, const QString &interpret, const QString &album, const QString &titel, int spielzeit, int sampleRate, int sampleCount, const QByteArray &hash);
    bool insertPath(const QString &path);
    bool insertPlaylist(const QString &playlistName);
    bool insertPlaylistTrack(int playlistID, int trackID);
    bool tableExists(const QString &tableName);
    void queryData();
    bool open();
    void close();
    QSqlDatabase& getDatabase();
    bool createPlaylist(const QString &name, const std::vector<Track> &tracks);
    bool addSongToPlaylist(const QString &playlistName, const Track &track);
    Playlist getPlaylist(const QString &name);
    std::vector<QString> getAllPlaylists(); // Add this method

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
