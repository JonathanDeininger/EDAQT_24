#include "Playlist.h"

void Playlist::addTrack(const Track &track) {
    tracks.push_back(track);
}

std::vector<Track> Playlist::getTracks() const {
    return tracks;
}

void Playlist::setTracks(const std::vector<Track> &newTracks) {
    tracks.clear(); // Clear the old tracks
    tracks = newTracks;
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

int Playlist::getPlaylistID() const {
    return playlistID;
}

void Playlist::setPlaylistID(int id) {
    playlistID = id;
}
