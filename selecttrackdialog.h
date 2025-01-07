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

    Track getSelectedTrack() const;

private slots:
    void onTrackSelected();

private:
    QListWidget *trackListWidget;
    QPushButton *selectButton;
    Track selectedTrack;
};

#endif // SELECTTRACKDIALOG_H