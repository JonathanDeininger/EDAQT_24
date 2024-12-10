#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <vector>
#include "track.h"

// Vorwärtsdeklaration der MediaController-Klasse
class MediaController;
class DataBase;

class Playlist {
public:
    void addFile(const QString &filePath);
    std::vector<Track> getTracks() const;

    void addFilesFromArgs(const QStringList &args, MediaController &mediaController, DataBase &database);

    void setFiles(const std::vector<QString> &newFiles);

    void print() const;

    const std::vector<QString>& getFiles() const;

    void setName(const QString &name);
    QString getName() const;

    int getLength() const;

private:
    std::vector<Track> tracks;
    std::vector<QString> files;
    QString name;
};

#endif // PLAYLIST_H
