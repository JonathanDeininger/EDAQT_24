#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>
#include <QDir>
#include <QListWidgetItem>
#include <QMessageBox>
#include "mediacontroller.h"
#include "database.h"
#include "playlist.h"

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
    void onPreviousButtonPressed();
    void updateProgressBar();
    void onSliderMoved();

private:
    Ui::MainWindow *ui;
    Playlist playList;
    MediaController mediaController;
    QTimer *progressTimer;
    int SliderPosition;
    DataBase db;

    void loadPlaylists();
    void loadPlaylist(const QString &name);
};

#endif // MAINWINDOW_H
