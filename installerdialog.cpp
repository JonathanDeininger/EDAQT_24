#include "installerdialog.h"
#include "ui_installerdialog.h"
#include "track.h"
#include <QMediaPlayer>
#include <QMediaMetaData>
#include "database.h"

InstallerDialog::InstallerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InstallerDialog)
    , dataBase()
{
    ui->setupUi(this);
    connect(ui->ChoosePath, &QPushButton::clicked, this, &InstallerDialog::onChooseMusicFolderClicked);
    connect(ui->NextButton, &QPushButton::clicked, this, &InstallerDialog::onNextButtonClicked);

    // Ensure the database is opened in the constructor
    if (!dataBase.open()) {
        // Handle database open failure
    }

    // Check if the necessary tables already exist
    if (!dataBase.tableExists("Mediathek")) {
        if (!dataBase.createTableMediathek()) {
            // Handle table creation failure
        }
    }
    if (!dataBase.tableExists("Pathlist")) {
        if (!dataBase.createTablePathlist()) {
            // Handle table creation failure
        }
    }
    if (!dataBase.tableExists("Playlists")) {
        if (!dataBase.createTablePlaylists()) {
            // Handle table creation failure
        }
    }
    if (!dataBase.tableExists("PlaylistTracks")) {
        if (!dataBase.createTablePlaylistTracks()) {
            // Handle table creation failure
        }
    }

    // Check if the "Alle Songs" playlist is empty
    if (dataBase.getPlaylist("Alle Songs").getTracks().empty()) {
        dataBase.insertPlaylist("Alle Songs");
    }
}

InstallerDialog::~InstallerDialog()
{
    dataBase.close();
    delete ui;
}

void processFiles(DataBase &dataBase, const QString &basePath, const QStringList &fileList) {
    foreach (const QString &fileName, fileList) {
        Track track(QDir(basePath).absoluteFilePath(fileName));
        // Daten in die Datenbank einfügen
        if (!dataBase.insertData(track.getFilePath(), track.getArtist(), track.getAlbum(), track.getTitle(), track.getDuration(), track.getSampleRate(), track.getSampleCount())) {
            // Handle data insertion failure
        }
    }
}

void processDirectory(DataBase &dataBase, const QDir &directory) {
    QStringList audioFiles = directory.entryList(QStringList() << "*.mp3" << "*.wav" << "*.flac" << "*.aac", QDir::Files);
    processFiles(dataBase, directory.absolutePath(), audioFiles);

    QStringList subDirs = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &subDir, subDirs) {
        QDir subDirectory(directory.absoluteFilePath(subDir));
        processDirectory(dataBase, subDirectory);
    }
}

bool hasAudioFiles(const QDir &directory) {
    QStringList audioFiles = directory.entryList(QStringList() << "*.mp3" << "*.wav" << "*.flac" << "*.aac", QDir::Files);
    if (!audioFiles.isEmpty()) {
        return true;
    }

    QStringList subDirs = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &subDir, subDirs) {
        QDir subDirectory(directory.absoluteFilePath(subDir));
        if (hasAudioFiles(subDirectory)) {
            return true;
        }
    }

    return false;
}

void InstallerDialog::onNextButtonClicked() {
    QDir directory(QString(ui->MusicPath->toPlainText()));

    if (!hasAudioFiles(directory)) {
        QMessageBox::information(this, tr("Keine Dateien gefunden"), tr("Der ausgewählte Ordner enthält keine unterstützten Audiodateien."));
        return;
    }

    processDirectory(dataBase, directory); // Process the base directory and its subdirectories
    bool successInsertPath = dataBase.insertPath(directory.absolutePath());
    if (successInsertPath) {
        // Create the "Alle Songs" playlist after inserting all tracks
        dataBase.createAllSongsPlaylist();
        accept(); // Close the dialog and return QDialog::Accepted
    }
}

void InstallerDialog::onChooseMusicFolderClicked() {
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Wähle einen Ordner"), QDir::homePath());
    if (folderPath.isEmpty()) {
        return;
    }
    ui->MusicPath->setText(folderPath);

    // Save the base path to the database
    if (!dataBase.insertPath(folderPath)) {
        QMessageBox::warning(this, tr("Fehler"), tr("Der Pfad konnte nicht in die Datenbank geschrieben werden."));
        return;
    }
}

void InstallerDialog::addAllSongsToPlaylist() {
    DataBase db;
    if (!db.open()) {
        // Handle database open failure
        return;
    }

    // Create the "Alle Songs" playlist if it doesn't exist
    if (!db.insertPlaylist("Alle Songs")) {
        // Handle playlist creation failure
    }

    QSqlQuery query(db.getDatabase());
    query.exec("SELECT TrackID FROM Mediathek");

    while (query.next()) {
        int trackID = query.value(0).toInt();
        db.insertPlaylistTrack(1, trackID); // Assuming "Alle Songs" has PlaylistID 1
    }

    db.close();
}

// Modify the method where songs are loaded into the database to call addAllSongsToPlaylist
void InstallerDialog::loadSongsIntoDatabase() {
    // ...existing code to load songs into the database...

    // After loading all songs, add them to the "Alle Songs" playlist
    addAllSongsToPlaylist();
}

bool InstallerDialog::isDatabaseEmpty() {
    QSqlQuery query(dataBase.getDatabase());
    query.exec("SELECT COUNT(*) FROM Mediathek");
    if (query.next()) {
        int count = query.value(0).toInt();
        return count == 0;
    }
    return true;
}
