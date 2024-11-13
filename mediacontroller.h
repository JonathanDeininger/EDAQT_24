#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QObject>
#include <QStringList>
#include "Playlist.h"
#include "DataBase.h"

class MediaController : public QObject {
    Q_OBJECT

public:
    MediaController(Playlist &playlist);
    void initializePlayer();
    void playCurrent();
    void pauseCurrent();
    void next();
    void prev();
    void setCurrentIndex(int index);
    QStringList selectMediaFiles(DataBase &database);
    Playlist& getPlaylist() { return playlist; } // Add this method
    void addFolderToPlaylist(const QString &folderPath);
    QMediaPlayer* getPlayer() { return &player; }
    QAudioOutput* getAudioOutput() { return &audioOutput; }
    // Slider und Timer für
    int getCurrentSongPosition();
    void setCurrentSongPosition(int position);
    qint64 getCurrentSongDuration();
    void updateCurrentSongDuration(qint64 duration);
private:

    Playlist &playlist;
    QMediaPlayer player;
    QAudioOutput audioOutput;
    QString currentSource;

    qint64 songDuration = 0;
    int currentSongPosition = 0;
    int currentIndex;
};

#endif // MEDIACONTROLLER_H
