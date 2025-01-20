#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#include <QAudioOutput>
#include <QListWidget>
#include <QMediaPlayer>
#include <QObject>
#include <QStringList>
#include "DataBase.h"
#include "MediaController.h"
#include "Playlist.h" // Include the Playlist header
#include "track.h"    // Include the Track header
#include <algorithm> // Für std::random_shuffle oder std::shuffle
#include <random>    // Für std::default_random_engine

enum class RepeatMode {
    NoRepeat,
    RepeatPlaylist,
    RepeatTrack
};

class MediaController : public QObject
{
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
    QMediaPlayer *getPlayer();
    QAudioOutput *getAudioOutput();
    int getCurrentSongPosition();
    void setCurrentSongPosition(int position);
    qint64 getCurrentSongDuration();
    void updateCurrentSongDuration(qint64 duration);
    Track getCurrentTrack();
    int getCurrentIndex();
    void shufflePlaylist();
    void toggleShuffle();
    bool getIsShuffleActive();
    RepeatMode getRepeatMode() const;
    void setRepeatMode(RepeatMode mode);

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
    QVector<int> shuffledIndices;
    bool isShuffleActive;
    RepeatMode currentRepeatMode;
};

#endif // MEDIACONTROLLER_H
