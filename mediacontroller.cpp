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
    connect(&player, &QMediaPlayer::positionChanged, [this](qint64 position) {
        emit positionChanged(position / 1000);
    });
    connect(&player, &QMediaPlayer::mediaStatusChanged, this, &MediaController::onMediaStatusChanged);
    connect(&player, &QMediaPlayer::errorOccurred, [](QMediaPlayer::Error error, const QString &errorString) {
        // Handle error
    });
    connect(&player, &QMediaPlayer::durationChanged, this, &MediaController::updateCurrentSongDuration);
    audioOutput.setVolume(0.5); // Set a default volume
}

void MediaController::setPlaylist(const Playlist &playlist) {
    this->playlist = playlist;
    currentIndex = 0; // Initialize currentIndex
}

void MediaController::playCurrent() {
    if (currentIndex >= 0 && currentIndex < playlist.getLength()) {
        currentTrack = playlist.getTracks()[currentIndex];

        QString newFile = playlist.getTracks()[currentIndex].getFilePath();
        if (newFile != currentSource) {
            currentSource = newFile;
            player.setSource(QUrl::fromLocalFile(newFile));
        }
        QFile file(newFile);
        if (!file.exists()) {
            // Handle file not existing
        } else if (!file.open(QIODevice::ReadOnly)) {
            // Handle file not opening
        } else {
            file.close();
        }
        player.play();
    } else {
        // Handle invalid index
    }
}

void MediaController::playPlaylist() {
    if (playlist.getLength() == 0) {
        // Handle empty playlist
        return;
    }

    if (currentIndex < 0 || currentIndex >= playlist.getLength()) {
        currentIndex = 0;
    }

    playCurrent();

    connect(&player, &QMediaPlayer::mediaStatusChanged, this, &MediaController::onMediaStatusChanged);
}

void MediaController::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::LoadedMedia) {
        songDuration = player.duration();
        emit currentTrackChanged(currentIndex, currentTrack.getTitle());
    }
    // Prüfe, ob der aktuelle Track fertig abgespielt wurde
    if (status == QMediaPlayer::EndOfMedia) {
        // Gehe zum nächsten Track
        currentIndex++;

        // Wenn das Ende der Playlist erreicht ist, stoppe oder starte erneut (je nach gewünschtem Verhalten)
        if (currentIndex >= playlist.getLength()) {
            currentIndex = 0; // Optional: Zurück zum Anfang der Playlist
            return;           // Oder: return, um die Wiedergabe zu stoppen
        }

        // Spiele den nächsten Track
        playCurrent();

        // Hole den aktuellen Track
        Track currentTrack = getCurrentTrack();
        QString currentTitle = currentTrack.getTitle();

        // Aktualisiere die UI (über ein Signal an MainWindow)
        emit currentTrackChanged(currentIndex, currentTitle);
    }
}

void MediaController::pauseCurrent() {
    player.pause();
}

void MediaController::next() {
    if (currentIndex < playlist.getLength() - 1) {
        currentIndex++;
        playCurrent();
    } else {
        // Handle no next track available
    }
}

void MediaController::prev() {
    if (currentIndex > 0) {
        currentIndex--;
        playCurrent();
    } else {
        // Handle no previous track available
    }
}

void MediaController::setCurrentIndex(int index) {
    if (index >= 0 && index < playlist.getLength()) {
        currentIndex = index;
    } else {
        std::cerr << "Invalid index" << std::endl;
    }
}

void MediaController::updateCurrentSongDuration(qint64 duration) {
    // Aktualisiere die Songdauer in Sekunden
    songDuration = duration / 1000;
    currentTrack.setDuration(songDuration); // Update the current track's duration
}

qint64 MediaController::getCurrentSongDuration() {
    return songDuration;
}

void MediaController::setCurrentSongPosition(int position) {
    player.setPosition(position * 1000); // Set position in milliseconds
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

Track MediaController::getCurrentTrack() {
    return currentTrack;
}

int MediaController::getCurrentIndex() {
    return currentIndex;
}
