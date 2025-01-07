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
    void playPlaylist();
    void setPlaylist(const Playlist &playlist); // Add this method declaration
    QMediaPlayer* getPlayer();
    QAudioOutput* getAudioOutput();
    int getCurrentSongPosition();
    void setCurrentSongPosition(int position);
    qint64 getCurrentSongDuration();
    void updateCurrentSongDuration(qint64 duration);
    Track getCurrentTrack();
    int getCurrentIndex();
signals:
    void currentTrackChanged(int index, const QString &title);
    void positionChanged(int position);
private slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status); // Add this method declaration
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
