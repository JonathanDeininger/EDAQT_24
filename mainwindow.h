#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData> // Include the QMimeData header
#include <QStandardItemModel>
#include <QTimer>
#include "Playlist.h" // Include the Playlist header
#include "database.h" // Include the database header
#include "mediacontroller.h"
#include "selecttrackdialog.h" // Include the SelectTrackDialog header

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPlaylistItemClicked(QListWidgetItem *item);
    void onSongTableItemClicked(const QModelIndex &index); // für später
    void onPlayButtonPressed();
    void onVolumeChanged(float value);
    void onPauseButtonPressed();
    void onNextButtonPressed();
    void onSliderPressed();
    void onPreviousButtonPressed();
    void updateProgressBar(qint64 currentSongPosition);
    void onSliderReleased();
    void setProgressBarAndSongDurationLabel();
    void setCurrentSongDuration(Track currentTrack);
    void updateCurrentTrackInfo();
    void onRandomButtonPressed();
    void onRepeatButtonPressed();
    void onSearchTextChanged(const QString &text);
    void onAddPlaylistButtonClicked();
    void onAddTrackButtonClicked(); // Add this method declaration
    void onRemovePlaylistButtonClicked(); // Add this method declaration
    void onAddFolderButtonClicked(); // Add this method declaration
    void onRefreshFilesButtonClicked(); // Add this method declaration
    void onPlaylistSammlungItemClicked(QListWidgetItem *item);
    void loadPlaylistsFromDatabase(); // Add this method to load playlists from the database
    void updateCurrentTrackDisplay(); // Add this method declaration

private:
    // ...existing code...
    void setupSongTable();
    void loadPlaylist(const QString &playlistName); // Add this method declaration
    void loadPlaylistInTable(
        const QString &
            playlistName); //testweise für SongTableView wenn das alles funktioniert löschen wir loadPlaylist
    // ...existing code...
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    Playlist playList; // Add a Playlist member
    MediaController *mediaController;
    QTimer *progressTimer;
    int SliderPosition;
    DataBase db; // Add a database member
    bool sliderBeingDragged = false;
    bool isShuffleActive = false;
};

#endif // MAINWINDOW_H
