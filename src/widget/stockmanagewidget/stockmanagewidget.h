#ifndef STOCKMANAGEWIDGET_H
#define STOCKMANAGEWIDGET_H

#include <QWidget>

class SqlHelp;
class myHelper;
class QTreeWidgetItem;
class QModelIndex;


namespace Ui {
class StockManageWidget;
}

class StockManageWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit StockManageWidget(QWidget *parent = 0);
    ~StockManageWidget();

    void InitForm();
    void InitWidget();
    void InitParameter();
    
private slots:

    void slot_treeClicked(const QModelIndex &index);

    void on_pbnFind_clicked();

    void on_pbnShowAll_clicked();

    void on_pbnModify_clicked();

private:
    Ui::StockManageWidget *ui;

    SqlHelp *m_pSqlhelp;
    QString ColumnNames[13];        //列名数组声明
    int ColumnWidths[13];           //列宽数组声明

    myHelper *m_pMyhelp;
};

#endif // STOCKMANAGEWIDGET_H
