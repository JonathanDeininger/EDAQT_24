#include "Playlist.h"
#include <iostream>

void Playlist::addFile(const QString &filePath) {
    tracks.emplace_back(filePath);
}

std::vector<Track> Playlist::getTracks() const {
    return tracks;
}

void Playlist::setFiles(const std::vector<QString> &newFiles) {
    files = newFiles;
}

void Playlist::print() const {
    std::cout << "Number of files: " << files.size() << std::endl;
    for (const auto &file : files) {
        std::cout << QFileInfo(file).fileName().toStdString() << std::endl;
    }
}

const std::vector<QString>& Playlist::getFiles() const {
    return files;
}

void Playlist::setName(const QString &name) {
    this->name = name;
}

QString Playlist::getName() const {
    return name;
}

int Playlist::getLength() const {
    return tracks.size();
}