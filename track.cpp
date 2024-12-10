#include "track.h"
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QEventLoop>
#include <QFileInfo>
#include <QDir> // Include QDir
#include <qcryptographichash.h>

Track::Track() : duration(0), sampleRate(0), sampleCount(0), trackID(0) {}

Track::Track(const QString &filePath) : filePath(filePath), duration(0), sampleRate(0), sampleCount(0), trackID(0) {
    QMediaPlayer player;
    player.setSource(QUrl::fromLocalFile(filePath));

    // Warten, bis die Metadaten verfügbar sind
    QEventLoop loop;
    QObject::connect(&player, &QMediaPlayer::mediaStatusChanged, [&loop, &player](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia || status == QMediaPlayer::InvalidMedia) {
            loop.quit();
        }
    });
    player.play();
    loop.exec();

    duration = player.duration() / 1000; // Dauer in Sekunden
    sampleRate = player.metaData().value(QMediaMetaData::AudioBitRate).toInt();
    if (sampleRate == 0) {
        sampleRate = 44100; // Default sample rate if not available
    }
    sampleCount = sampleRate * duration;

    if (!player.metaData().isEmpty()) {
        artist = player.metaData().value(QMediaMetaData::AlbumArtist).toString();
        album = player.metaData().value(QMediaMetaData::AlbumTitle).toString();
        title = player.metaData().value(QMediaMetaData::Title).toString();
    } else {
        artist = "unbekannt";
        title = QFileInfo(filePath).fileName();
    }

    computeHash();
}

Track::Track(const QString &basePath, const QString &fileName) : duration(0), sampleRate(0), sampleCount(0), trackID(0) {
    filePath = QDir(basePath).absoluteFilePath(fileName);
    QMediaPlayer player;
    player.setSource(QUrl::fromLocalFile(filePath));

    // Warten, bis die Metadaten verfügbar sind
    QEventLoop loop;
    QObject::connect(&player, &QMediaPlayer::mediaStatusChanged, [&loop, &player](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia || status == QMediaPlayer::InvalidMedia) {
            loop.quit();
        }
    });
    player.play();
    loop.exec();

    duration = player.duration() / 1000; // Dauer in Sekunden
    sampleRate = player.metaData().value(QMediaMetaData::AudioBitRate).toInt();
    if (sampleRate == 0) {
        sampleRate = 44100; // Default sample rate if not available
    }
    sampleCount = sampleRate * duration;

    if (!player.metaData().isEmpty()) {
        artist = player.metaData().value(QMediaMetaData::AlbumArtist).toString();
        album = player.metaData().value(QMediaMetaData::AlbumTitle).toString();
        title = player.metaData().value(QMediaMetaData::Title).toString();
    } else {
        artist = "unbekannt";
        title = QFileInfo(filePath).fileName();
    }

    computeHash();
}

QString Track::getFilePath() const {
    return filePath;
}

QString Track::getArtist() const {
    return artist;
}

QString Track::getAlbum() const {
    return album;
}

QString Track::getTitle() const {
    return title;
}

int Track::getDuration() const {
    return duration;
}

int Track::getSampleRate() const {
    return sampleRate;
}

int Track::getSampleCount() const {
    return sampleCount;
}

QByteArray Track::getHash() const {
    return hash;
}

int Track::getTrackID() const {
    return trackID;
}

void Track::computeHash() {
    QCryptographicHash crypto(QCryptographicHash::Sha256);
    crypto.addData(filePath.toUtf8());
    crypto.addData(artist.toUtf8());
    crypto.addData(album.toUtf8());
    crypto.addData(title.toUtf8());
    crypto.addData(QString::number(duration).toUtf8());
    hash = crypto.result();
}