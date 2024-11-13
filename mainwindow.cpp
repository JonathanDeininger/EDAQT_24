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
    progressTimer = new QTimer(this);


    connect(ui->Playlist, &QListWidget::itemClicked, this, &MainWindow::onPlaylistItemClicked);
    connect(ui->ChoosePlaylistButton, &QPushButton::clicked, this, &MainWindow::onChoosePlaylistButtonClicked);
    connect(ui->Play,&QPushButton::clicked, this, &MainWindow::onPlayButtonPressed);
    connect(ui->LautstaerkeRegler, &QSlider::valueChanged, this ,&MainWindow::onVolumeChanged);
    connect(ui->Pause,&QPushButton::clicked, this, &MainWindow::onPauseButtonPressed);
    connect(ui->previousPushButton,&QPushButton::clicked, this, &MainWindow::onPreviousButtonPressed);
    connect(ui->nextPushButton,&QPushButton::clicked, this, &MainWindow::onNextButtonPressed);
    connect(ui->Fortschrittslider,&QSlider::sliderMoved, this,&MainWindow::onSliderMoved);
    connect(progressTimer, &QTimer::timeout, this, &MainWindow::updateProgressBar);
    //
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
    mediaController.initializePlayer();

}


void MainWindow::onPlaylistItemClicked(QListWidgetItem *item){
    //progressTimer->start(1000);
    ui->currentSongLabel->setText(item->text());
    int index = ui->Playlist->row(item);
    mediaController.setCurrentIndex(index);

}

void MainWindow::onPlayButtonPressed(){
    mediaController.playCurrent();
    int totalSeconds = mediaController.getCurrentSongDuration();// /1000 um Sekunden heraus zu bekommen
    std::cout << totalSeconds << std::endl;
    ui->Fortschrittslider->setRange(0,totalSeconds);
    // Umrechung in Stunden, Minuten und Sekunden
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    // std::cout << seconds << minutes << hours <<totalSeconds<< std::endl;

    // Anzeige im Format hh:mm:ss
    ui->currentSongDurationLabel->setText(
        QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'))
        );
}

void MainWindow::onVolumeChanged(float value){
    mediaController.getAudioOutput()->setVolume(value/100);
}

void MainWindow::onPauseButtonPressed(){
    mediaController.pauseCurrent();
}

void MainWindow::onPreviousButtonPressed(){
    mediaController.prev();
}

void MainWindow::onNextButtonPressed(){
    mediaController.next();
}


void MainWindow::updateProgressBar(){

    int currentPosition = mediaController.getCurrentSongPosition();
    if (currentPosition < mediaController.getCurrentSongDuration()) {
        mediaController.setCurrentSongPosition(currentPosition++); // Simuliert das Fortschreiten der Wiedergabe
        ui->Fortschrittslider->setValue(currentPosition);
    } else {
        progressTimer->stop(); // Stoppt den Timer, wenn das Lied zu Ende ist
    }

}

void MainWindow::onSliderMoved(){
    int currentPosition = mediaController.getCurrentSongPosition();
    mediaController.setCurrentSongPosition(currentPosition); // Setzt die aktuelle Position auf die gewählte
    ui->Fortschrittslider->setValue(currentPosition);
}
