#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include <QCryptographicHash>

class Track {
public:
    Track();
    Track(const QString &filePath); // Add this constructor
    Track(const QString &basePath, const QString &fileName); // Update constructor to accept base path and file name
    QString getFilePath() const;
    QString getArtist() const;
    QString getAlbum() const;
    QString getTitle() const;
    int getDuration() const;
    int getSampleRate() const;
    int getSampleCount() const;
    QByteArray getHash() const; // Add this method to get the hash
    int getTrackID() const; // Add this method to get the track ID

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
    int trackID; // Add this member to store the track ID
};

#endif // TRACK_H