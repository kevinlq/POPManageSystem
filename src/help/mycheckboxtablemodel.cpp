#include "mycheckboxtablemodel.h"

MyCheckboxTableModel::MyCheckboxTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

bool MyCheckboxTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( !index.isValid())
        return false;
    if (role == Qt::CheckStateRole && index.column() == 0)
    {
        check_state_map[index.row()] = (value == Qt::Checked ?Qt::Checked:Qt::Unchecked);
    }
    return true;
}

QVariant MyCheckboxTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch(role)
    {
        case Qt::CheckStateRole:
        if (index.column() == 0)
        {
            if (check_state_map.contains(index.row()))
                return check_state_map[index.row()] == Qt::Checked ? Qt::Checked : Qt::Unchecked;
            return Qt::Unchecked;
        }
    default:
        return QVariant();
    }
    return QVariant();
}

Qt::ItemFlags MyCheckboxTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if (index.column() == 0)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable |Qt::ItemIsUserCheckable;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
