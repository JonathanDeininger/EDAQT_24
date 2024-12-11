#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>
#include <QDir>
#include <QListWidgetItem>
#include <QMessageBox>
#include "mediacontroller.h"
#include "database.h" // Include the database header
#include "Playlist.h" // Include the Playlist header

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
    void onChoosePlaylistButtonClicked();
    void onPlaylistItemClicked(QListWidgetItem *item);
    void onPlayButtonPressed();
    void onVolumeChanged(float value);
    void onPauseButtonPressed();
    void onNextButtonPressed();
    void onSliderPressed();
    void onPreviousButtonPressed();
    void updateProgressBar(qint64 currentSongPosition);
    void onSliderReleased();
    void loadTracksFromDatabase(); // Add this method to load tracks from the database
    void setProgressBarAndSongDurationLabel();
    void setCurrentSongDuration(Track currentTrack);
    void updateCurrentTrackInfo(int index, const QString &title);
    void onRandomButtonPressed();
    void onRepeatButtonPressed();
    void randomizePlaylist();
    void onSearchTextChanged(const QString &text);
private:
    Ui::MainWindow *ui;
    Playlist playList; // Add a Playlist member
    MediaController *mediaController;
    QTimer *progressTimer;
    int SliderPosition;
    DataBase db; // Add a database member
    bool sliderBeingDragged = false;
    bool isShuffleActive = false;
};

#endif // MAINWINDOW_H
