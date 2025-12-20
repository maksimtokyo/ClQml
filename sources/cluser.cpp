#include "cluser.h"


ClUser::ClUser(QObject *parent, QString pathtojson)
    : QObject(parent),
      m_pathtojson(pathtojson),
      m_phonenumber(""),
      m_username(""),
      m_imgpath(""),
      m_firstmusicid(0),
      m_lastmusicid(0),
      m_lastmusicname(""),
      m_lastmusicpath(""),
      m_lastmusictimestop(0)
{
    qDebug() << "Path to json: " << m_pathtojson;
    loadfromjsonfile();
}

bool ClUser::loadfromjsonfile(){
    QFile jsonfile(m_pathtojson);

    if (!jsonfile.open(QIODevice::ReadOnly)){
        return false;
    }

    QByteArray data = jsonfile.readAll();
    jsonfile.close();

    QJsonParseError parsererror;
    QJsonDocument jsondoc = QJsonDocument::fromJson(data, &parsererror);

    if (parsererror.error != QJsonParseError::NoError){
        return false;
    }

    QJsonObject jsonobj = jsondoc.object();
    QJsonObject jsonuser = jsonobj.value("User").toObject();

    m_phonenumber = jsonuser.value("phonenumber").toString();
    m_username = jsonuser.value("username").toString();
    m_imgpath = jsonuser.value("imgpath").toString();
    m_firstmusicid = jsonuser.value("firstmusicid").toString().toUInt();


    QJsonObject jsonlast = jsonobj.value("LastMusic").toObject();

    m_lastmusicid = jsonlast.value("lasrmusicid").toString().toUInt();
    m_lastmusicname = jsonlast.value("lastmusicname").toString();
    m_lastmusicpath = jsonlast.value("lastmusicpath").toString();
    m_lastmusictimestop = jsonlast.value("lastmusictimestop").toInt();

    return true;
}

bool ClUser::savetojsonfile(){
    QJsonObject jsonobj;

    QJsonObject jsonuser;
    jsonuser["phonenumber"] = m_phonenumber;
    jsonuser["username"] = m_phonenumber;
    jsonuser["imgpath"] = m_imgpath;
    jsonuser["firstmusicid"] = QString::number(m_firstmusicid);
    jsonobj["User"] = jsonuser;


    QJsonObject lastmusic;
    lastmusic["lasrmusicid"] = QString::number(m_lastmusicid);
    lastmusic["lastmusicname"] = m_lastmusicname;
    lastmusic["lastmusicpath"] = m_lastmusicpath;
    lastmusic["lastmusictimestop"] = m_lastmusictimestop;
    jsonobj["LastMusic"] = lastmusic;

    QByteArray data = QJsonDocument(jsonobj).toJson();

    QFile jsonfile(m_pathtojson);
    if (!jsonfile.open(QIODevice::WriteOnly)){
        return false;
    }

    jsonfile.write(data);
    jsonfile.close();

    return true;
}

std::uint32_t ClUser::getlastmusicid() const
{
    return m_lastmusicid;
}

void ClUser::setLastmusicid(std::uint32_t newLastmusicid)
{
    m_lastmusicid = newLastmusicid;
}

QString ClUser::getlastmusicname() const
{
    return m_lastmusicname;
}

void ClUser::setLastmusicname(const QString &newLastmusicname)
{
    m_lastmusicname = newLastmusicname;
}

QString ClUser::getlastmusicpath() const
{
    return m_lastmusicpath;
}

void ClUser::setLastmusicpath(const QString &newLastmusicpath)
{
    m_lastmusicpath = newLastmusicpath;
}

int ClUser::getlastmusictimestop() const
{
    return m_lastmusictimestop;
}

void ClUser::setLastmusictimestop(int newLastmusictimestop)
{
    m_lastmusictimestop = newLastmusictimestop;
}

QString ClUser::getphonenumber() const
{
    return m_phonenumber;
}

QString ClUser::getusername() const
{
    return m_username;
}

QString ClUser::getimgpath() const
{
    return m_imgpath;
}

std::uint32_t ClUser::getfirstmusicid() const
{
    return m_firstmusicid;
}

void ClUser::setphonenumber(const QString& phonenumber){
    m_phonenumber = phonenumber;
}

void ClUser::setsusername(const QString& username){
    m_username = username;
}

void ClUser::setimgpath(const QString& imgpath){
    m_imgpath = imgpath;
}

void ClUser::setfirstmusicid(std::uint32_t firstmusicid){
    m_firstmusicid = firstmusicid;
}

ClUser::~ClUser(){
    savetojsonfile();
    qDebug() << "ClUser:: деструктор";

}

