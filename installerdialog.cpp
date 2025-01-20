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

    dataBase.processDirectory(directory); // Process the base directory and its subdirectories
    bool successInsertPath = dataBase.insertPath(directory.absolutePath());
    if (successInsertPath) {
        // Create the "Alle Songs" playlist after inserting all tracks
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


bool InstallerDialog::isDatabaseEmpty() {
    QSqlQuery query(dataBase.getDatabase());
    query.exec("SELECT COUNT(*) FROM Mediathek");
    if (query.next()) {
        int count = query.value(0).toInt();
        return count == 0;
    }
    return true;
}
