#include "mainwindow.h"
#include <QInputDialog> // Include QInputDialog header
#include "./ui_mainwindow.h"
#include "installerdialog.h" // Include InstallerDialog header
#include "mediacontroller.h"
#include "playlist.h"
#include "selecttrackdialog.h" // Include the SelectTrackDialog header
#include <qdebug.h>
#include <qlogging.h>
#include <vector>

/**
 * Konstruktor der MainWindow-Klasse.
 * Initialisiert die Benutzeroberfläche, lädt die Playlists und verbindet UI-Elemente mit Slots.
 * @param parent Der Eltern-Widget, falls vorhanden.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new QStandardItemModel(this))
    , db() // Initialisiert die Datenbank
{
    mediaController = new MediaController(ui->Playlist, playList);
    ui->setupUi(this);
    setupSongTable(); // Konfiguriert die Song-Tabelle

    progressTimer = new QTimer(this);

    // Verbindet die UI-Elemente mit den entsprechenden Slots
    connect(ui->Play, &QPushButton::clicked, this, &MainWindow::onPlayButtonPressed);
    connect(ui->LautstaerkeRegler, &QSlider::valueChanged, this, &MainWindow::onVolumeChanged);
    connect(ui->Pause, &QPushButton::clicked, this, &MainWindow::onPauseButtonPressed);
    connect(ui->previousPushButton, &QPushButton::clicked, this, &MainWindow::onPreviousButtonPressed);
    connect(ui->nextPushButton, &QPushButton::clicked, this, &MainWindow::onNextButtonPressed);
    connect(ui->Fortschrittslider, &QSlider::sliderReleased, this, &MainWindow::onSliderReleased);
    connect(ui->Fortschrittslider, &QSlider::sliderPressed, this, &MainWindow::onSliderPressed);
    connect(mediaController, &MediaController::positionChanged, this, &MainWindow::updateProgressBar);
    connect(mediaController, &MediaController::currentTrackChanged, this, &MainWindow::updateCurrentTrackInfo);
    connect(ui->searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    connect(ui->Random, &QPushButton::clicked, this, &MainWindow::onRandomButtonPressed);
    connect(ui->Repeat, &QPushButton::clicked, this, &MainWindow::onRepeatButtonPressed);
    connect(ui->AddPlaylistButton, &QPushButton::clicked, this, &MainWindow::onAddPlaylistButtonClicked);
    connect(ui->AddTrackButton, &QPushButton::clicked, this, &MainWindow::onAddTrackButtonClicked);
    connect(ui->RemovePlaylist, &QPushButton::clicked, this, &MainWindow::onRemovePlaylistButtonClicked);
    connect(ui->AddFolder, &QPushButton::clicked, this, &MainWindow::onAddFolderButtonClicked);
    connect(ui->RefreshFiles, &QPushButton::clicked, this, &MainWindow::onRefreshFilesButtonClicked);
    connect(ui->RemoveTrack, &QPushButton::clicked, this, &MainWindow::onRemoveTrackButtonClicked);
    connect(ui->PlaylistSammlung, &QListWidget::itemClicked, this, &MainWindow::onPlaylistSammlungItemClicked);
    connect(ui->songTable, &QTableView::doubleClicked, this, &MainWindow::onSongTableItemClicked);

    // Überprüft, ob die Datenbank leer ist, und zeigt ggf. den Installationsdialog an
    InstallerDialog installerDialog;
    if (installerDialog.isDatabaseEmpty()) {
        installerDialog.exec();
    }

    // Lädt die Playlists aus der Datenbank
    loadPlaylistsFromDatabase();

    // Lädt die "Alle Songs"-Playlist in die Song-Tabelle
    loadPlaylistInTable("Alle Songs");

    // Setzt die Lautstärke auf 50%
    ui->LautstaerkeRegler->setSliderPosition(50);
}

/**
 * Destruktor der MainWindow-Klasse.
 * Bereinigt die Ressourcen und entfernt die Benutzeroberfläche.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Konfiguriert die Song-Tabelle.
 * Stellt sicher, dass die Tabelle korrekt dargestellt wird und das gewünschte Verhalten aufweist.
 */
