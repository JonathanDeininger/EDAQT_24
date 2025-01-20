#include "MediaController.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

MediaController::MediaController(QListWidget *playlistWidget, Playlist &playlist)
    : playlistWidget(playlistWidget)
    , playlist(playlist)
    , currentIndex(0)
    , currentTrack(),isShuffleActive(false),currentRepeatMode(RepeatMode::NoRepeat)
{
    initializePlayer();
}

void MediaController::initializePlayer()
{
    player.setAudioOutput(&audioOutput);
    connect(&player, &QMediaPlayer::positionChanged, [this](qint64 position) {
        emit positionChanged(position / 1000);
    });
    connect(&player,
            &QMediaPlayer::mediaStatusChanged,
            this,
            &MediaController::onMediaStatusChanged);
    connect(&player,
            &QMediaPlayer::errorOccurred,
            [](QMediaPlayer::Error error, const QString &errorString) {
                // Handle error by opening a message box that says "Error occurred while playing media"
                QMessageBox::critical(nullptr, "Error", "Error: " + errorString);

            });
    connect(&player,
            &QMediaPlayer::durationChanged,
            this,
            &MediaController::updateCurrentSongDuration);
    audioOutput.setVolume(0.5); // Set a default volume
}

void MediaController::setPlaylist(const Playlist &playlist)
{
    this->playlist = playlist;
    currentIndex = 0; // Initialize currentIndex
}

void MediaController::playCurrent()
{
    if (playlist.getLength() == 0) return;

    int indexToPlay = currentIndex;
    if (isShuffleActive && currentIndex < shuffledIndices.size()) {
        indexToPlay = shuffledIndices[currentIndex];
    }

    if (indexToPlay >= 0 && indexToPlay < playlist.getLength()) {
        currentTrack = playlist.getTracks()[indexToPlay];
        QString newFile = currentTrack.getFilePath();

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

void MediaController::playPlaylist()
{
    if (playlist.getLength() == 0) {
        // Handle empty playlist
        return;
    }

    if (currentIndex < 0 || currentIndex >= playlist.getLength()) {
        currentIndex = 0;
    }

    playCurrent();

    //connect(&player,&QMediaPlayer::mediaStatusChanged,this,&MediaController::onMediaStatusChanged);
}

void MediaController::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia) {
        songDuration = player.duration();
        emit currentTrackChanged(currentIndex, currentTrack.getTitle());
    }

    if (status == QMediaPlayer::EndOfMedia) {
        switch (currentRepeatMode) {
        case RepeatMode::NoRepeat:
            currentIndex++;
            if (currentIndex >= playlist.getLength()) {
                player.stop(); // Beendet die Wiedergabe, wenn die Playlist beendet ist
            } else {
                playCurrent(); // Spielt den nächsten Track ab
            }
            break;

        case RepeatMode::RepeatPlaylist:
            currentIndex++;
            qDebug() << "Aktueller Index: " << currentIndex;
            if (currentIndex >= playlist.getLength()) {
                currentIndex = 0; // Setzt zum Anfang der Playlist zurück
                qDebug() << "Shuffle: " << isShuffleActive;
            }
            playCurrent(); // Spielt den nächsten Track ab oder beginnt von vorne
            break;

        case RepeatMode::RepeatTrack:
            playCurrent(); // Spielt den gleichen Track erneut ab
            break;
        }

        // Messenger für UI-Updates mit dem aktuellen Track im Playlist-Index
        emit currentTrackChanged(currentIndex, currentTrack.getTitle());
    }
}

void MediaController::pauseCurrent()
{
    player.pause();
}

void MediaController::next()
{
    if (playlist.getLength() == 0) return;

    // Falls Shuffle aktiv ist, wähle den nächsten zufälligen Titel
    if (isShuffleActive) {
        if (currentIndex < shuffledIndices.size() - 1) {
            currentIndex++;
        } else {
            currentIndex = 0; // Falls am Ende, von vorn anfangen
        }
    } else {
        if (currentIndex < playlist.getLength() - 1) {
            currentIndex++;
        } else {
            // Handle no next track available
        }
    }

    playCurrent();
}

void MediaController::prev()
{
    if (currentIndex > 0) {
        currentIndex--;
        playCurrent();
    } else {
        // Handle no previous track available
    }
}

void MediaController::setCurrentIndex(int index)
{
    if (index >= 0 && index < playlist.getLength()) {
        currentIndex = index;
    } else {
        std::cerr << "Invalid index" << std::endl;
    }
}

void MediaController::updateCurrentSongDuration(qint64 duration)
{
    // Aktualisiere die Songdauer in Sekunden
    songDuration = duration / 1000;
    currentTrack.setDuration(songDuration); // Update the current track's duration
}

void MediaController::shufflePlaylist() {
    shuffledIndices.clear();
    for (int i = 0; i < playlist.getLength(); ++i) {
        shuffledIndices.append(i);
    }

    // Shuffle the indices
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffledIndices.begin(), shuffledIndices.end(), g);
}

void MediaController::toggleShuffle() {
    isShuffleActive = !isShuffleActive;
    if (isShuffleActive) {
        shufflePlaylist();
    }
    currentIndex = 0;
    playCurrent();
}


qint64 MediaController::getCurrentSongDuration()
{
    return songDuration;
}

void MediaController::setCurrentSongPosition(int position)
{
    player.setPosition(position * 1000); // Set position in milliseconds
}

int MediaController::getCurrentSongPosition()
{
    return player.position() / 1000; // Return position in seconds
}

QMediaPlayer *MediaController::getPlayer()
{
    return &player;
}

QAudioOutput *MediaController::getAudioOutput()
{
    return &audioOutput;
}

Track MediaController::getCurrentTrack()
{
    return currentTrack;
}

int MediaController::getCurrentIndex()
{

    if (isShuffleActive) {
        // Wenn Shuffle aktiv ist, geben Sie den aktuellen gemischten Index zurück.
        if (currentIndex >= 0 && currentIndex < shuffledIndices.size()) {
            return shuffledIndices[currentIndex];
        } else {
            std::cerr << "Fehler: currentIndex außerhalb des gültigen Bereichs im Shuffle" << std::endl;
            return -1; // Oder behandeln Sie den Fehler auf eine geeignete Weise.
        }
    } else {
        // Wenn kein Shuffle aktiv ist, geben Sie den aktuellen normalen Index zurück.
        return currentIndex;
    }
}

bool MediaController::getIsShuffleActive() {
    return isShuffleActive;
}

RepeatMode MediaController::getRepeatMode() const{

    return currentRepeatMode;

}

void MediaController::setRepeatMode(RepeatMode RM)
{
    currentRepeatMode =  RM;
}
