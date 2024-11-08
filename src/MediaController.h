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
    void next();
    void prev();
    QStringList selectMediaFiles(DataBase &database);
    Playlist& getPlaylist() { return playlist; } // Add this method
    void addFolderToPlaylist(const QString &folderPath);

    QMediaPlayer* getPlayer() { return &player; }
    QAudioOutput* getAudioOutput() { return &audioOutput; }

private:
    Playlist &playlist;
    QMediaPlayer player;
    QAudioOutput audioOutput;
    int currentIndex;
};

#endif // MEDIACONTROLLER_H