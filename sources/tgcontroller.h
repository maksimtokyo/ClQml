#ifndef TGCONTROLLER_H
#define TGCONTROLLER_H

#include <QtQml/qqmlmoduleregistration.h>

#include "td/telegram/Client.h"
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include "debugmodel.h"
#include "musiclist.h"
#include "tgclient.h"
#include "geniusclient.h"
#include "dbuser.h"
#include "cluser.h"

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <queue>

#include <vector>
#include <mutex>
#include <QMetaObject>
#include <QThread>
#include <QDebug>


class TgController : public  QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(TgController)
    Q_PROPERTY(DebugModel* deblist READ deblist  NOTIFY deblistChanged)
    Q_PROPERTY (musiclist* listmodel READ listmodel NOTIFY listmodelChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setphoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(QString code READ code WRITE setcode NOTIFY codeChanged)
    Q_PROPERTY (QString password READ password WRITE setpassword NOTIFY passwordChanged)
    Q_PROPERTY(QString view READ view NOTIFY viewChanged)

public:

    enum TypeSignal{ OUT, IN};
    TgController(QObject *parent = nullptr);
    DebugModel* deblist() {return client->m_deblist;}
    musiclist* listmodel() {return m_mlist;}


    void setphoneNumber(const QString &phoneNumber);
    QString phoneNumber() const;

    QString code();
    void setcode(const QString &code);

    QString password();
    void setpassword(const QString &password);


    QString view();
    ~TgController();


signals:

    void deblistChanged();
    void listmodelChanged();
    void phoneNumberChanged(TypeSignal type);
    void codeChanged(TypeSignal type);
    void passwordChanged(TypeSignal type);
    void idChanged(TypeSignal type);
    void installMusicSignal(int id_m);
    void loadmusicchat();
    void viewChanged();

public slots:

    void setPath(int index, QString path);
    void get_id_aut(int32_t idaut);
    void idautprocessing(TypeSignal type);
    void installMusic(int id_m);
    void sendlink(QString link);

private:

    QThread wThread;
    int32_t m_curid{-1};
    tgclient *client{nullptr};
    QString m_phoneNumber{""};
    QString m_code{""};
    QString m_password{""};
    QString m_view {"qmlfiles/phoneview.qml"};
    musiclist *m_mlist;
    dbuser *db;


    ClUser *m_cluser;
};
#endif // TGCONTROLLER_H