void MainWindow::setupSongTable()
{
    model->setHorizontalHeaderLabels({"#", "Titel", "Artist", "Duration"});

    // Modell an TableView binden
    ui->songTable->setModel(model);

    // Hover- und Auswahl-Stil
    ui->songTable->setStyleSheet("QTableView::item:hover {"
                                 "    background-color: none;"
                                 "}"
                                 "QTableView::item:selected {"
                                 "    background-color: lightgrey;"
                                 "    color: black;"
                                 "}"
                                 "QTableView::row:hover {"
                                 "    background-color: lightblue;"
                                 "}");
    // Einstellungen für die Zeilenauswahl
    ui->songTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songTable->horizontalHeader()->setHighlightSections(false); // Header nicht markieren
    ui->songTable->setSelectionBehavior(QTableView::SelectRows);    // Ganze Zeile auswählen
    ui->songTable->setSelectionMode(QTableView::SingleSelection);   // Nur eine Zeile auswählbar

    // Gitter entfernen
    ui->songTable->setShowGrid(false);

    // Header formatieren
    QHeaderView *header = ui->songTable->horizontalHeader();
    ui->songTable->verticalHeader()->setVisible(false); // Vertikalen Header ausblenden
}

/**
 * Wird aufgerufen, wenn ein Element in der Song-Tabelle doppelt angeklickt wird.
 * Startet die Wiedergabe des ausgewählten Songs.
 * @param index Der Index des angeklickten Elements.
 */
void MainWindow::onSongTableItemClicked(const QModelIndex &index)
{
    int row = index.row(); // Aktuelle Zeile ermitteln

    QString songTitle = model->item(row, 1)->text(); // Spalte 1: Titel
    QString songFilePath = model->item(row, 0)->data(Qt::UserRole).toString(); // Dateipfad aus UserRole

    ui->currentSongLabel->setText(songTitle); // Aktuelles Lied setzen
    mediaController->setCurrentIndex(row);
    mediaController->playPlaylist(); // Wiedergabe starten

    setProgressBarAndSongDurationLabel(); // Fortschrittsbalken und Song-Dauer setzen
}

/**
 * Setzt die Anzeige der Songdauer basierend auf dem aktuellen Track.
 * @param currentTrack Der aktuell ausgewählte Track.
 */
void MainWindow::setCurrentSongDuration(Track currentTrack)
{
    int totalSeconds = currentTrack.getDuration();
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    // Anzeige im Format hh:mm:ss
    ui->currentSongDurationLabel->setText(QString("%1:%2:%3")
                                              .arg(hours, 2, 10, QChar('0'))
                                              .arg(minutes, 2, 10, QChar('0'))
                                              .arg(seconds, 2, 10, QChar('0')));
}

/**
 * Wird aufgerufen, wenn der Wiedergabe-Button gedrückt wird.
 * Startet die Wiedergabe des aktuellen Songs.
 */
void MainWindow::onPlayButtonPressed()
{
    mediaController->playCurrent();
}

/**
 * Wird aufgerufen, wenn sich die Lautstärke ändert.
 * Passt die Lautstärke der Wiedergabe an.
 * @param value Der neue Lautstärkewert (0-100).
 */
void MainWindow::onVolumeChanged(float value)
{
    mediaController->getAudioOutput()->setVolume(value / 100);
}

/**
 * Wird aufgerufen, wenn der Pause-Button gedrückt wird.
 * Pausiert die Wiedergabe des aktuellen Songs.
 */
void MainWindow::onPauseButtonPressed()
{
    mediaController->pauseCurrent();
}

/**
 * Wird aufgerufen, wenn der Zurück-Button gedrückt wird.
 * Spielt den vorherigen Song ab und aktualisiert die Anzeige.
 */
void MainWindow::onPreviousButtonPressed()
{
    mediaController->prev();
    updateCurrentTrackDisplay();
}

/**
 * Wird aufgerufen, wenn der Weiter-Button gedrückt wird.
 * Spielt den nächsten Song ab und aktualisiert die Anzeige.
 */
