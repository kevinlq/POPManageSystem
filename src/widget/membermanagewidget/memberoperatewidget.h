#ifndef MEMBEROPERATEWIDGET_H
#define MEMBEROPERATEWIDGET_H

#include <QDialog>

class SqlHelp;
class BasicDataWidget;

namespace Ui {
class MemberOperateWidget;
}

class MemberOperateWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit MemberOperateWidget(QWidget *parent = 0);
    ~MemberOperateWidget();

    void InitForm();
    void InitParameter();
    void CreateConnect();

    //用来接收会员主界面发过来的信息
    void SetTitleInfo(QString Info, int flag, QString &id);

private:
    void AddMemberInfo();                           //添加会员信息
    void ModifyMemberInfo();                        //修改会员信息
    void ShowMemberInfo();                          //根据要修改的会员信息进行显示
    void ClearMemberInfo();                         //清空会员界面信息

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    
private slots:
    void on_btnMenu_Close_clicked();

    void on_pbnCnacel_clicked();

    void on_pbnGetMemberId_clicked();

    void on_pbnSave_clicked();

    void on_Find_pushButton_clicked();

    void on_pbnCardRule_clicked();

    void on_Vip_comboBox_activated(const QString &arg1);

private slots:
    void slot_updateVipInfo();
    //void slot_UpdateUnitInfo();
    //void slot_UpdateSupplyInfo();
signals:
    void signal_addMenberOk();                      //添加会员信息成功

private:
    Ui::MemberOperateWidget *ui;

    QPoint mousePoint;
    bool mousePressed;
    QRect location;

    SqlHelp *m_pSqlhelp;
    BasicDataWidget *m_pBascWidget;
};

#endif // MEMBEROPERATEWIDGET_H
