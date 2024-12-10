#include "mainwindow.h"
#include "Playlist.h"
#include "mediacontroller.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // Pass the QListWidget and Playlist to the MediaController
    , db() // Initialize the database
{
    mediaController = new MediaController(ui->Playlist, playList);
    ui->setupUi(this);
    progressTimer = new QTimer(this);
    connect(ui->Playlist, &QListWidget::itemClicked, this, &MainWindow::onPlaylistItemClicked);
    connect(ui->ChoosePlaylistButton, &QPushButton::clicked, this, &MainWindow::onChoosePlaylistButtonClicked);
    connect(ui->Play, &QPushButton::clicked, this, &MainWindow::onPlayButtonPressed);
    connect(ui->LautstaerkeRegler, &QSlider::valueChanged, this, &MainWindow::onVolumeChanged);
    connect(ui->Pause, &QPushButton::clicked, this, &MainWindow::onPauseButtonPressed);
    connect(ui->previousPushButton, &QPushButton::clicked, this, &MainWindow::onPreviousButtonPressed);
    connect(ui->nextPushButton, &QPushButton::clicked, this, &MainWindow::onNextButtonPressed);
    connect(ui->Fortschrittslider, &QSlider::sliderReleased, this, &MainWindow::onSliderReleased);
    connect(ui->Fortschrittslider, &QSlider::sliderPressed, this, &MainWindow::onSliderPressed);
    connect(mediaController, &MediaController::positionChanged,this, &MainWindow::updateProgressBar);
    connect(mediaController, &MediaController::currentTrackChanged, this, &MainWindow::updateCurrentTrackInfo);
    // Load tracks from the database
    loadTracksFromDatabase();
}

MainWindow::~MainWindow() {
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
    mediaController->initializePlayer();

}

void MainWindow::onPlaylistItemClicked(QListWidgetItem *item) {
    ui->currentSongLabel->setText(item->text());
    int index = ui->Playlist->row(item);
    mediaController->setCurrentIndex(index);
    mediaController->playPlaylist();
    setProgressBarAndSongDurationLabel();
}

void MainWindow::setCurrentSongDuration(Track currentTrack)
{
    int totalSeconds = currentTrack.getDuration();
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
void MainWindow::onPlayButtonPressed() {
    mediaController->playCurrent();
}

void MainWindow::onVolumeChanged(float value) {
    mediaController->getAudioOutput()->setVolume(value / 100);
}

void MainWindow::onPauseButtonPressed() {
    mediaController->pauseCurrent();
}

void MainWindow::onPreviousButtonPressed() {
    // Hole den aktuellen Index des Songs
    mediaController->prev();
    Track currentTrack = mediaController->getCurrentTrack();

    QString currentTitle = currentTrack.getTitle();
    // Setze den Titel des vorherigen Songs als currentSongLabel
    ui->currentSongLabel->setText(currentTitle);
    ui->Playlist->setCurrentRow(mediaController->getCurrentIndex());
    setProgressBarAndSongDurationLabel();
}

void MainWindow::onNextButtonPressed() {
    // Hole den aktuellen Index des Songs
    mediaController->next();
    Track currentTrack = mediaController->getCurrentTrack();

    QString currentTitle = currentTrack.getTitle();
    // Setze den Titel des vorherigen Songs als currentSongLabel
    ui->currentSongLabel->setText(currentTitle);
    ui->Playlist->setCurrentRow(mediaController->getCurrentIndex());
    setProgressBarAndSongDurationLabel();
}
void MainWindow::updateCurrentTrackInfo(int index, const QString &title) {
    // Setze den aktuellen Songtitel
    ui->currentSongLabel->setText(title);

    // Markiere den aktuellen Song in der Playlist
    ui->Playlist->setCurrentRow(index);

    // Aktualisiere die Fortschrittsleiste und die Songdaueranzeige
    setProgressBarAndSongDurationLabel();
}

void MainWindow::updateProgressBar(qint64 currentSongPosition) {
    // Überprüft, ob der Benutzer den Slider gerade bewegt
    if (sliderBeingDragged) {
        return;  // Funktion verlassen, wenn der Slider gerade bewegt wird
    }

    Track currentTrack = mediaController->getCurrentTrack();
    qDebug() << "SampleCount von aktuellen Track: " << currentTrack.getSampleCount();
    qDebug() << "Position changed saertzh:" << currentSongPosition;

    // Aktualisiert den Slider nur, wenn der Benutzer nicht mit ihm interagiert
    ui->Fortschrittslider->setValue(currentSongPosition);
}
void MainWindow::onSliderPressed() {
    // Setzt das Flag, wenn der Benutzer den Slider drückt
    sliderBeingDragged = true;
}

void MainWindow::onSliderReleased() {
    // Setzt das Flag zurück, wenn der Benutzer den Slider loslässt
    sliderBeingDragged = false;

    // Aktualisiert die Position des Songs basierend auf dem Sliderwert
    qint64 currentPosition = static_cast<qint64>(ui->Fortschrittslider->value());
    qDebug() <<"neuer Wert des Sliders: "<< currentPosition;
    mediaController->setCurrentSongPosition(currentPosition);
    ui->Fortschrittslider->setValue(currentPosition);
}
void MainWindow::setProgressBarAndSongDurationLabel()
{
    Track currentTrack = mediaController->getCurrentTrack();
    qDebug() << "FilePath von aktuellen Track: "<< currentTrack.getFilePath();
    qDebug() << "SampleCount von aktuellen Track: "<<currentTrack.getDuration();
    ui->Fortschrittslider->setRange(0, currentTrack.getDuration());
    setCurrentSongDuration(currentTrack);
    // ui->Fortschrittslider->setMinimum(0);
    // ui->Fortschrittslider->setMaximum(currentTrack.getSampleCount()*1000);
}

void MainWindow::loadTracksFromDatabase() {
    if (!db.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }

    QSqlQuery query(db.getDatabase());
    query.exec("SELECT FilePath, Interpret, Titel, Album, Spielzeit, SampleRate, SampleCount, Hash FROM Mediathek");

    while (query.next()) {
        QString filePath = query.value(0).toString();
        QString interpret = query.value(1).toString();
        QString titel = query.value(2).toString();
        QString album = query.value(3).toString();
        int spielzeit = query.value(4).toInt();
        int sampleRate = query.value(5).toInt();
        int sampleCount = query.value(6).toInt();
        QByteArray hash = query.value(7).toByteArray();

        qDebug() << "Loaded track from database:";
        qDebug() << "File path:" << filePath;
        qDebug() << "Artist:" << interpret;
        qDebug() << "Album:" << album;
        qDebug() << "Title:" << titel;
        qDebug() << "Duration:" << spielzeit;
        qDebug() << "Sample Rate:" << sampleRate;
        qDebug() << "Sample Count:" << sampleCount;
        qDebug() << "Hash:" << hash;

        // Formatieren des Anzeigetexts für die Liste
        QString displayText = interpret + " | " + titel;
        if (!album.isEmpty()) {
            displayText += " | " + album;
        }

        // Erstelle das QListWidgetItem
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, filePath);  // Speichert den vollständigen Pfad als User-Daten
        ui->Playlist->addItem(item);
        playList.addFile(filePath);
    }

    db.close();
}


