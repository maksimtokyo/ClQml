#ifndef DBUSER_H
#define DBUSER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>

class dbuser : public QObject
{
    Q_OBJECT

public:
    dbuser(QObject *parent = nullptr);
    bool savechanges();

private:
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // DBUSER_H
