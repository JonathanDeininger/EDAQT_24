#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QString>
#include "track.h"
#include <vector>

class MediaController;
class DataBase;

class Playlist
{
public:
    void addTrack(const Track &track);
    std::vector<Track> getTracks() const;
    void setTracks(const std::vector<Track> &newTracks);
    void setName(const QString &name);
    QString getName() const;
    int getLength() const;
    int getPlaylistID() const;
    void setPlaylistID(int id);

private:
    QString name;
    int playlistID;
    std::vector<Track> tracks;
};

#endif
