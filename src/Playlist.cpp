#include "Playlist.h"
#include "MediaController.h"
#include "DataBase.h"

void Playlist::addFilesFromArgs(const QStringList &args, MediaController &mediaController, DataBase &database) {
    if (args.isEmpty()) {
        QStringList files = mediaController.selectMediaFiles(database);
        for (const QString &file : files) {
            addFile(file);
        }
    } else {
        QFileInfo fileInfo(args.first());
        if (fileInfo.isDir()) {
            QDir dir(args.first());
            QStringList filters;
            filters << "*.mp3" << "*.mp4" << "*.wav" << "*.avi";
            QStringList files = dir.entryList(filters, QDir::Files);
            for (QString &file : files) {
                file = dir.absoluteFilePath(file);
                addFile(file);
            }
        } else {
            addFile(args.first());
        }
    }

    if (getFiles().empty()) {
        std::cerr << "No media files found." << std::endl;
        exit(-1);
    }
}