void MainWindow::onNextButtonPressed()
{
    mediaController->next();
    updateCurrentTrackDisplay();
}

/**
 * Aktualisiert die Anzeige mit Informationen zum aktuellen Track.
 * Markiert den aktuellen Song in der Tabelle und setzt die Fortschrittsleiste.
 */
void MainWindow::updateCurrentTrackInfo()
{
    int index = mediaController->getCurrentIndex();
    Track currentTrack = mediaController->getCurrentTrack();
    QModelIndex modelIndex = model->index(index, 0); // Zeile: index, Spalte: 0
    ui->songTable->setCurrentIndex(modelIndex);
    ui->songTable->scrollTo(modelIndex);

    ui->currentSongLabel->setText(
        QString("%1 | %2").arg(currentTrack.getTitle(), currentTrack.getArtist()));
    setProgressBarAndSongDurationLabel();
}

/**
 * Aktualisiert die Fortschrittsleiste basierend auf der aktuellen Songposition.
 * @param currentSongPosition Die aktuelle Position des Songs in Millisekunden.
 */
void MainWindow::updateProgressBar(qint64 currentSongPosition)
{
    if (sliderBeingDragged) {
        return; // Funktion verlassen, wenn der Slider gerade bewegt wird
    }

    ui->Fortschrittslider->setValue(currentSongPosition);
}

/**
 * Setzt das Flag, wenn der Fortschritts-Slider gedrückt wird.
 */
void MainWindow::onSliderPressed()
{
    sliderBeingDragged = true;
}

/**
 * Aktualisiert die Songposition, wenn der Fortschritts-Slider losgelassen wird.
 */
void MainWindow::onSliderReleased()
{
    sliderBeingDragged = false;
    qint64 currentPosition = static_cast<qint64>(ui->Fortschrittslider->value());
    mediaController->setCurrentSongPosition(currentPosition);
    ui->Fortschrittslider->setValue(currentPosition);
}

/**
 * Setzt die Fortschrittsleiste und die Songdaueranzeige basierend auf dem aktuellen Track.
 */
void MainWindow::setProgressBarAndSongDurationLabel()
{
    Track currentTrack = mediaController->getCurrentTrack();
    ui->Fortschrittslider->setRange(0, currentTrack.getDuration());
    setCurrentSongDuration(currentTrack);
}

/**
 * Wird aufgerufen, wenn der Shuffle-Button gedrückt wird.
 * Aktiviert oder deaktiviert den Shuffle-Modus und aktualisiert die Anzeige.
 */
void MainWindow::onRandomButtonPressed()
{
    mediaController->toggleShuffle();
    if (mediaController->getIsShuffleActive()) {
        ui->Random->setStyleSheet("background-color: red; color: white;");
    } else {
        ui->Random->setStyleSheet("");
    }
}

/**
 * Wird aufgerufen, wenn der Wiederholungs-Button gedrückt wird.
 * Wechselt zwischen den Wiederholungsmodi und aktualisiert die Anzeige.
 */
void MainWindow::onRepeatButtonPressed()
{
    RepeatMode mode = mediaController->getRepeatMode();

    switch (mode) {
    case RepeatMode::NoRepeat:
        mediaController->setRepeatMode(RepeatMode::RepeatPlaylist);
        ui->Repeat->setStyleSheet("background-color: green; color: white;");
        break;
    case RepeatMode::RepeatPlaylist:
        mediaController->setRepeatMode(RepeatMode::RepeatTrack);
        ui->Repeat->setStyleSheet("background-color: red; color: white;");
        break;
    case RepeatMode::RepeatTrack:
        mediaController->setRepeatMode(RepeatMode::NoRepeat);
        ui->Repeat->setStyleSheet("");
        break;
    }
}

/**
 * Wird aufgerufen, wenn der Text in der Suchleiste geändert wird.
 * Filtert die Songs in der Tabelle basierend auf dem Suchtext.
 * @param text Der eingegebene Suchtext.
 */
