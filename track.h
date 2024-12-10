#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include <QCryptographicHash>

class Track {
public:
    Track();
    Track(const QString &filePath); // Update this constructor to handle both cases
    QString getFilePath() const;
    QString getArtist() const;
    QString getAlbum() const;
    QString getTitle() const;
    int getDuration() const;
    int getSampleRate() const;
    int getSampleCount() const;
    QByteArray getHash() const;
    int getTrackID() const;

private:
    void computeHash();

    QString filePath;
    QString artist;
    QString album;
    QString title;
    int duration;
    int sampleRate;
    int sampleCount;
    QByteArray hash;
    int trackID;
};

#endif // TRACK_H