#include "MediaController.h"
#include <QFileDialog>
#include <QDir>
#include <iostream>
#include <QMessageBox>

MediaController::MediaController(QListWidget *playlistWidget, Playlist &playlist)
    : playlistWidget(playlistWidget), playlist(playlist), currentIndex(0), currentTrack() {
    initializePlayer();
}

void MediaController::initializePlayer() {
    player.setAudioOutput(&audioOutput);
    connect(&player, &QMediaPlayer::positionChanged, [](qint64 position) {
        qDebug() << "Position changed:" << position;
    });
    connect(&player, &QMediaPlayer::mediaStatusChanged, [](QMediaPlayer::MediaStatus status) {
        qDebug() << "Media status changed:" << status;
    });
    connect(&player, &QMediaPlayer::errorOccurred, [](QMediaPlayer::Error error, const QString &errorString) {
        qDebug() << "Error occurred:" << error << errorString;
    });
    connect(&player, &QMediaPlayer::durationChanged, this, &MediaController::updateCurrentSongDuration);
    audioOutput.setVolume(0.5); // Set a default volume
}

void MediaController::playCurrent() {
    if (currentIndex >= 0 && currentIndex < playlist.getLength()) {
        QString newFile = playlist.getTracks()[currentIndex].getFilePath();
        qDebug() << "Attempting to play file:" << newFile;
        if (newFile != currentSource) {
            currentSource = newFile;
            player.setSource(QUrl::fromLocalFile(newFile));
            qDebug() << "Setting source to:" << newFile;
        }
        QFile file(newFile);
        if (!file.exists()) {
            qDebug() << "File does not exist:" << newFile;
        } else if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "File cannot be opened:" << newFile;
        } else {
            qDebug() << "File is accessible:" << newFile;
            file.close();
        }
        player.play();
        qDebug() << "Playing:" << newFile;
    } else {
        qDebug() << "Invalid index:" << currentIndex;
    }
}

void MediaController::pauseCurrent() {
    player.pause();
    qDebug() << "Paused";
}

void MediaController::next() {
    qDebug() << "Current index before next:" << currentIndex;
    if (currentIndex < playlist.getLength() - 1) {
        currentIndex++;
        qDebug() << "Current index after increment:" << currentIndex;
        playCurrent();
        qDebug() << "Next track:" << currentIndex;
    } else {
        qDebug() << "No next track available";
    }
}

void MediaController::prev() {
    qDebug() << "Current index before prev:" << currentIndex;
    if (currentIndex > 0) {
        currentIndex--;
        qDebug() << "Current index after decrement:" << currentIndex;
        playCurrent();
        qDebug() << "Previous track:" << currentIndex;
    } else {
        qDebug() << "No previous track available";
    }
}

void MediaController::addFolderToPlaylist(const QString &folderPath) {
    QDir dir(folderPath);
    if (!dir.exists()) {
        std::cerr << "Directory does not exist: " << folderPath.toStdString() << std::endl;
        return;
    }

    QStringList filters;
    filters << "*.mp3" << "*.mp4" << "*.wav" << "*.avi";
    QStringList files = dir.entryList(filters, QDir::Files);
    for (QString &file : files) {
        file = dir.absoluteFilePath(file);
        QListWidgetItem *item = new QListWidgetItem(file);
        item->setData(Qt::UserRole, file);
        playlistWidget->addItem(item);
        playlist.addFile(file);
    }

    if (files.isEmpty()) {
        std::cerr << "No media files found in directory: " << folderPath.toStdString() << std::endl;
    } else {
        std::cout << "Added " << files.size() << " files to the playlist from directory: " << folderPath.toStdString() << std::endl;
    }
}

void MediaController::setCurrentIndex(int index) {
    qDebug() << "Setting current index to:" << index;
    if (index >= 0 && index < playlist.getLength()) {
        currentIndex = index;
        qDebug() << "Current index set to:" << currentIndex;
    } else {
        std::cerr << "Invalid index" << std::endl;
    }
}

void MediaController::updateCurrentSongDuration(qint64 duration) {
    // Aktualisiere die Songdauer in Sekunden
    songDuration = duration / 1000;
    qDebug() << "Song duration updated to:" << songDuration;
}

qint64 MediaController::getCurrentSongDuration() {
    return songDuration;
}

void MediaController::setCurrentSongPosition(int position) {
    player.setPosition(position * 1000); // Set position in milliseconds
    qDebug() << "Current song position set to:" << position;
}

int MediaController::getCurrentSongPosition() {
    return player.position() / 1000; // Return position in seconds
}

QMediaPlayer* MediaController::getPlayer() {
    return &player;
}

QAudioOutput* MediaController::getAudioOutput() {
    return &audioOutput;
}

