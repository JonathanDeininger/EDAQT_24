#ifndef COMMAND_LISTENER_H
#define COMMAND_LISTENER_H

#include "MediaController.h"
#include "DataBase.h" // Include DataBase header
#include <QMediaPlayer>
#include <QAudioOutput>
#include <atomic>

extern std::atomic<bool> running;

void commandListener(MediaController *mediaController, DataBase *database);
void volUp(QAudioOutput *audioOutput);
void volDown(QAudioOutput *audioOutput);
void next(QMediaPlayer *player);
void prev(QMediaPlayer *player);

#endif // COMMAND_LISTENER_H