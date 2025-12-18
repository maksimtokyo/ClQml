#ifndef DEBUGMODEL_H
#define DEBUGMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class DebugModel : public QAbstractListModel
{

    Q_OBJECT
public:

    enum Roles {
        TextRole = Qt::UserRole + 1
    };


    DebugModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void addDebugString(const QString &msg);
    Q_INVOKABLE void clear();

private:
    QStringList m_items;

};

#endif // DEBUGMODEL_H
