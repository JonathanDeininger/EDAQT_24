#include "mainwindow.h"
#include "Playlist.h"
#include "mediacontroller.h"
#include "./ui_mainwindow.h"
#include "installerdialog.h" // Include InstallerDialog header
#include <QInputDialog> // Include QInputDialog header

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // Pass the QListWidget and Playlist to the MediaController
    , db() // Initialize the database
{
    mediaController = new MediaController(ui->Playlist, playList);
    ui->setupUi(this);
    progressTimer = new QTimer(this);
    connect(ui->Playlist, &QListWidget::itemClicked, this, &MainWindow::onPlaylistItemClicked);
    // Fix incorrect member name
    connect(ui->RemovePlaylistButton, &QPushButton::clicked, this, &MainWindow::onChoosePlaylistButtonClicked);
    connect(ui->Play, &QPushButton::clicked, this, &MainWindow::onPlayButtonPressed);
    connect(ui->LautstaerkeRegler, &QSlider::valueChanged, this, &MainWindow::onVolumeChanged);
    connect(ui->Pause, &QPushButton::clicked, this, &MainWindow::onPauseButtonPressed);
    connect(ui->previousPushButton, &QPushButton::clicked, this, &MainWindow::onPreviousButtonPressed);
    connect(ui->nextPushButton, &QPushButton::clicked, this, &MainWindow::onNextButtonPressed);
    connect(ui->Fortschrittslider, &QSlider::sliderReleased, this, &MainWindow::onSliderReleased);
    connect(ui->Fortschrittslider, &QSlider::sliderPressed, this, &MainWindow::onSliderPressed);
    connect(mediaController, &MediaController::positionChanged,this, &MainWindow::updateProgressBar);
    connect(mediaController, &MediaController::currentTrackChanged, this, &MainWindow::updateCurrentTrackInfo);
    connect(ui->searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    connect(ui->Random,&QPushButton::clicked, this, &MainWindow::onRandomButtonPressed);
    connect(ui->Repeat,&QPushButton::clicked, this, &MainWindow::onRepeatButtonPressed);
    connect(ui->AddPlaylistButton, &QPushButton::clicked, this, &MainWindow::onAddPlaylistButtonClicked);
    connect(ui->RemovePlaylistButton, &QPushButton::clicked, this, &MainWindow::onRemovePlaylistButtonClicked);
    connect(ui->AddTrackButton, &QPushButton::clicked, this, &MainWindow::onAddTrackButtonClicked);
    connect(ui->RemoveTrackButton, &QPushButton::clicked, this, &MainWindow::onRemoveTrackButtonClicked);
    connect(ui->PlaylistSammlung, &QListWidget::itemClicked, this, &MainWindow::onPlaylistSammlungItemClicked);

    // Enable drag and drop
    ui->Playlist->setDragEnabled(true);
    ui->Playlist->setAcceptDrops(false);
    ui->Playlist->setDropIndicatorShown(true);
    ui->PlaylistSammlung->setAcceptDrops(true);
    ui->PlaylistSammlung->setDropIndicatorShown(true);

    // Show InstallerDialog if the database is empty
    InstallerDialog installerDialog;
    if (installerDialog.isDatabaseEmpty()) {
        installerDialog.exec();
    }

    // Load tracks from the database
    loadTracksFromDatabase();
    // Load playlists from the database
    loadPlaylistsFromDatabase();
    // Load the "Alle Songs" playlist
    loadPlaylist("Alle Songs");
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
    // qDebug() << "SampleCount von aktuellen Track: " << currentTrack.getSampleCount();
    // qDebug() << "Position changed saertzh:" << currentSongPosition;

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

void MainWindow::setProgressBarAndSongDurationLabel() {
    Track currentTrack = mediaController->getCurrentTrack();
    qDebug() << "FilePath von aktuellen Track: " << currentTrack.getFilePath();
    qDebug() << "Duration von aktuellen Track: " << currentTrack.getDuration();
    ui->Fortschrittslider->setRange(0, currentTrack.getDuration());
    setCurrentSongDuration(currentTrack);
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

void MainWindow::randomizePlaylist()
{
    // Hole alle Items aus der Playlist

    QList<QListWidgetItem*> items;
    for (int i = 0; i < ui->Playlist->count(); ++i) {
        items.append(ui->Playlist->takeItem(i));
    }
    // items.append(mediaController->getCurrentPlaylist());
    // Zufällig mischen
    std::random_shuffle(items.begin(), items.end());

    // Füge die gemischten Items wieder hinzu
    for (QListWidgetItem* item : items) {
        ui->Playlist->addItem(item);
    }

    // Synchronisiere die gemischte Reihenfolge mit der internen Playlist
    QVector<QString> randomizedFilePaths;
    for (int i = 0; i < ui->Playlist->count(); ++i) {
        randomizedFilePaths.append(ui->Playlist->item(i)->data(Qt::UserRole).toString());
    }
    // mediaController->setPlaylist(randomizedFilePaths);
}

void MainWindow::onRandomButtonPressed()
{
    // Wechsel des Shuffle-Status
    isShuffleActive = !isShuffleActive;

    // Ändere die Farbe des Buttons basierend auf dem Status
    if (isShuffleActive) {
        // Shuffle aktiv: Button grün färben
        ui->Random->setStyleSheet("background-color: green; color: white;");
        qDebug() << "Shuffle mode activated.";

        // Randomisiere die Playlist
        randomizePlaylist();

        // Starte mit der zufälligen Wiedergabe
        mediaController->setCurrentIndex(0); // Erster Track in der zufälligen Reihenfolge
        mediaController->playCurrent();
    } else {
        // Shuffle deaktiviert: Button rot färben
        ui->Random->setStyleSheet("background-color: red; color: white;");
        qDebug() << "Shuffle mode deactivated.";
    }
}

void MainWindow::onRepeatButtonPressed()
{
    return;
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    for (int i = 0; i < ui->Playlist->count(); ++i) {
        QListWidgetItem *item = ui->Playlist->item(i);
        bool match = item->text().contains(text, Qt::CaseInsensitive);
        item->setHidden(!match);
    }
}

void MainWindow::onAddPlaylistButtonClicked() {
    // Prompt the user to enter a name for the new playlist
    QString playlistName = QInputDialog::getText(this, tr("Add Playlist"), tr("Playlist Name:"));
    if (!playlistName.isEmpty()) {
        if (db.insertPlaylist(playlistName)) {
            loadPlaylistsFromDatabase();
            // Reload the list of playlists from the database
            loadPlaylistsFromDatabase();
        } else {
            // Show an error message if the playlist could not be added
            QMessageBox::warning(this, tr("Error"), tr("Failed to add playlist."));
        }
    }
}

void MainWindow::onRemovePlaylistButtonClicked() {
    // Implement the logic to remove the selected playlist
    QListWidgetItem *item = ui->PlaylistSammlung->currentItem();
    if (item) {
        QString playlistName = item->text();
        // Implement the logic to remove the playlist from the database
        // ...
        loadPlaylistsFromDatabase();
    }
}

void MainWindow::onAddTrackButtonClicked() {
    // Öffnet einen Dateidialog, um eine Audiodatei auszuwählen
    QString filePath = QFileDialog::getOpenFileName(this, tr("Wähle eine Audiodatei"), QDir::homePath(), tr("Audio Files (*.mp3 *.wav *.flac *.aac)"));
    if (filePath.isEmpty()) {
        return;
    }

    // Füge die ausgewählte Datei zur aktuellen Playlist hinzu
    QListWidgetItem *item = new QListWidgetItem(QFileInfo(filePath).fileName());
    item->setData(Qt::UserRole, filePath);
    ui->Playlist->addItem(item);

    // Erstelle ein Track-Objekt und füge es zur Playlist hinzu
    Track track;
    track.setFilePath(filePath);
    playList.addTrack(track);

    // Optional: Füge den Track auch zur Datenbank hinzu
    // db.insertData(filePath, track.getArtist(), track.getAlbum(), track.getTitle(), track.getDuration(), track.getSampleRate(), track.getSampleCount(), track.getHash());

    qDebug() << "Track added to playlist:" << filePath;
}

void MainWindow::onRemoveTrackButtonClicked() {
    // Implement the logic to remove the selected track from the playlist
    // ...
}

void MainWindow::onPlaylistSammlungItemClicked(QListWidgetItem *item) {
    // Load the tracks of the selected playlist
    QString playlistName = item->text();
    loadPlaylist(playlistName);

    // Debug: Ausgabe der geladenen Tracks
    qDebug() << "Playlist clicked:" << playlistName;
    for (int i = 0; i < ui->Playlist->count(); ++i) {
        QListWidgetItem *trackItem = ui->Playlist->item(i);
        qDebug() << "Track" << i << ":" << trackItem->text() << "Path:" << trackItem->data(Qt::UserRole).toString();
    }
}

void MainWindow::loadPlaylistsFromDatabase() {
    ui->PlaylistSammlung->clear();
    std::vector<QString> playlists = db.getAllPlaylists();
    for (const auto &playlistName : playlists) {
        ui->PlaylistSammlung->addItem(new QListWidgetItem(playlistName));
    }
}

void MainWindow::loadPlaylist(const QString &playlistName) {
    Playlist playlist = db.getPlaylist(playlistName);
    ui->Playlist->clear();
    playList.setFiles(playlist.getFiles()); // Update the playList member
    qDebug() << "Loading playlist:" << playlistName;
    qDebug() << "Number of tracks in playlist:" << playlist.getFiles().size();
    for (const auto &filePath : playlist.getFiles()) {
        qDebug() << "Adding track to playlist:" << filePath;
        QListWidgetItem *trackItem = new QListWidgetItem(QFileInfo(filePath).fileName());
        trackItem->setData(Qt::UserRole, filePath);
        ui->Playlist->addItem(trackItem);
    }
    mediaController->setPlaylist(playList); // Ensure MediaController gets the updated playlist
    qDebug() << "Playlist set in MediaController with" << playList.getFiles().size() << "tracks.";

    // Debug: Ausgabe des ersten geladenen Tracks
    if (!playlist.getTracks().empty()) {
        Track firstTrack = playlist.getTracks().front();
        qDebug() << "First track details:";
        qDebug() << "File path:" << firstTrack.getFilePath();
        qDebug() << "Artist:" << firstTrack.getArtist();
        qDebug() << "Album:" << firstTrack.getAlbum();
        qDebug() << "Title:" << firstTrack.getTitle();
        qDebug() << "Duration:" << firstTrack.getDuration();
        qDebug() << "Sample Rate:" << firstTrack.getSampleRate();
        qDebug() << "Sample Count:" << firstTrack.getSampleCount();
        qDebug() << "Hash:" << firstTrack.getHash();
        qDebug() << "Track ID:" << firstTrack.getTrackID();
    }
}
