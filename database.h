#ifndef DATABASE_H
#define DATABASE_H

#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include "playlist.h"
#include "track.h"

class Playlist;

class DataBase
{
public:
    DataBase();
    ~DataBase();
    bool createTableMediathek();
    bool createTablePathlist();
    bool createTablePlaylists();
    bool createTablePlaylistTracks();
    bool insertData(const QString &filePath,
                    const QString &interpret,
                    const QString &album,
                    const QString &titel,
                    int spielzeit,
                    int sampleRate);
    bool insertPath(const QString &path);
    bool insertPlaylist(const QString &playlistName);
    bool insertPlaylistTrack(int playlistID, int trackID);
    bool tableExists(const QString &tableName);
    bool open();
    void close();
    QSqlDatabase &getDatabase();
    Playlist getPlaylist(const QString &playlistName);
    std::vector<Playlist> getAllPlaylists();
    bool removePlaylist(const QString &playlistName);
    int getPlaylistID(const QString &playlistName);
    Track getTrack(const QString &filePath);
    void checkFiles();
    void processFiles(const QString &basePath, const QStringList &fileList);
    void processDirectory(const QDir &directory);
    bool removeTrackFromPlaylist(int playlistID, int trackID);

private:
    QSqlDatabase db;
};

#endif
