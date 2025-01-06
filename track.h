#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include <QCryptographicHash>

class Track {
public:
    Track();
    Track(const QString &filePath);
    QString getFilePath() const;
    QString getArtist() const;
    QString getAlbum() const;
    QString getTitle() const;
    int getDuration() const;
    int getSampleRate() const;
    int getSampleCount() const;
    QByteArray getHash() const;
    int getTrackID() const;
    void setFilePath(const QString &filePath);
    void setArtist(const QString &artist);
    void setAlbum(const QString &album);
    void setTitle(const QString &title);
    void setDuration(int duration);
    void setSampleRate(int sampleRate);
    void setSampleCount(int sampleCount);
    void setHash(const QByteArray &hash);
    void setTrackID(int trackID);

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
