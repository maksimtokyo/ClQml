#ifndef MUSICLIST_H
#define MUSICLIST_H


#include <QAbstractListModel>
#include <QStringList>

struct music{

    QString musicpath {""};
    QString musicname {""};
    std::int32_t music_id {0};

    music(QString musicname = "", std::int32_t music_id = 0, QString  musicpath = "")
        : musicname(musicname), music_id(music_id), musicpath(musicpath) {}
};

class musiclist : public QAbstractListModel
{
    Q_OBJECT

public:

    enum Roles {
        TextRole = Qt::UserRole + 1,
        MusicRole = Qt::UserRole + 2,

    };

    musiclist(QObject *parent = nullptr);

    Q_INVOKABLE void addmusic(const music &msc);
    Q_INVOKABLE void clear();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void setPath(int index, QString path);

    std::uint32_t getidmusic(int index);
    QList<music> m_musics;
};

#endif // MUSICLIST_H
