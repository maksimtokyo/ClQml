#ifndef CLUSER_H
#define CLUSER_H


#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QFile>
#include <QStandardPaths>

class ClUser : public QObject
{
    Q_OBJECT
public:
    ClUser(QObject *parent = nullptr, QString pathtojson = QString(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/user.json"));

    void setphonenumber(const QString& phonenumber);
    void setsusername(const QString& username);
    void setimgpath(const QString& imgpath);
    void setfirstmusicid(std::uint32_t firstmusicid);
    void setLastmusicid(std::uint32_t newLastmusicid);
    void setLastmusicname(const QString &newLastmusicname);
    void setLastmusicpath(const QString &newLastmusicpath);
    void setLastmusictimestop(int newLastmusictimestop);

    QString getphonenumber() const;
    QString getusername() const;
    QString getimgpath() const;
    std::uint32_t getfirstmusicid() const;
    std::uint32_t getlastmusicid() const;
    QString getlastmusicname() const;
    QString getlastmusicpath() const;
    int getlastmusictimestop() const;

private:

    bool loadfromjsonfile();
    bool savetojsonfile();

    QString m_pathtojson;
    QString m_phonenumber;
    QString m_username;
    QString m_imgpath;
    std::uint32_t m_firstmusicid;


    std::uint32_t m_lastmusicid;
    QString m_lastmusicname;
    QString m_lastmusicpath;
    int m_lastmusictimestop;
};

#endif // CLUSER_H
