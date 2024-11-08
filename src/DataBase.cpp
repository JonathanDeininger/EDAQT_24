#include "DataBase.h"
#include <QDebug>

DataBase::DataBase(const QString &path) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
}

bool DataBase::open() {
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        return false;
    } else {
        qDebug() << "Database: connection ok";
        return true;
    }
}

void DataBase::close() {
    db.close();
}

bool DataBase::savePlaylist(const std::vector<QString> &playlist) {
    QSqlQuery query;
    query.prepare("INSERT INTO playlists (name) VALUES (?)");
    for (const QString &file : playlist) {
        query.addBindValue(file);
        if (!query.exec()) {
            qDebug() << "Error: failed to insert data -" << query.lastError();
            return false;
        }
    }
    return true;
}

bool DataBase::loadPlaylist(std::vector<QString> &playlist) {
    QSqlQuery query("SELECT name FROM playlists");
    while (query.next()) {
        QString name = query.value(0).toString();
        playlist.push_back(name);
    }
    return true;
}

bool DataBase::initialize() {
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS playlists (id INTEGER PRIMARY KEY, name TEXT)");
    if (!query.exec()) {
        qDebug() << "Error: failed to create table -" << query.lastError();
        return false;
    }
    return true;
}