void MainWindow::onSearchTextChanged(const QString &text)
{
    for (int i = 0; i < model->rowCount(); ++i) {
        bool match = false;
        for (int j = 0; j < model->columnCount(); ++j) {
            QStandardItem *item = model->item(i, j);
            if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        ui->songTable->setRowHidden(i, !match);
    }
}

/**
 * Wird aufgerufen, wenn der "Playlist hinzufügen"-Button gedrückt wird.
 * Fügt eine neue Playlist zur Datenbank hinzu und lädt die aktualisierte Playlist-Liste.
 */
void MainWindow::onAddPlaylistButtonClicked()
{
    QString playlistName = QInputDialog::getText(this, tr("Add Playlist"), tr("Playlist Name:"));
    if (!playlistName.isEmpty()) {
        if (db.insertPlaylist(playlistName)) {
            loadPlaylistsFromDatabase();
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Failed to add playlist."));
        }
    }
}

/**
 * Wird aufgerufen, wenn der "Track hinzufügen"-Button gedrückt wird.
 * Fügt einen Track zur aktuellen Playlist hinzu.
 */
void MainWindow::onAddTrackButtonClicked()
{
    Playlist allSongsPlaylist = db.getPlaylist("Alle Songs");
    SelectTrackDialog dialog(allSongsPlaylist.getTracks(), this);
    if (dialog.exec() == QDialog::Accepted) {
        Track track = dialog.getSelectedTrack();
        if (!track.getFilePath().isEmpty()) {
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
            loadPlaylist(playList.getName());
            loadPlaylistInTable(playList.getName());
        }
    }
}
/**
 * Wird aufgerufen, wenn ein Element in der PlaylistSammlung geklickt wird.
 * Lädt die ausgewählte Playlist und zeigt sie in der UI an.
 * @param item Das geklickte Element in der PlaylistSammlung.
 */
void MainWindow::onPlaylistSammlungItemClicked(QListWidgetItem *item)
{
    QString playlistName = item->text();
    loadPlaylist(playlistName);
    loadPlaylistInTable(playlistName);
}

/**
 * Lädt alle Playlists aus der Datenbank und fügt sie zur PlaylistSammlung hinzu.
 */
void MainWindow::loadPlaylistsFromDatabase()
{
    ui->PlaylistSammlung->clear();
    std::vector<Playlist> playlists = db.getAllPlaylists();
    for (const auto &playlist : playlists) {
        ui->PlaylistSammlung->addItem(new QListWidgetItem(playlist.getName()));
    }
}

/**
 * Lädt die Playlist mit dem angegebenen Namen aus der Datenbank.
 * Fügt die Tracks zur Playlist hinzu und zeigt sie in der QListWidget an.
 * @param playlistName Der Name der zu ladenden Playlist.
 */
void MainWindow::loadPlaylist(const QString &playlistName)
{
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

/**
 * Lädt die Playlist mit dem angegebenen Namen in die Song-Tabelle.
 * @param playlistName Der Name der zu ladenden Playlist.
 */
void MainWindow::loadPlaylistInTable(const QString &playlistName)
{
    Playlist playlist = db.getPlaylist(playlistName);
    playList.setPlaylistID(playlist.getPlaylistID());
    playList.setName(playlistName);
    playList.setTracks(playlist.getTracks());

    model->clear(); // Vorherige Daten löschen

    model->setHorizontalHeaderLabels({"#", "Title", "Artist", "Duration"});

    int row = 0;
    for (const auto &track : playlist.getTracks()) {
        QStandardItem *indexItem = new QStandardItem(QString::number(row + 1));
        QStandardItem *titleItem = new QStandardItem(track.getTitle());
        QStandardItem *artistItem = new QStandardItem(track.getArtist());
        QStandardItem *durationItem = new QStandardItem(track.getSongDurationAsString());
        indexItem->setTextAlignment(Qt::AlignCenter);
        durationItem->setTextAlignment(Qt::AlignCenter);
        model->setItem(row, 0, indexItem);
        model->setItem(row, 1, titleItem);
        model->setItem(row, 2, artistItem);
        model->setItem(row, 3, durationItem);
        indexItem->setData(track.getFilePath(), Qt::UserRole);
        ++row;
    }

    ui->songTable->setModel(model);
    ui->songTable->setColumnWidth(0, 50);
    ui->songTable->setColumnWidth(1, 200);
    ui->songTable->setColumnWidth(2, 150);
    ui->songTable->setColumnWidth(3, 80);

    db.close();
}

/**
 * Aktualisiert die Anzeige des aktuellen Tracks in der UI.
 */
void MainWindow::updateCurrentTrackDisplay()
{
    Track currentTrack = mediaController->getCurrentTrack();
    QString currentTitle = currentTrack.getTitle();
    int currentIndex = mediaController->getCurrentIndex();

    ui->currentSongLabel->setText(currentTitle);
    QModelIndex index = model->index(currentIndex, 0);
    ui->songTable->setCurrentIndex(index);
    ui->songTable->scrollTo(index);
    setProgressBarAndSongDurationLabel();
}

/**
 * Wird aufgerufen, wenn der "Playlist entfernen"-Button geklickt wird.
 * Entfernt die aktuell ausgewählte Playlist aus der Datenbank.
 */
void MainWindow::onRemovePlaylistButtonClicked()
{
    QListWidgetItem *currentItem = ui->PlaylistSammlung->currentItem();
    if (currentItem) {
        QString playlistName = currentItem->text();
        if (db.removePlaylist(playlistName)) {
            loadPlaylistsFromDatabase();
            ui->Playlist->clear();
            model->clear();
            ui->currentSongLabel->clear();
        } else {
            QMessageBox::warning(
                this,
                tr("Error"),
                tr("Failed to remove playlist or 'Alle Songs' playlist cannot be removed."));
        }
    }
}

/**
 * Wird aufgerufen, wenn der "Ordner hinzufügen"-Button geklickt wird.
 * Fügt einen neuen Ordner zur Datenbank hinzu und lädt die "Alle Songs"-Playlist.
 */
void MainWindow::onAddFolderButtonClicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,
                                                           tr("Wähle einen Ordner"),
                                                           QDir::homePath());
    if (folderPath.isEmpty()) {
        return;
    }

    QDir directory(folderPath);
    db.processDirectory(directory);
    bool successInsertPath = db.insertPath(directory.absolutePath());
    if (successInsertPath) {
        loadPlaylistInTable("Alle Songs");
    } else {
        QMessageBox::warning(this,
                             tr("Fehler"),
                             tr("Der Pfad konnte nicht in die Datenbank geschrieben werden."));
    }
}

