#ifndef MYCHECKBOXTABLEMODEL_H
#define MYCHECKBOXTABLEMODEL_H

#include <QAbstractTableModel>

class MyCheckboxTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MyCheckboxTableModel(QObject *parent = 0);

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QMap<int ,Qt::CheckState> check_state_map;
    
};

#endif // MYCHECKBOXTABLEMODEL_H
