#include "dbuser.h"


dbuser::dbuser(QObject *parent)
    : QObject(parent), db(QSqlDatabase::addDatabase("QSQLITE", "cluserdbconnection"))
{

    db.setDatabaseName("cluserdb");
    db.open();
}


bool dbuser::savechanges(){
}
