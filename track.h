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
    void setFilePath(const QString &filePath); // Add this method declaration
    void setArtist(const QString &artist); // Add this method declaration
    void setAlbum(const QString &album); // Add this method declaration
    void setTitle(const QString &title); // Add this method declaration
    void setDuration(int duration); // Add this method declaration
    void setSampleRate(int sampleRate); // Add this method declaration
    void setSampleCount(int sampleCount); // Add this method declaration
    void setHash(const QByteArray &hash); // Add this method declaration
    void setTrackID(int trackID); // Add this method declaration

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
