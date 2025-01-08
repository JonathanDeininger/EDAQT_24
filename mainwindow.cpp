#include "mainwindow.h"
#include "playlist.h"
#include "mediacontroller.h"
#include "./ui_mainwindow.h"
#include "installerdialog.h" // Include InstallerDialog header
#include "selecttrackdialog.h" // Include the SelectTrackDialog header
#include <QInputDialog> // Include QInputDialog header
#include <qdebug.h>
#include <qlogging.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // Pass the QListWidget and Playlist to the MediaController
    , model(new QStandardItemModel(this))
    , db() // Initialize the database
{
    mediaController = new MediaController(ui->Playlist, playList);
    ui->setupUi(this);
    setupSongTable();
    progressTimer = new QTimer(this);
    connect(ui->Playlist, &QListWidget::itemClicked, this, &MainWindow::onPlaylistItemClicked);
    // Fix incorrect member name
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
    connect(ui->AddTrackButton, &QPushButton::clicked, this, &MainWindow::onAddTrackButtonClicked);
    connect(ui->PlaylistSammlung, &QListWidget::itemClicked, this, &MainWindow::onPlaylistSammlungItemClicked);
    connect(ui->songTable,&QTableView::doubleClicked, this, &MainWindow::onSongTableItemClicked);

    // Show InstallerDialog if the database is empty
    InstallerDialog installerDialog;
    if (installerDialog.isDatabaseEmpty()) {
        installerDialog.exec();
    }

    // Load playlists from the database
    loadPlaylistsFromDatabase();

    // Load the "Alle Songs" playlist
    loadPlaylist("Alle Songs");
    loadPlaylistInTable("Alle Songs");

    ui->LautstaerkeRegler->setSliderPosition(50);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::setupSongTable(){

    model->setHorizontalHeaderLabels({"#", "Titel", "Artist", "Duration"});

    // Modell an TableView binden
    ui->songTable->setModel(model);

    // Hover- und Auswahl-Stil
    ui->songTable->setStyleSheet(
        "QTableView::item:hover {"
        "    background-color: none;"
        "}"
        "QTableView::item:selected {"
        "    background-color: lightgrey;"
        "    color: black;"
        "}"
        "QTableView::row:hover {"
        "    background-color: lightblue;"
        "}"
        );
    // Einstellungen für die Zeilenauswahl
    ui->songTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songTable->horizontalHeader()->setHighlightSections(false); // Header nicht markieren
    ui->songTable->setSelectionBehavior(QTableView::SelectRows); // Ganze Zeile auswählen
    ui->songTable->setSelectionMode(QTableView::SingleSelection); // Nur eine Zeile auswählbar
    // ui->songTable->
    // Gitter entfernen
    ui->songTable->setShowGrid(false);


    // Header formatieren
    QHeaderView *header = ui->songTable->horizontalHeader();
    ui->songTable->verticalHeader()->setVisible(false); // Vertikalen Header ausblenden


}


//kann das weg??

void MainWindow::onPlaylistItemClicked(QListWidgetItem *item) {
    // ui->currentSongLabel->setText(item->text());
    // int index = ui->Playlist->row(item);
    // mediaController->setCurrentIndex(index);
    // mediaController->playPlaylist();
    // setProgressBarAndSongDurationLabel();
}
void MainWindow::onSongTableItemClicked(const QModelIndex &index){ //für später

    // Aktuelle Zeile ermitteln
    int row = index.row();

    // Song-Details aus der Tabelle abrufen
    QString songTitle = model->item(row, 1)->text(); // Spalte 1: Titel
    QString songFilePath = model->item(row, 0)->data(Qt::UserRole).toString(); // Dateipfad aus UserRole

    // Aktuelles Lied setzen
    ui->currentSongLabel->setText(songTitle);
    mediaController->setCurrentIndex(row);
    mediaController->playPlaylist();

    // Fortschrittsbalken und Song-Dauer setzen
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
    mediaController->prev();
    updateCurrentTrackDisplay();
}

void MainWindow::onNextButtonPressed() {
    mediaController->next();
    updateCurrentTrackDisplay();
}

void MainWindow::updateCurrentTrackInfo(int index) {


    Track currentTrack = mediaController->getCurrentTrack();
    // Markiere die entsprechende Zeile in der TableView
    QModelIndex modelIndex = model->index(index, 0); // Zeile: index, Spalte: 0 (oder eine andere Spalte, falls nötig)
    ui->songTable->setCurrentIndex(modelIndex);      // Markiere die Zeile
    ui->songTable->scrollTo(modelIndex);            // Scrolle zu der Zeile, falls sie nicht sichtbar ist

    // Setze den aktuellen Songtitel im Label
    ui->currentSongLabel->setText(QString("%1 | %2").arg(currentTrack.getTitle(),currentTrack.getArtist()));
    // Aktualisiere die Fortschrittsleiste und die Songdaueranzeige
    setProgressBarAndSongDurationLabel();

}

void MainWindow::updateProgressBar(qint64 currentSongPosition) {
    // Überprüft, ob der Benutzer den Slider gerade bewegt
    if (sliderBeingDragged) {
        return;  // Funktion verlassen, wenn der Slider gerade bewegt wird
    }

    Track currentTrack = mediaController->getCurrentTrack();
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
    mediaController->setCurrentSongPosition(currentPosition);
    ui->Fortschrittslider->setValue(currentPosition);
}

void MainWindow::setProgressBarAndSongDurationLabel() {
    Track currentTrack = mediaController->getCurrentTrack();
    ui->Fortschrittslider->setRange(0, currentTrack.getDuration());
    setCurrentSongDuration(currentTrack);
}


void MainWindow::onRandomButtonPressed() {
    // Wechsel des Shuffle-Status
    isShuffleActive = !isShuffleActive;

    // Ändere die Farbe des Buttons basierend auf dem Status
    if (isShuffleActive) {
        // Shuffle aktiv: Button grün färben
        ui->Random->setStyleSheet("background-color: green; color: white;");
        qDebug() << "Shuffle mode activated.";

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

// Diese Methode wird aufgerufen, wenn der "Playlist hinzufügen"-Button geklickt wird und fügt eine neue Playlist hinzu
// zur Datenbank und lädt die aktualisierte PlaylistSammlung

void MainWindow::onAddPlaylistButtonClicked() {
    // Prompt the user to enter a name for the new playlist
    QString playlistName = QInputDialog::getText(this, tr("Add Playlist"), tr("Playlist Name:"));
    if (!playlistName.isEmpty()) {
        if (db.insertPlaylist(playlistName)) {
            loadPlaylistsFromDatabase();
        } else {
            // Show an error message if the playlist could not be added
            QMessageBox::warning(this, tr("Error"), tr("Failed to add playlist."));
        }
    }
}
// Diese Methode wird aufgerufen, wenn der "Track hinzufügen"-Button geklickt wird und fügt
// einen Track zur aktuellen Playlist hinzu
void MainWindow::onAddTrackButtonClicked() {
    // Lade die "Alle Songs"-Playlist
    Playlist allSongsPlaylist = db.getPlaylist("Alle Songs");

    // Zeige den Dialog mit den Tracks aus der "Alle Songs"-Playlist
    SelectTrackDialog dialog(allSongsPlaylist.getTracks(), this);
    if (dialog.exec() == QDialog::Accepted) {
        Track track = dialog.getSelectedTrack();
        if (!track.getFilePath().isEmpty()) {
            // Füge den Track zur Datenbank hinzu
            QSqlQuery query(db.getDatabase());
            query.prepare("SELECT PlaylistID FROM Playlists WHERE Name = :name");
            query.bindValue(":name", playList.getName());
            if (query.exec() && query.next()) {
                int playlistID = query.value(0).toInt();
                int trackID = track.getTrackID();
                if (trackID != -1 && playlistID != -1) {
                    db.insertPlaylistTrack(playlistID, trackID);
                }
            }

            // Aktualisiere die UI, um den neuen Track anzuzeigen
            loadPlaylist(playList.getName());
        }
    }
}
// Diese Methode wird aufgerufen, wenn ein Element in der PlaylistSammlung geklickt wird und
// lädt die ausgewählte Playlist
void MainWindow::onPlaylistSammlungItemClicked(QListWidgetItem *item) {
    QString playlistName = item->text();
    loadPlaylist(playlistName);
    loadPlaylistInTable(playlistName);
}

// Diese Methode lädt alle Playlists aus der Datenbank und fügt sie zur PlaylistSammlung hinzu (QListWidget)
void MainWindow::loadPlaylistsFromDatabase() {
    ui->PlaylistSammlung->clear();
    std::vector<QString> playlists = db.getAllPlaylists();
    for (const auto &playlistName : playlists) {
        ui->PlaylistSammlung->addItem(new QListWidgetItem(playlistName));
    }
}

// Diese Methode lädt die Playlist mit dem angegebenen Namen aus der Datenbank
// und fügt die Tracks zur Playlist hinzu und zeigt sie in der QListWidget an

void MainWindow::loadPlaylist(const QString &playlistName) {
    Playlist playlist = db.getPlaylist(playlistName);
    playList.setName(playlistName);
    playList.setTracks(playlist.getTracks());
    ui->Playlist->clear();
    for (const auto &track : playlist.getTracks()) {
        QListWidgetItem *item = new QListWidgetItem(track.getTitle(), ui->Playlist);
        item->setData(Qt::UserRole, track.getFilePath());
    }
    db.close();
}

void MainWindow::loadPlaylistInTable(const QString &playlistName) {
    // Playlist aus der Datenbank laden
    Playlist playlist = db.getPlaylist(playlistName);
    playList.setName(playlistName);
    playList.setTracks(playlist.getTracks());

    // Datenmodell für die TableView vorbereiten
    model->clear(); // Vorherige Daten löschen

    // Header-Titel setzen
    model->setHorizontalHeaderLabels({"#" ,"Title", "Artist", "Duration"});

    // Daten aus der Playlist ins Modell einfügen
    int row = 0;
    for (const auto &track : playlist.getTracks()) {
        QStandardItem *indexItem = new QStandardItem(QString::number(row + 1));
        QStandardItem *titleItem = new QStandardItem(track.getTitle());
        QStandardItem *artistItem = new QStandardItem(track.getArtist());
        QStandardItem *durationItem = new QStandardItem(track.getSongDurationAsString());
        // Index zentrieren
        indexItem->setTextAlignment(Qt::AlignCenter);
        durationItem->setTextAlignment(Qt::AlignCenter);
        // Daten ins Modell einfügen
        model->setItem(row, 0, indexItem);
        model->setItem(row, 1, titleItem);
        model->setItem(row, 2, artistItem);
        model->setItem(row, 3, durationItem);

        // Optional: Dateipfad als Benutzerdaten speichern
        indexItem->setData(track.getFilePath(), Qt::UserRole);

        ++row;
    }

    // Modell an die TableView binden
    ui->songTable->setModel(model);
    // Spaltenbreiten festsetzen
    ui->songTable->setColumnWidth(0, 50);  // Spalte 0: Index, 50 Pixel
    ui->songTable->setColumnWidth(1, 200); // Spalte 1: Titel, 200 Pixel
    ui->songTable->setColumnWidth(2, 150); // Spalte 2: Interpret, 150 Pixel
    ui->songTable->setColumnWidth(3, 80);  // Spalte 3: Dauer, 80 Pixel
    // Debug-Ausgabe
    qDebug() << "Playlist:" << playList.getName();

    // Datenbank schließen
    db.close();
}
// Diese Methode aktualisiert die Anzeige des aktuellen Tracks in der UI

void MainWindow::updateCurrentTrackDisplay() {
    // Track currentTrack = mediaController->getCurrentTrack();
    // QString currentTitle = currentTrack.getTitle();
    // ui->currentSongLabel->setText(currentTitle);
    // ui->Playlist->setCurrentRow(mediaController->getCurrentIndex());
    // setProgressBarAndSongDurationLabel();

    // Aktuellen Track vom MediaController abrufen
    Track currentTrack = mediaController->getCurrentTrack();
    QString currentTitle = currentTrack.getTitle();
    int currentIndex = mediaController->getCurrentIndex();

    // Songtitel im Label anzeigen
    ui->currentSongLabel->setText(currentTitle);

    // Zeile in der TableView auswählen
    QModelIndex index = model->index(currentIndex, 0); // Zeile: currentIndex, Spalte: 0
    ui->songTable->setCurrentIndex(index);
    ui->songTable->scrollTo(index); // Automatisch zu der Zeile scrollen

    // Fortschrittsbalken und Song-Dauer aktualisieren
    setProgressBarAndSongDurationLabel();

}
