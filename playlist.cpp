#include "Playlist.h"
#include <iostream>
#include <qlogging.h>

// von Sam ertgboinregjnerghojkgres
void Playlist::addFile(const QString &filePath) {
    qDebug() << "Adding file to playlist:" << filePath; // Debug statement
    files.append(filePath);
    Track track;
    track.setFilePath(filePath);
    addTrack(track); // Ensure track is added when file is added
}

void Playlist::addTrack(const Track &track) {
    qDebug() << "Adding track to playlist:" << track.getTitle(); // Debug statement
    tracks.push_back(track);
}

std::vector<Track> Playlist::getTracks() const {
    qDebug() << "Retrieving tracks from playlist. Number of tracks:" << tracks.size(); // Debug statement
    return tracks;
}

void Playlist::setFiles(const QVector<QString> &newFiles) {
    files = newFiles;
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
    return files.size();
}

void Playlist::randomize() {
    std::random_shuffle(tracks.begin(), tracks.end());
    files.clear();
    for (const auto &track : tracks) {
        files.append(track.getFilePath());
    }
    qDebug() << "Playlist randomized with" << tracks.size() << "tracks.";
}

int Playlist::getPlaylistID() const {
    return playlistID;
}

void Playlist::setPlaylistID(int id) {
    playlistID = id;
}
