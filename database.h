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
    bool insertData(const QString &filePath, const QString &interpret, const QString &album, const QString &titel, int spielzeit, int sampleRate);
    bool insertPath(const QString &path);
    bool insertPlaylist(const QString &playlistName);
    bool insertPlaylistTrack(int playlistID, int trackID);
    bool tableExists(const QString &tableName);
    bool open();
    void close();
    QSqlDatabase& getDatabase();
    Playlist getPlaylist(const QString &playlistName); // Deklaration der Methode
    std::vector<QString> getAllPlaylists();
    bool removePlaylist(const QString &playlistName);
    int getPlaylistID(const QString &playlistName); // Add this method declaration
    // int getTrackID(const QString &filePath); // Add this method declaration
    Track getTrack(const QString &filePath); // Add this method declaration
    void checkFiles(); // Add this method declaration
    void processFiles(const QString &basePath, const QStringList &fileList);
    void processDirectory(const QDir &directory);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
