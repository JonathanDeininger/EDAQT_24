#include "installerdialog.h"
#include "ui_installerdialog.h"
#include "track.h"
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <iostream>
#include <qdebug.h>

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
        qDebug() << "Failed to open the database in InstallerDialog constructor.";
    }
}

InstallerDialog::~InstallerDialog()
{
    dataBase.close();
    delete ui;
}

void processFiles(DataBase &dataBase, const QString &basePath, const QStringList &fileList) {
    foreach (const QString &fileName, fileList) {
        Track track(basePath, fileName);
        qDebug() << "Processing file:" << fileName;
        qDebug() << "File path:" << track.getFilePath();
        qDebug() << "Artist:" << track.getArtist();
        qDebug() << "Album:" << track.getAlbum();
        qDebug() << "Title:" << track.getTitle();
        qDebug() << "Duration:" << track.getDuration();
        qDebug() << "Sample Rate:" << track.getSampleRate();
        qDebug() << "Sample Count:" << track.getSampleCount();
        qDebug() << "Hash:" << track.getHash();

        // Daten in die Datenbank einfügen
        if (!dataBase.insertData(track.getFilePath(), track.getArtist(), track.getAlbum(), track.getTitle(), track.getDuration(), track.getSampleRate(), track.getSampleCount(), track.getHash())) {
            qDebug() << "Fehler beim Einfügen von Daten für" << fileName;
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

void InstallerDialog::onNextButtonClicked() {
    QDir directory(QString(ui->MusicPath->toPlainText()));
    QStringList audioFiles = directory.entryList(QStringList() << "*.mp3" << "*.wav" << "*.flac" << "*.aac", QDir::Files);

    if (audioFiles.isEmpty()) {
        QMessageBox::information(this, tr("Keine Dateien gefunden"), tr("Der ausgewählte Ordner enthält keine unterstützten Audiodateien."));
        return;
    }

    bool successMediathek = dataBase.createTableMediathek();
    if (successMediathek) {
        bool successOptions = dataBase.createTableOptionen();
        if (successOptions) {
            processFiles(dataBase, directory.absolutePath(), audioFiles);
            QList<QPair<float, QString>> options;
            options.append(qMakePair(0.0f, directory.absolutePath()));
            bool successInsertOptions = dataBase.insertOptions(options);
            if (successInsertOptions) {
                close();
            }
        }
    }
}

void InstallerDialog::onChooseMusicFolderClicked() {
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Wähle einen Ordner"), QDir::homePath());
    if (folderPath.isEmpty()) {
        return;
    }
    ui->MusicPath->setText(folderPath);

    // Save the base path to the database
    qDebug() << "Attempting to insert path into database:" << folderPath;
    QList<QPair<float, QString>> options;
    options.append(qMakePair(0.5f, folderPath));
    if (!dataBase.insertOptions(options)) {
        qDebug() << "Failed to insert path into database.";
        QMessageBox::warning(this, tr("Fehler"), tr("Der Pfad konnte nicht in die Datenbank geschrieben werden."));
        return;
    }

    // Process the base directory and its subdirectories
    processDirectory(dataBase, QDir(folderPath));
}
