#include "Playlist.h"
#include <iostream>
#include <qlogging.h>

// von Sam ertgboinregjnerghojkgres
void Playlist::addFile(const QString &filePath) {
    files.append(filePath);
    Track track(filePath);
    tracks.push_back(track);
}

void Playlist::addTrack(const Track &track) {
    files.append(track.getFilePath());
    tracks.push_back(track);
    qDebug() << "Track added to playlist:" << track.getFilePath();
}

std::vector<Track> Playlist::getTracks() const {
    return tracks;
}

void Playlist::setFiles(const QVector<QString> &newFiles) {
    files = newFiles;
    tracks.clear();
    for (const auto &filePath : files) {
        Track track(filePath);
        tracks.push_back(track);
    }
}

void Playlist::print() const {
    std::cout << "Number of files: " << files.size() << std::endl;
    for (const auto &file : files) {
        std::cout << QFileInfo(file).fileName().toStdString() << std::endl;
    }
}

const QVector<QString>& Playlist::getFiles() const {
    return files;
}

void Playlist::setName(const QString &name) {
    this->name = name;
}

QString Playlist::getName() const {
    return name;
}

int Playlist::getLength() const {
    return tracks.size(); // Return the correct number of tracks
}
