#include <QApplication>
#include <QCommandLineParser>
#include <thread>
#include <atomic>
#include "MediaController.h"
#include "Playlist.h"
#include "CommandListener.h"
#include "DataBase.h"

std::atomic<bool> running(true);

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("file", "The media file or directory to play.");
    parser.process(app);
    QStringList args = parser.positionalArguments();

    Playlist playlist;
    MediaController mediaController(playlist);
    DataBase database("playlist.db");

    playlist.addFilesFromArgs(args, mediaController, database);
    playlist.print();
    mediaController.playCurrent();

    if (database.open()) {
        if (database.initialize()) { // Initialize the database
            database.savePlaylist(playlist.getFiles());
        }
        database.close();
    }

    std::thread listenerThread(commandListener, &mediaController, &database);
    int result = app.exec();
    running = false;
    listenerThread.join();
    return result;
}