#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <QDir>
// #include <vector>

class DataBase {
public:
    DataBase();
    ~DataBase();
    bool createTableMediathek();
    bool createTableOptionen();
    bool insertData(const QString &interpret, const QString &album, const QString &titel, int spielzeit);
    bool insertDataIntoOptions(const float &LautStaerke, QDir &MusikPfad, QString &NutzerName);
    bool doesUserNameExists(const QString &NutzerName);
    void queryData();
    bool open();
    void close();
    // bool savePlaylist(const std::vector<QString> &playlist);
    // bool loadPlaylist(std::vector<QString> &playlist);
    // bool initialize();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