/**
 * Wird aufgerufen, wenn der "Dateien aktualisieren"-Button geklickt wird.
 * Überprüft die Datenbank auf verwaiste Dateien und aktualisiert die Anzeige.
 */
void MainWindow::onRefreshFilesButtonClicked()
{
    if (!db.open()) {
        QMessageBox::warning(this, tr("Fehler"), tr("Datenbank konnte nicht geöffnet werden."));
        return;
    }
    db.checkFiles();
    loadPlaylistInTable("Alle Songs");
}

/**
 * Wird aufgerufen, wenn der "Track entfernen"-Button geklickt wird.
 * Entfernt den ausgewählten Track aus der aktuellen Playlist.
 */
void MainWindow::onRemoveTrackButtonClicked()
{
    QModelIndex currentIndex = ui->songTable->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, tr("Fehler"), tr("Kein Track ausgewählt."));
        return;
    }

    int row = currentIndex.row();
    int trackID = playList.getTracks().at(row).getTrackID();
    int playlistID = playList.getPlaylistID();
    if (playlistID != 1) {
        if (db.removeTrackFromPlaylist(playlistID, trackID)) {
            loadPlaylistInTable(playList.getName());
        } else {
            QMessageBox::warning(this, tr("Fehler"), tr("Track konnte nicht entfernt werden."));
        }
    } else {
        QMessageBox::warning(this,
                             tr("Fehler"),
                             tr("Track kann nicht von Alle Songs entfernt werden"));
    }
}

