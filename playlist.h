#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <vector>
#include "track.h"
#include "database.h" // Include the database header

class MediaController;
class DataBase;

class Playlist {
public:
    void addFile(const QString &filePath);
    void addTrack(const Track &track); // Add this method declaration
    std::vector<Track> getTracks() const;
    void addFilesFromArgs(const QStringList &args, MediaController &mediaController, DataBase &database);
    void setFiles(const QVector<QString> &newFiles);
    void print() const;
    const QVector<QString>& getFiles() const;
    void setName(const QString &name);
    QString getName() const;
    int getLength() const;

    std::vector<Track> tracks; // Make the tracks vector public

private:
    QVector<QString> files;
    QString name;
};

#endif // PLAYLIST_H
