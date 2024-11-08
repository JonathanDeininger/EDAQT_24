#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <vector>

class DataBase {
public:
    DataBase(const QString &path);
    bool open();
    void close();
    bool savePlaylist(const std::vector<QString> &playlist);
    bool loadPlaylist(std::vector<QString> &playlist);
    bool initialize();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H