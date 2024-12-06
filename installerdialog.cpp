#include "installerdialog.h"
#include "ui_installerdialog.h"


InstallerDialog::InstallerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InstallerDialog)
    , dataBase()
{
    ui->setupUi(this);
    connect(ui->ChoosePath, &QPushButton::clicked, this, &InstallerDialog::onChooseMusicFolderClicked);
    connect(ui->NextButton, &QPushButton::clicked, this, &InstallerDialog::onNextButtonClicked);
}

InstallerDialog::~InstallerDialog()
{
    delete ui;
}

void processFiles(DataBase &dataBase, const QStringList &fileList) {
    foreach (const QString &fileName, fileList) {
        // Entfernen der ".mp3"-Endung
        QString name = fileName;
        name.chop(4);  // Entfernt ".mp3"

        // Aufteilen des Dateinamens an den Unterstrichen "_"
        QStringList parts = name.split('_');

        // Interpret, Album und Titel extrahieren
        QString interpret = parts.value(0);
        QString album = parts.value(1, "");  // Wenn kein Album vorhanden, leeren String verwenden
        QString titel = parts.value(2);

        // Optional: Leerzeichen durch Bindestriche ersetzen
        interpret.replace(" ", "-");
        album.replace(" ", "-");
        titel.replace(" ", "-");

        // Spielzeit hier als Platzhalter 0 (muss später angepasst werden)
        int spielzeit = 0;  // Hier müsste eine Funktion zum Ermitteln der Spielzeit aufgerufen werden

        // Daten in die Datenbank einfügen
        if (!dataBase.insertData(interpret, album, titel, spielzeit)) {
            qDebug() << "Fehler beim Einfügen von Daten für" << fileName;
        }

    }
}

void InstallerDialog::onNextButtonClicked()
{
    bool successOpenDB = false;
    bool successMediathek = false;
    bool successOptions = false;
    bool successInsertOptions = false;
    successOpenDB = dataBase.open();
    if (successOpenDB)
    {
        QDir directory(QString(ui->MusicPath->toPlainText()));
        QString userName = QProcessEnvironment::systemEnvironment().value("USERNAME");
        QStringList audioFiles = directory.entryList(QStringList() << "*.mp3" << "*.wav" << "*.flac" << "*.aac", QDir::Files);
        if (audioFiles.isEmpty()) {
            QMessageBox::information(this, tr("Keine Dateien gefunden"), tr("Der ausgewählte Ordner enthält keine unterstützten Audiodateien."));
            return;
        }

        successMediathek = dataBase.createTableMediathek();
        if (successMediathek)
            successOptions = dataBase.createTableOptionen();
        if (successOptions)
            processFiles(dataBase,audioFiles);
        successInsertOptions = dataBase.insertDataIntoOptions(0,directory,userName);
        dataBase.close();
        if (successInsertOptions){
            close();
        }
    }
}

void InstallerDialog::onChooseMusicFolderClicked()
{
    // Öffnet Windows-Explorer um den Ordner mit den Songs zu finden wird keiner ausgewählt verlässt er die Funktion
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Wähle einen Ordner"), QDir::homePath());
    if (folderPath.isEmpty()){
        return;
    }
    ui->MusicPath->setText(folderPath);
    // Sucht alle unterstützten Audiodateien raus, wenn es keine gibt verlässt er die Funktion

}
