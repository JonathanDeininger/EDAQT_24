#include "Playlist.h"

/**
 * Fügt einen Track zur Playlist hinzu.
 * @param track Der Track, der hinzugefügt werden soll.
 */
void Playlist::addTrack(const Track &track)
{
    tracks.push_back(track);
}

/**
 * Gibt alle Tracks der Playlist zurück.
 * @return Ein Vektor mit den Tracks der Playlist.
 */
std::vector<Track> Playlist::getTracks() const
{
    return tracks;
}

/**
 * Setzt die Tracks der Playlist.
 * Löscht die alten Tracks und ersetzt sie durch die neuen.
 * @param newTracks Ein Vektor mit den neuen Tracks.
 */
void Playlist::setTracks(const std::vector<Track> &newTracks)
{
    tracks.clear(); // Alte Tracks löschen
    tracks = newTracks;
}

/**
 * Setzt den Namen der Playlist.
 * @param name Der neue Name der Playlist.
 */
void Playlist::setName(const QString &name)
{
    this->name = name;
}

/**
 * Gibt den Namen der Playlist zurück.
 * @return Der Name der Playlist.
 */
QString Playlist::getName() const
{
    return name;
}

/**
 * Gibt die Anzahl der Tracks in der Playlist zurück.
 * @return Die Anzahl der Tracks.
 */
int Playlist::getLength() const
{
    return tracks.size();
}

/**
 * Gibt die ID der Playlist zurück.
 * @return Die ID der Playlist.
 */
int Playlist::getPlaylistID() const
{
    return playlistID;
}

/**
 * Setzt die ID der Playlist.
 * @param id Die neue ID der Playlist.
 */
void Playlist::setPlaylistID(int id)
{
    playlistID = id;
}
