#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QObject>
#include <QStringList>
#include <QListWidget>
#include "DataBase.h"
#include "track.h" // Include the Track header
#include "Playlist.h" // Include the Playlist header

class MediaController : public QObject {
    Q_OBJECT

public:
    MediaController(QListWidget *playlistWidget, Playlist &playlist);
    void initializePlayer();
    void playCurrent();
    void pauseCurrent();
    void next();
    void prev();
    void setCurrentIndex(int index);
    QStringList selectMediaFiles(DataBase &database);
    void addFolderToPlaylist(const QString &folderPath);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void playPlaylist();
    QMediaPlayer* getPlayer();
    QAudioOutput* getAudioOutput();
    int getCurrentSongPosition();
    void setCurrentSongPosition(int position);
    qint64 getCurrentSongDuration();
    void updateCurrentSongDuration(qint64 duration);
    Track getCurrentTrack();
    int getCurrentIndex();
    std::vector<Track> getCurrentPlaylist();
signals:
    void currentTrackChanged(int index, const QString &title);
    void positionChanged(int position);
private:
    QListWidget *playlistWidget;
    Playlist &playlist;
    QMediaPlayer player;
    QAudioOutput audioOutput;
    QString currentSource;
    qint64 songDuration = 0;
    int currentSongPosition = 0;
    int currentIndex = 0;
    Track currentTrack;

};

#endif // MEDIACONTROLLER_H
