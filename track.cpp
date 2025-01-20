#include "track.h"
#include <QDir>
#include <QEventLoop>
#include <QFileInfo>
#include <QMediaMetaData>
#include <QMediaPlayer>

/**
 * Standardkonstruktor der Track-Klasse.
 * Initialisiert die Attribute mit Standardwerten.
 */
Track::Track()
    : duration(0)
    , sampleRate(0)
    , trackID(-1)
{}

/**
 * Konstruktor der Track-Klasse.
 * Initialisiert den Track anhand des Dateipfads und lädt Metadaten.
 * @param filePath Der Dateipfad des Tracks.
 */
Track::Track(const QString &filePath)
    : filePath(filePath)
    , duration(0)
    , sampleRate(0)
    , trackID(-1)
{
    if (QFileInfo(filePath).isRelative()) {
        this->filePath = QDir::current().absoluteFilePath(filePath);
    } else {
        this->filePath = filePath;
    }

    QMediaPlayer player;
    player.setSource(QUrl::fromLocalFile(this->filePath));

    // Warten, bis die Metadaten verfügbar sind
    QEventLoop loop;
    QObject::connect(&player,
                     &QMediaPlayer::mediaStatusChanged,
                     [&loop, &player](QMediaPlayer::MediaStatus status) {
                         if (status == QMediaPlayer::LoadedMedia
                             || status == QMediaPlayer::InvalidMedia) {
                             loop.quit();
                         }
                     });
    player.play();
    loop.exec();

    // Dauer des Tracks in Sekunden ermitteln
    duration = player.duration() / 1000;

    // Samplerate aus den Metadaten lesen, Standardwert verwenden, falls nicht verfügbar
    sampleRate = player.metaData().value(QMediaMetaData::AudioBitRate).toInt();
    if (sampleRate == 0) {
        sampleRate = 44100;
    }

    // Künstlername ermitteln
    QString albumArtist = player.metaData().value(QMediaMetaData::AlbumArtist).toString();
    QString contributingArtist = player.metaData().value(QMediaMetaData::ContributingArtist).toString();

    if (!albumArtist.isEmpty() && !contributingArtist.isEmpty()) {
        artist = albumArtist + ", " + contributingArtist;
    } else if (!albumArtist.isEmpty()) {
        artist = albumArtist;
    } else if (!contributingArtist.isEmpty()) {
        artist = contributingArtist;
    } else {
        artist = "unbekannt";
    }

    // Albumtitel ermitteln
    album = player.metaData().value(QMediaMetaData::AlbumTitle).toString();
    if (album.isEmpty()) {
        album = "unbekannt";
    }

    // Titel des Tracks ermitteln
    title = player.metaData().value(QMediaMetaData::Title).toString();
    if (title.isEmpty()) {
        title = QFileInfo(this->filePath).fileName();
    }
}

/**
 * Gibt den Dateipfad des Tracks zurück.
 * @return Der Dateipfad.
 */
QString Track::getFilePath() const
{
    return filePath;
}

/**
 * Gibt den Künstler des Tracks zurück.
 * @return Der Künstlername.
 */
QString Track::getArtist() const
{
    return artist;
}

/**
 * Gibt das Album des Tracks zurück.
 * @return Der Albumname.
 */
QString Track::getAlbum() const
{
    return album;
}

/**
 * Gibt den Titel des Tracks zurück.
 * @return Der Titel.
 */
QString Track::getTitle() const
{
    return title;
}

/**
 * Gibt die Dauer des Tracks in Sekunden zurück.
 * @return Die Dauer.
 */
int Track::getDuration() const
{
    return duration;
}

/**
 * Gibt die Samplerate des Tracks zurück.
 * @return Die Samplerate.
 */
int Track::getSampleRate() const
{
    return sampleRate;
}

/**
 * Gibt die ID des Tracks zurück.
 * @return Die Track-ID.
 */
int Track::getTrackID() const
{
    return trackID;
}

/**
 * Setzt den Dateipfad des Tracks.
 * @param filePath Der neue Dateipfad.
 */
void Track::setFilePath(const QString &filePath)
{
    this->filePath = filePath;
}

/**
 * Setzt den Künstler des Tracks.
 * @param artist Der neue Künstlername.
 */
void Track::setArtist(const QString &artist)
{
    this->artist = artist;
}

/**
 * Setzt das Album des Tracks.
 * @param album Der neue Albumname.
 */
void Track::setAlbum(const QString &album)
{
    this->album = album;
}

/**
 * Setzt den Titel des Tracks.
 * @param title Der neue Titel.
 */
void Track::setTitle(const QString &title)
{
    this->title = title;
}

/**
 * Setzt die Dauer des Tracks.
 * @param duration Die neue Dauer in Sekunden.
 */
void Track::setDuration(int duration)
{
    this->duration = duration;
}

/**
 * Setzt die Samplerate des Tracks.
 * @param sampleRate Die neue Samplerate.
 */
void Track::setSampleRate(int sampleRate)
{
    this->sampleRate = sampleRate;
}

/**
 * Setzt die ID des Tracks.
 * @param trackID Die neue Track-ID.
 */
void Track::setTrackID(int trackID)
{
    this->trackID = trackID;
}

/**
 * Gibt die Dauer des Tracks als formatierte Zeichenkette im Format mm:ss zurück.
 * @return Die formatierte Dauer.
 */
QString Track::getSongDurationAsString() const
{
    int totalSeconds = this->duration;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}
