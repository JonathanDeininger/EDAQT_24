#ifndef SELECTTRACKDIALOG_H
#define SELECTTRACKDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "track.h" // Include the Track header

class SelectTrackDialog : public QDialog {
    Q_OBJECT

public:
    explicit SelectTrackDialog(const std::vector<Track> &tracks, QWidget *parent = nullptr);

    QString getSelectedTrack() const;

private slots:
    void onTrackSelected();

private:
    QListWidget *trackListWidget;
    QPushButton *selectButton;
    QString selectedTrack;
};

#endif // SELECTTRACKDIALOG_H