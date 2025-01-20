#ifndef SELECTTRACKDIALOG_H
#define SELECTTRACKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>
#include "track.h"

class SelectTrackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTrackDialog(const std::vector<Track> &tracks, QWidget *parent = nullptr);

    Track getSelectedTrack() const;

private slots:
    void onTrackSelected();
    void onSearchTextChanged(const QString &text);

private:
    QLineEdit *searchBar;
    QTableView *trackTableView;
    QPushButton *selectButton;
    QStandardItemModel *model;
    Track selectedTrack;
    std::vector<Track> allTracks;
};

#endif
