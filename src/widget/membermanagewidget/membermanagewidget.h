#ifndef MEMBERMANAGEWIDGET_H
#define MEMBERMANAGEWIDGET_H

#include <QWidget>

class QStandardItemModel;
class QCompleter;
class SqlHelp;
class QSqlQueryModel;
class MemberOperateWidget;
class QModelIndex;

namespace Ui {
class MemberManageWidget;
}

class MemberManageWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MemberManageWidget(QWidget *parent = 0);
    ~MemberManageWidget();
    void InitForm();
    void InitParameters();
    void CreateConnect();

    void CompleteEmail();                           //自动补全Email

    QString GetTableviewColumn();                   //获取鼠标选中行的第一列
    
private slots:
    void on_pbnAddMem_clicked();

    void on_Modify_pushButton_clicked();

    void on_Delete_pushButton_clicked();

    void on_Refresh_pushButton_clicked();

private slots:

    void slot_ShowControl(const int);                //显示控件的槽函数

    void slot_OnEmailChoosed(const QString &);

    void slot_OnTextChanged(const QString &);

    void slot_RefrechInfo();                         //刷新信息

    void slot_ShowToolTip(const QModelIndex &index);//设置鼠标跟踪提示


    void on_Select_pushButton_clicked();

    void on_pbnShowAll_clicked();

    void on_Tutor_Out_pushButton_clicked();

private:
    void SetDataShow();
    void SetDataHide();

private:
    Ui::MemberManageWidget *ui;

    QSqlQueryModel *QueryModel;                 //查询模型
    SqlHelp *m_pSqlhelp;                        //数据库操作对象
    QString ColumnNames[11];                    //列名数组声明
    int ColumnWidths[11];                       //列宽数组声明

    QCompleter *m_pCompleter;
    QStandardItemModel *m_pStandarModel;

    MemberOperateWidget *m_pMemberOperate;

    QStringList list_info;
};

#endif // MEMBERMANAGEWIDGET_H
