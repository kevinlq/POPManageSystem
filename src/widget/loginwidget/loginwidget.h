/***************************************************************
 *Copyright(c) 2014-2015 Company By LH Unigress
 *All rights reserved.
 *文件名称：系统登录文件
 *简要描述：从数据库中匹配输入的密码，进行登录身份的判断
 *
 *当前版本：V2.0
 *作者：Kelvin Li
 *创作日期：2014/10
 *说明：输入密码正确进入系统，错误三次后系统自动退出
 *修改时间：2015-8-1
 *修改说明：
******************************************************************/
#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

class SqlHelp;
class ServerHelp;

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();
    void InitForm();
    void InitParameter();

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
    
private slots:
    void on_btnLogin_clicked();

    void on_btnMenu_Close_clicked();

    void on_btnMenu_Min_clicked();

    void on_txtUserName_currentIndexChanged(int index);

private:
    Ui::LoginWidget *ui;

    QPoint mousePoint;
    bool mousePressed;
    QRect location;

    SqlHelp *m_pSqlhelp;
    ServerHelp *m_pserverhelp;
};

#endif // LOGINWIDGET_H
