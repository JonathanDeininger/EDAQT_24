#include "mainwindow.h"
#include "Playlist.h"
#include "mediacontroller.h"
#include "./ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mediaController(ui->Playlist, playList) // Pass the QListWidget and Playlist to the MediaController
    , db() // Initialize the database
{
    ui->setupUi(this);
    progressTimer = new QTimer(this);
    connect(ui->Playlist, &QListWidget::itemClicked, this, &MainWindow::onPlaylistItemClicked);
    connect(ui->ChoosePlaylistButton, &QPushButton::clicked, this, &MainWindow::onChoosePlaylistButtonClicked);
    connect(ui->Play, &QPushButton::clicked, this, &MainWindow::onPlayButtonPressed);
    connect(ui->LautstaerkeRegler, &QSlider::valueChanged, this, &MainWindow::onVolumeChanged);
    connect(ui->Pause, &QPushButton::clicked, this, &MainWindow::onPauseButtonPressed);
    connect(ui->previousPushButton, &QPushButton::clicked, this, &MainWindow::onPreviousButtonPressed);
    connect(ui->nextPushButton, &QPushButton::clicked, this, &MainWindow::onNextButtonPressed);
    connect(ui->Fortschrittslider, &QSlider::sliderMoved, this, &MainWindow::onSliderMoved);
    connect(progressTimer, &QTimer::timeout, this, &MainWindow::updateProgressBar);

    // Ensure the database is opened
    if (!db.open()) {
        qDebug() << "Failed to open the database in MainWindow constructor.";
    }

    // Load playlists into the PlaylistSammlung
    loadPlaylists();
}

MainWindow::~MainWindow() {
    db.close();
    delete ui;
}

void MainWindow::onChoosePlaylistButtonClicked() {
    // Öffnet Windows-Explorer um den Ordner mit den Songs zu finden wird keiner ausgewählt verlässt er die Funktion
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Wähle einen Ordner"), QDir::homePath());
    if (folderPath.isEmpty()) {
        return;
    }
    QDir directory(folderPath);
    // Sucht alle unterstützten Audiodateien raus, wenn es keine gibt verlässt er die Funktion
    QStringList audioFiles = directory.entryList(QStringList() << "*.mp3" << "*.wav" << "*.flac" << "*.aac", QDir::Files);
    if (audioFiles.isEmpty()) {
        QMessageBox::information(this, tr("Keine Dateien gefunden"), tr("Der ausgewählte Ordner enthält keine unterstützten Audiodateien."));
        return;
    }

    // Löschen der Inhalte der Playlist, falls es bereits Einträge hat
    ui->Playlist->clear();

    // Füllen der Playlist mit den Audiodateien
    foreach (const QString &fileName, audioFiles) {
        QString fullPath = directory.absoluteFilePath(fileName);
        QListWidgetItem *item = new QListWidgetItem(fileName);
        item->setData(Qt::UserRole, fullPath);  // Speichert den vollständigen Pfad
        ui->Playlist->addItem(item);
        playList.addFile(fullPath);
    }
    mediaController.initializePlayer();
}

void MainWindow::onPlaylistItemClicked(QListWidgetItem *item) {
    ui->currentSongLabel->setText(item->text());
    int index = ui->Playlist->row(item);
    mediaController.setCurrentIndex(index);
}

void MainWindow::onPlayButtonPressed() {
    mediaController.playCurrent();
    int totalSeconds = mediaController.getCurrentSongDuration(); // /1000 um Sekunden heraus zu bekommen
    ui->Fortschrittslider->setRange(0, totalSeconds);
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    // Anzeige im Format hh:mm:ss
    ui->currentSongDurationLabel->setText(
        QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'))
    );
}

void MainWindow::onVolumeChanged(float value) {
    mediaController.getAudioOutput()->setVolume(value / 100);
}

void MainWindow::onPauseButtonPressed() {
    mediaController.pauseCurrent();
}

void MainWindow::onPreviousButtonPressed() {
    mediaController.prev();
}

void MainWindow::onNextButtonPressed() {
    mediaController.next();
}

void MainWindow::updateProgressBar() {
    int currentPosition = mediaController.getCurrentSongPosition();
    if (currentPosition < mediaController.getCurrentSongDuration()) {
        mediaController.setCurrentSongPosition(currentPosition++); // Simuliert das Fortschreiten der Wiedergabe
        ui->Fortschrittslider->setValue(currentPosition);
    } else {
        progressTimer->stop(); // Stoppt den Timer, wenn das Lied zu Ende ist
    }
}

void MainWindow::onSliderMoved() {
    int currentPosition = mediaController.getCurrentSongPosition();
    mediaController.setCurrentSongPosition(currentPosition); // Setzt die aktuelle Position auf die gewählte
    ui->Fortschrittslider->setValue(currentPosition);
}

void MainWindow::loadPlaylists() {
    // Clear the PlaylistSammlung
    ui->PlaylistSammlung->clear();

    // Add the special playlist "alle Songs"
    QListWidgetItem *allSongsItem = new QListWidgetItem("alle Songs");
    ui->PlaylistSammlung->addItem(allSongsItem);

    // Retrieve all playlists from the database
    std::vector<QString> playlists = db.getAllPlaylists();
    for (const auto &playlistName : playlists) {
        QListWidgetItem *item = new QListWidgetItem(playlistName);
        ui->PlaylistSammlung->addItem(item);
    }
}

void MainWindow::loadPlaylist(const QString &name) {
    Playlist playlist = db.getPlaylist(name);

    ui->Playlist->clear(); // Use the correct QListWidget
    for (const auto &track : playlist.getTracks()) {
        ui->Playlist->addItem(track.getTitle()); // Use the correct QListWidget
    }
}
