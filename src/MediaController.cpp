#include "MediaController.h"
#include "DataBase.h"
#include <QFileDialog>
#include <QDir>
#include <iostream>
#include <QMessageBox>

MediaController::MediaController(Playlist &playlist)
    : playlist(playlist), currentIndex(0) {
    initializePlayer();
}

void MediaController::initializePlayer() {
    player.setAudioOutput(&audioOutput);
    connect(&player, &QMediaPlayer::positionChanged, [](qint64 position) {
        // Handle position changed
    });
    connect(&player, &QMediaPlayer::mediaStatusChanged, [](QMediaPlayer::MediaStatus status) {
        // Handle media status changed
    });
    connect(&player, &QMediaPlayer::errorOccurred, [](QMediaPlayer::Error error, const QString &errorString) {
        // Handle error occurred
    });
    audioOutput.setVolume(0.5);
}

void MediaController::playCurrent() {
    const auto &files = playlist.getFiles();
    if (currentIndex >= 0 && currentIndex < files.size()) {
        QString currentFile = files[currentIndex];
        std::cout << "Playing: " << currentFile.toStdString() << std::endl;
        player.setSource(QUrl::fromLocalFile(currentFile));
        player.play();
    }
}

void MediaController::next() {
    const auto &files = playlist.getFiles();
    if (currentIndex < files.size() - 1) {
        currentIndex++;
        playCurrent();
    }
}

void MediaController::prev() {
    if (currentIndex > 0) {
        currentIndex--;
        playCurrent();
    }
}

QStringList MediaController::selectMediaFiles(DataBase &database) {
    QStringList files;
    QMessageBox msgBox;
    msgBox.setText("Load media files");
    msgBox.setInformativeText("Do you want to load the playlist from the database or select a directory?");
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Open | QMessageBox::Save);
    msgBox.setDefaultButton(QMessageBox::Open);
    int ret = msgBox.exec();

    switch (ret) {
        case QMessageBox::Open: {
            QString dirPath = QFileDialog::getExistingDirectory(nullptr, "Select Media Directory");
            if (dirPath.isEmpty()) {
                std::cerr << "No directory selected." << std::endl;
                exit(-1);
            }
            QDir dir(dirPath);
            QStringList filters;
            filters << "*.mp3" << "*.mp4" << "*.wav" << "*.avi";
            files = dir.entryList(filters, QDir::Files);
            for (QString &file : files) {
                file = dir.absoluteFilePath(file);
            }
            break;
        }
        case QMessageBox::Save: {
            if (database.open()) {
                std::vector<QString> playlist;
                database.loadPlaylist(playlist);
                database.close();
                for (const auto &file : playlist) {
                    files.append(file);
                }
            }
            break;
        }
        case QMessageBox::Cancel:
        default:
            std::cerr << "Operation cancelled." << std::endl;
            exit(-1);
    }

    return files;
}

void MediaController::addFolderToPlaylist(const QString &folderPath) {
    QDir dir(folderPath);
    if (!dir.exists()) {
        std::cerr << "Directory does not exist: " << folderPath.toStdString() << std::endl;
        return;
    }

    QStringList filters;
    filters << "*.mp3" << "*.mp4" << "*.wav" << "*.avi";
    QStringList files = dir.entryList(filters, QDir::Files);
    for (QString &file : files) {
        file = dir.absoluteFilePath(file);
        playlist.addFile(file);
    }

    if (files.isEmpty()) {
        std::cerr << "No media files found in directory: " << folderPath.toStdString() << std::endl;
    } else {
        std::cout << "Added " << files.size() << " files to the playlist from directory: " << folderPath.toStdString() << std::endl;
    }
}