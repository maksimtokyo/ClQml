#include "debugmodel.h"


DebugModel::DebugModel(QObject *parent) : QAbstractListModel(parent){

}

int DebugModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return m_items.size();
}

QVariant DebugModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid() || index.row() >= m_items.size())
        return {};


    if (role == TextRole)
        return m_items.at(index.row());

    return {};
}

QHash<int, QByteArray> DebugModel::roleNames() const{
    return{
       {TextRole , "text"}
    };
}

void DebugModel::addDebugString(const QString &msg){
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(msg);
    endInsertRows();
}

void DebugModel::clear(){
    beginResetModel();
    m_items.clear();
    endResetModel();
}

