#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <QDir>

class DataBase {
public:
    DataBase();
    ~DataBase();
    bool createTableMediathek();
    bool createTablePathlist(); // Update this method
    bool createTablePlaylists();
    bool createTablePlaylistTracks();
    bool insertData(const QString &filePath, const QString &interpret, const QString &album, const QString &titel, int spielzeit, int sampleRate, int sampleCount, const QByteArray &hash);
    bool insertPath(const QString &path); // Update this method
    bool insertPlaylist(const QString &playlistName);
    bool insertPlaylistTrack(int playlistID, int trackID);
    bool tableExists(const QString &tableName); // Add this method
    void queryData();
    bool open();
    void close();
    QSqlDatabase& getDatabase();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
