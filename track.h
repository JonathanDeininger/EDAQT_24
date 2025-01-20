#ifndef TRACK_H
#define TRACK_H

#include <QString>

class Track
{
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
    int getTrackID() const;
    void setFilePath(const QString &filePath);
    void setArtist(const QString &artist);
    void setAlbum(const QString &album);
    void setTitle(const QString &title);
    void setDuration(int duration);
    void setSampleRate(int sampleRate);
    void setTrackID(int trackID);
    QString getSongDurationAsString() const;

private:
    QString filePath;
    QString artist;
    QString album;
    QString title;
    int duration;
    int sampleRate;
    int trackID;
};

#endif // TRACK_H
