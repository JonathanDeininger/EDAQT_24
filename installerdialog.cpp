#include "installerdialog.h"
#include <QMediaMetaData>
#include <QMediaPlayer>
#include "database.h"
#include "ui_installerdialog.h"

/**
 * Konstruktor der InstallerDialog-Klasse.
 * Initialisiert die Benutzeroberfläche und überprüft die Datenbank.
 * @param parent Der Eltern-Widget, falls vorhanden.
 */
InstallerDialog::InstallerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InstallerDialog)
    , dataBase()
{
    ui->setupUi(this);
    connect(ui->ChoosePath,
            &QPushButton::clicked,
            this,
            &InstallerDialog::onChooseMusicFolderClicked);
    connect(ui->NextButton, &QPushButton::clicked, this, &InstallerDialog::onNextButtonClicked);

    // Öffnet die Datenbank bei der Initialisierung.
    if (!dataBase.open()) {
        // Fehlerbehandlung, falls die Datenbank nicht geöffnet werden kann.
    }

    // Überprüft und erstellt bei Bedarf die erforderlichen Tabellen.
    if (!dataBase.tableExists("Mediathek") && !dataBase.createTableMediathek()) {
        // Fehlerbehandlung für fehlgeschlagene Tabellenerstellung.
    }
    if (!dataBase.tableExists("Pathlist") && !dataBase.createTablePathlist()) {
        // Fehlerbehandlung für fehlgeschlagene Tabellenerstellung.
    }
    if (!dataBase.tableExists("Playlists") && !dataBase.createTablePlaylists()) {
        // Fehlerbehandlung für fehlgeschlagene Tabellenerstellung.
    }
    if (!dataBase.tableExists("PlaylistTracks") && !dataBase.createTablePlaylistTracks()) {
        // Fehlerbehandlung für fehlgeschlagene Tabellenerstellung.
    }

    // Überprüft, ob die "Alle Songs"-Playlist leer ist und fügt sie hinzu, falls nötig.
    if (dataBase.getPlaylist("Alle Songs").getTracks().empty()) {
        dataBase.insertPlaylist("Alle Songs");
    }
}

/**
 * Destruktor der InstallerDialog-Klasse.
 * Schließt die Datenbank und bereinigt die Benutzeroberfläche.
 */
InstallerDialog::~InstallerDialog()
{
    dataBase.close();
    delete ui;
}

/**
 * Überprüft rekursiv, ob ein Verzeichnis unterstützte Audiodateien enthält.
 * @param directory Das zu überprüfende Verzeichnis.
 * @return true, wenn Audiodateien gefunden wurden, sonst false.
 */
bool hasAudioFiles(const QDir &directory)
{
    // Sucht nach unterstützten Audiodateien im aktuellen Verzeichnis.
    QStringList audioFiles = directory.entryList(QStringList()
                                                     << "*.mp3" << "*.wav" << "*.flac" << "*.aac",
                                                 QDir::Files);
    if (!audioFiles.isEmpty()) {
        return true;
    }

    // Durchsucht Unterverzeichnisse rekursiv nach Audiodateien.
    QStringList subDirs = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &subDir, subDirs) {
        QDir subDirectory(directory.absoluteFilePath(subDir));
        if (hasAudioFiles(subDirectory)) {
            return true;
        }
    }

    return false;
}

/**
 * Slot, der ausgeführt wird, wenn der "Weiter"-Button geklickt wird.
 * Überprüft, ob der ausgewählte Ordner Audiodateien enthält, und verarbeitet diese.
 */
void InstallerDialog::onNextButtonClicked()
{
    QDir directory(QString(ui->MusicPath->toPlainText()));

    if (!hasAudioFiles(directory)) {
        QMessageBox::information(
            this,
            tr("Keine Dateien gefunden"),
            tr("Der ausgewählte Ordner enthält keine unterstützten Audiodateien."));
        return;
    }

    dataBase.processDirectory(directory); // Verarbeitet das Verzeichnis und seine Unterverzeichnisse.
    bool successInsertPath = dataBase.insertPath(directory.absolutePath());
    if (successInsertPath) {
        // Schließt den Dialog und signalisiert erfolgreichen Abschluss.
        accept();
    }
}

/**
 * Slot, der ausgeführt wird, wenn der "Ordner wählen"-Button geklickt wird.
 * Öffnet einen Dialog zur Auswahl eines Verzeichnisses und speichert den Pfad.
 */
void InstallerDialog::onChooseMusicFolderClicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,
                                                           tr("Wähle einen Ordner"),
                                                           QDir::homePath());
    if (folderPath.isEmpty()) {
        return; // Abbrechen, wenn kein Ordner ausgewählt wurde.
    }
    ui->MusicPath->setText(folderPath);

    // Speichert den Basisordner in der Datenbank.
    if (!dataBase.insertPath(folderPath)) {
        QMessageBox::warning(this,
                             tr("Fehler"),
                             tr("Der Pfad konnte nicht in die Datenbank geschrieben werden."));
        return;
    }
}

/**
 * Überprüft, ob die Datenbank leer ist.
 * @return true, wenn keine Einträge in der Mediathek vorhanden sind, sonst false.
 */
bool InstallerDialog::isDatabaseEmpty()
{
    QSqlQuery query(dataBase.getDatabase());
    query.exec("SELECT COUNT(*) FROM Mediathek");
    if (query.next()) {
        int count = query.value(0).toInt();
        return count == 0; // Gibt true zurück, wenn keine Einträge vorhanden sind.
    }
    return true; // Rückgabe bei fehlgeschlagener Abfrage.
}
