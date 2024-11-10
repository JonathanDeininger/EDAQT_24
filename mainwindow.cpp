#include "mainwindow.h"
#include "playlist.h"
#include "mediacontroller.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , playList()
    , mediaController(playList)
    , dataBase("C:/Users/enigm/Documents/Musicplayer_2")
{
    ui->setupUi(this);
    // Typischerweise: Initialisierung der UI-Elemente
    // Playlist = new QListWidget(this);
    // currentSongLabel = new QLabel(this);
    // choosePlaylistButton = new QPushButton(this);
    //mediaController = new MediaController(std::vector<QString>);
    connect(ui->Playlist, &QListWidget::itemClicked, this, &MainWindow::onPlaylistItemClicked);
    connect(ui->ChoosePlaylistButton, &QPushButton::clicked, this, &MainWindow::onChoosePlaylistButtonClicked);
    connect(ui->Play,&QPushButton::clicked, this, &MainWindow::onPlayButtonPressed);
    connect(ui->LautstaerkeRegler, &QSlider::valueChanged, this ,&MainWindow::onVolumeChanged);
    connect(ui->Pause,&QPushButton::clicked, this, &MainWindow::onPauseButtonPressed);
    // if (!dataBase.open()) {
    //     QMessageBox::critical(this, tr("Datenbankfehler"), tr("Datenbank konnte nicht geöffnet werden."));
    // } else {
    //     dataBase.initialize(); // Initialisiere die Datenbankstruktur, falls erforderlich
    // }
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::onChoosePlaylistButtonClicked()
{

    // Öffnet Windows-Explorer um den Ordner mit den Songs zu finden wird keiner ausgewählt verlässt er die Funktion
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Wähle einen Ordner"), QDir::homePath());
    if (folderPath.isEmpty()){
        return;
    }
    QDir directory(folderPath);
    // Sucht alle mp3 Dateien raus, wenn es keine gibt verlässt er die Funktion
    QStringList mp3Files = directory.entryList(QStringList() << "*.mp3", QDir::Files);
    if (mp3Files.isEmpty()) {
        QMessageBox::information(this, tr("Keine Dateien gefunden"), tr("Der ausgewählte Ordner enthält keine mp3-Dateien."));
        return;
    }

    // Löschen der Inhalte der Playlist, falls es bereits Einträge hat
    ui->Playlist->clear();

    // Füllen der Playlist mit den mp3-Dateien
    foreach (const QString &fileName, mp3Files) {
        QString fullPath = directory.absoluteFilePath(fileName);
        QListWidgetItem *item = new QListWidgetItem(fileName);
        item->setData(Qt::UserRole, fullPath);  // Speichert den vollständigen Pfad
        ui->Playlist->addItem(item);
        playList.addFile(fullPath);
    }
    mediaController.initializePlayer();

}


void MainWindow::onPlaylistItemClicked(QListWidgetItem *item){
    ui->currentSongLabel->setText(item->text());
    int index = ui->Playlist->row(item);
    mediaController.setCurrentIndex(index);
}

void MainWindow::onPlayButtonPressed(){
    mediaController.playCurrent();
}

void MainWindow::onVolumeChanged(float value){
    mediaController.getAudioOutput()->setVolume(value/100);
}

void MainWindow::onPauseButtonPressed(){

}

