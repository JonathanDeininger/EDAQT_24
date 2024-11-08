#include "CommandListener.h"
#include "MediaController.h"
#include "DataBase.h"
#include <iostream>
#include <atomic>
#include <QMetaObject>
#include <QFileDialog> // Add this include

extern std::atomic<bool> running;

void volUp(QAudioOutput *audioOutput) {
    audioOutput->setVolume(std::min(audioOutput->volume() + 0.1, 1.0));
}

void volDown(QAudioOutput *audioOutput) {
    audioOutput->setVolume(std::max(audioOutput->volume() - 0.1, 0.0));
}

void next(MediaController *mediaController) {
    mediaController->next();
}

void prev(MediaController *mediaController) {
    mediaController->prev();
}

void commandListener(MediaController *mediaController, DataBase *database) {
    std::string command;
    while (running) {
        std::cin >> command;
        if (command == "pause") {
            QMetaObject::invokeMethod(mediaController->getPlayer(), "pause", Qt::QueuedConnection);
            std::cout << "Player paused." << std::endl;
        } else if (command == "play") {
            QMetaObject::invokeMethod(mediaController->getPlayer(), "play", Qt::QueuedConnection);
            std::cout << "Player playing." << std::endl;
        } else if (command == "stop") {
            QMetaObject::invokeMethod(mediaController->getPlayer(), "stop", Qt::QueuedConnection);
            std::cout << "Player stopped." << std::endl;
        } else if (command == "next") {
            QMetaObject::invokeMethod(mediaController, [mediaController]() { mediaController->next(); }, Qt::QueuedConnection);
            std::cout << "Next track." << std::endl;
        } else if (command == "prev") {
            QMetaObject::invokeMethod(mediaController, [mediaController]() { mediaController->prev(); }, Qt::QueuedConnection);
            std::cout << "Previous track." << std::endl;
        } else if (command == "volUp") {
            QMetaObject::invokeMethod(mediaController->getAudioOutput(), [mediaController]() { volUp(mediaController->getAudioOutput()); }, Qt::QueuedConnection);
            std::cout << "Volume up." << std::endl;
        } else if (command == "volDown") {
            QMetaObject::invokeMethod(mediaController->getAudioOutput(), [mediaController]() { volDown(mediaController->getAudioOutput()); }, Qt::QueuedConnection);
            std::cout << "Volume down." << std::endl;
        } else if (command == "save") {
            QMetaObject::invokeMethod(mediaController, [mediaController, database]() {
                std::vector<QString> playlist = mediaController->getPlaylist().getFiles();
                if (database->open()) {
                    database->savePlaylist(playlist);
                    database->close();
                    std::cout << "Playlist saved." << std::endl;
                }
            }, Qt::QueuedConnection);
        } else if (command == "load") {
            QMetaObject::invokeMethod(mediaController, [mediaController, database]() {
                std::vector<QString> playlist;
                if (database->open()) {
                    database->loadPlaylist(playlist);
                    database->close();
                    mediaController->getPlaylist().setFiles(playlist);
                    mediaController->playCurrent();
                    std::cout << "Playlist loaded." << std::endl;
                }
            }, Qt::QueuedConnection);
        } else if (command == "addFolder") {
            QMetaObject::invokeMethod(mediaController, [mediaController]() {
                QString dirPath = QFileDialog::getExistingDirectory(nullptr, "Select Media Directory");
                if (!dirPath.isEmpty()) {
                    mediaController->addFolderToPlaylist(dirPath);
                    std::cout << "Folder added to playlist." << std::endl;
                } else {
                    std::cerr << "No directory selected." << std::endl;
                }
            }, Qt::QueuedConnection);
        } else if (command == "exit") {
            running = false;
        }
    }
}