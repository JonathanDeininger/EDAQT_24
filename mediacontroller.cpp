#include "MediaController.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

/**
 * Konstruktor der MediaController-Klasse.
 * Initialisiert den MediaController mit einer Playlist und einem zugehörigen Widget.
 * @param playlistWidget Das Widget zur Anzeige der Playlist.
 * @param playlist Die Playlist, die gesteuert werden soll.
 */
MediaController::MediaController(QListWidget *playlistWidget, Playlist &playlist)
    : playlistWidget(playlistWidget)
    , playlist(playlist)
    , currentIndex(0)
    , currentTrack()
    , isShuffleActive(false)
    , currentRepeatMode(RepeatMode::NoRepeat)
{
    initializePlayer();
}

/**
 * Initialisiert den MediaPlayer und verbindet die entsprechenden Signale und Slots.
 */
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
                QMessageBox::critical(nullptr, "Error", "Error: " + errorString);
            });
    connect(&player,
            &QMediaPlayer::durationChanged,
            this,
            &MediaController::updateCurrentSongDuration);
    audioOutput.setVolume(0.5); // Standardlautstärke einstellen
}

/**
 * Setzt die Playlist, die gesteuert werden soll.
 * @param playlist Die neue Playlist.
 */
void MediaController::setPlaylist(const Playlist &playlist)
{
    this->playlist = playlist;
    currentIndex = 0; // Index initialisieren
}

/**
 * Spielt den aktuellen Song in der Playlist ab.
 */
void MediaController::playCurrent()
{
    if (playlist.getLength() == 0)
        return;

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
            // Fehlerbehandlung für fehlende Datei
        } else if (!file.open(QIODevice::ReadOnly)) {
            // Fehlerbehandlung für nicht lesbare Datei
        } else {
            file.close();
        }
        player.play();
    } else {
        // Fehlerbehandlung für ungültigen Index
    }
}

/**
 * Spielt die gesamte Playlist ab.
 */
void MediaController::playPlaylist()
{
    if (playlist.getLength() == 0) {
        return; // Leere Playlist
    }

    if (currentIndex < 0 || currentIndex >= playlist.getLength()) {
        currentIndex = 0;
    }

    playCurrent();
}

/**
 * Reagiert auf Änderungen des Medienstatus.
 * @param status Der neue Medienstatus.
 */
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
                player.stop();
            } else {
                playCurrent();
            }
            break;

        case RepeatMode::RepeatPlaylist:
            currentIndex++;
            if (currentIndex >= playlist.getLength()) {
                currentIndex = 0;
            }
            playCurrent();
            break;

        case RepeatMode::RepeatTrack:
            playCurrent();
            break;
        }

        emit currentTrackChanged(currentIndex, currentTrack.getTitle());
    }
}

/**
 * Pausiert die Wiedergabe des aktuellen Songs.
 */
void MediaController::pauseCurrent()
{
    player.pause();
}

/**
 * Springt zum nächsten Song in der Playlist.
 */
void MediaController::next()
{
    if (playlist.getLength() == 0)
        return;

    if (isShuffleActive) {
        if (currentIndex < shuffledIndices.size() - 1) {
            currentIndex++;
        } else {
            currentIndex = 0;
        }
    } else {
        if (currentIndex < playlist.getLength() - 1) {
            currentIndex++;
        }
    }

    playCurrent();
}

/**
 * Springt zum vorherigen Song in der Playlist.
 */
void MediaController::prev()
{
    if (currentIndex > 0) {
        currentIndex--;
        playCurrent();
    }
}

/**
 * Setzt den aktuellen Index in der Playlist.
 * @param index Der neue Index.
 */
void MediaController::setCurrentIndex(int index)
{
    if (index >= 0 && index < playlist.getLength()) {
        currentIndex = index;
    }
}

/**
 * Aktualisiert die Dauer des aktuellen Songs.
 * @param duration Die neue Dauer in Millisekunden.
 */
void MediaController::updateCurrentSongDuration(qint64 duration)
{
    songDuration = duration / 1000;
    currentTrack.setDuration(songDuration);
}

/**
 * Aktiviert oder deaktiviert den Shuffle-Modus und mischt die Playlist, falls aktiv.
 */
void MediaController::toggleShuffle()
{
    isShuffleActive = !isShuffleActive;
    if (isShuffleActive) {
        shufflePlaylist();
    }
    currentIndex = 0;
    playCurrent();
}

/**
 * Mischt die Playlist.
 */
void MediaController::shufflePlaylist()
{
    shuffledIndices.clear();
    for (int i = 0; i < playlist.getLength(); ++i) {
        shuffledIndices.append(i);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffledIndices.begin(), shuffledIndices.end(), g);
}

/**
 * Gibt die aktuelle Songdauer zurück.
 * @return Die Dauer des aktuellen Songs in Sekunden.
 */
qint64 MediaController::getCurrentSongDuration()
{
    return songDuration;
}

/**
 * Setzt die Wiedergabeposition des aktuellen Songs.
 * @param position Die neue Position in Sekunden.
 */
void MediaController::setCurrentSongPosition(int position)
{
    player.setPosition(position * 1000);
}

/**
 * Gibt die aktuelle Wiedergabeposition des Songs zurück.
 * @return Die aktuelle Position in Sekunden.
 */
int MediaController::getCurrentSongPosition()
{
    return player.position() / 1000;
}

/**
 * Gibt einen Zeiger auf den QMediaPlayer zurück.
 * @return Zeiger auf den MediaPlayer.
 */
QMediaPlayer *MediaController::getPlayer()
{
    return &player;
}

/**
 * Gibt einen Zeiger auf das QAudioOutput zurück.
 * @return Zeiger auf das AudioOutput.
 */
QAudioOutput *MediaController::getAudioOutput()
{
    return &audioOutput;
}

/**
 * Gibt den aktuellen Track zurück.
 * @return Der aktuelle Track.
 */
Track MediaController::getCurrentTrack()
{
    return currentTrack;
}

/**
 * Gibt den aktuellen Index in der Playlist zurück.
 * @return Der aktuelle Index.
 */
int MediaController::getCurrentIndex()
{
    if (isShuffleActive) {
        if (currentIndex >= 0 && currentIndex < shuffledIndices.size()) {
            return shuffledIndices[currentIndex];
        }
    }
    return currentIndex;
}

/**
 * Gibt zurück, ob der Shuffle-Modus aktiv ist.
 * @return true, wenn Shuffle aktiv ist, sonst false.
 */
bool MediaController::getIsShuffleActive()
{
    return isShuffleActive;
}

/**
 * Gibt den aktuellen Wiederholungsmodus zurück.
 * @return Der aktuelle Wiederholungsmodus.
 */
RepeatMode MediaController::getRepeatMode() const
{
    return currentRepeatMode;
}

/**
 * Setzt den Wiederholungsmodus.
 * @param RM Der neue Wiederholungsmodus.
 */
void MediaController::setRepeatMode(RepeatMode RM)
{
    currentRepeatMode = RM;
}
