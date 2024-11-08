#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <iostream>
#include <vector>

// Vorwärtsdeklaration der MediaController-Klasse
class MediaController;
class DataBase;

class Playlist {
public:
    void addFile(const QString &file) {
        files.push_back(file);
    }

    void addFilesFromArgs(const QStringList &args, MediaController &mediaController, DataBase &database);

    void setFiles(const std::vector<QString> &newFiles) {
        files = newFiles;
    }

    void print() const {
        std::cout << "Number of files: " << files.size() << std::endl;
        for (const auto &file : files) {
            std::cout << QFileInfo(file).fileName().toStdString() << std::endl;
        }
    }

    const std::vector<QString>& getFiles() const {
        return files;
    }

private:
    std::vector<QString> files;
};

#endif // PLAYLIST_H