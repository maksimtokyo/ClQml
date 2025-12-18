#include "musiclist.h"


musiclist::musiclist(QObject *parent) : QAbstractListModel(parent){

}

int musiclist::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return m_musics.size();
}

QVariant musiclist::data(const QModelIndex &index, int role) const{
    if (!index.isValid() || index.row() >= m_musics.size())
        return {};


    if (role == TextRole)
        return m_musics.at(index.row()).musicname;
    if (role == MusicRole)
        return m_musics.at(index.row()).musicpath;

    return {};
}

QHash<int, QByteArray> musiclist::roleNames() const{
    return{
       {TextRole , "text"},
       {MusicRole, "music"}
    };
}

void musiclist::addmusic(const music &msc){
    beginInsertRows(QModelIndex(), m_musics.size(), m_musics.size());
    m_musics.append(msc);
    qDebug() << msc.musicname;
    endInsertRows();
}

std::uint32_t musiclist::getidmusic(int index){
    return m_musics[index].music_id;
}

void musiclist::clear(){
    beginResetModel();
    m_musics.clear();
    endResetModel();
}

void musiclist::setPath(int index, QString path){
    m_musics[index] = path;
    emit dataChanged(this->index(index), this->index(index));
}
