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
******************************************************************/
#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "mainwindow.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "myapp.h"
#include "sqlhelp.h"
#include "serverhelp.h"
#include "constvalue.h"
#include <QMouseEvent>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    this->InitForm();
    this->InitParameter();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::InitForm()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("登录");
    this->setWindowIcon(QIcon(DEFAULT_ICON));

    //安装事件监听器,让标题栏识别鼠标双击
    ui->Login_widget_title->installEventFilter(this);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min,QChar(0xf068),10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf179), 12);

    location = this->geometry();
    mousePressed = false;

    m_pSqlhelp = new SqlHelp(this);
}

void LoginWidget::InitParameter()
{
    m_pSqlhelp->BindDataToCombox("UserInfo","UserName",ui->txtUserName);
    ui->txtUserPwd->setText("admin");

    ui->cbxAutoLogin->setChecked(true);
    ui->btnLogin->setToolTip("登录");

    ui->txtUserName->setEditable(true);     //用户名输入框可以编辑
    ui->txtUserName->lineEdit()->setStyleSheet("margin:1px; border:0px;");
}

void LoginWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton))
    {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void LoginWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void LoginWidget::on_btnLogin_clicked()
{
    m_pserverhelp = new ServerHelp(ui->txtUserName->currentText(),ui->txtUserPwd->text());

    Myapp::LastLoginter = ui->txtUserName->currentText();
    Myapp::CurrentUserPwd = ui->txtUserPwd->text();
    Myapp::CurrentUserName = Myapp::LastLoginter;
    Myapp::CurrentUserType = QString("管理员");
    Myapp::WriteConfig();
    MainWindow::Instance()->show();
    this->hide();
}

void LoginWidget::on_btnMenu_Close_clicked()
{
    this->close();
}

void LoginWidget::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void LoginWidget::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void LoginWidget::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_Return) || ( e->key() == Qt::Key_Enter))
    {
        this->on_btnLogin_clicked();
    }
}

void LoginWidget::on_txtUserName_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->head->setStyleSheet("border-image: url(:/image/login/0.png);");
    }else if (index == 1)
    {
        ui->head->setStyleSheet("border-image: url(:/image/login/1.png);");
    }else if (index == 2)
    {
        ui->head->setStyleSheet("border-image: url(:/image/login/2.png);");
    }

}
