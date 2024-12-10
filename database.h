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
    bool insertData(const QString &filePath, const QString &interpret, const QString &album, const QString &titel, int spielzeit, int sampleRate, int sampleCount, const QByteArray &hash);
    bool insertOptions(const QList<QPair<float, QString>> &options);
    void queryData();
    bool open();
    void close();
    QSqlDatabase& getDatabase(); // Add this method to access the database

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
