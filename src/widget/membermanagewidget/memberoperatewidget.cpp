#include "memberoperatewidget.h"
#include "ui_memberoperatewidget.h"
#include <QMouseEvent>
#include <QSqlError>
#include <QDebug>
#include <QBitmap>
#include <QPainter>
#include "src/help/iconhelper.h"
#include "sqlhelp.h"
#include "myhelper.h"
#include <QSqlQuery>
#include "basicdatawidget.h"

static int m_pFlags = 0;                           //添加或修改会员信息标志(1:添加，2：修改)
static QString memberId = "";                      //会员ID(用于修改会员信息)

MemberOperateWidget::MemberOperateWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MemberOperateWidget)
{
    ui->setupUi(this);

    this->InitForm();
    this->InitParameter();
    this->CreateConnect();
}

MemberOperateWidget::~MemberOperateWidget()
{
    delete ui;
}

void MemberOperateWidget::InitForm()
{
    //隐藏边框
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf00a), 12);

    location = this->geometry();
    mousePressed = false;

    m_pSqlhelp = new SqlHelp();
    m_pBascWidget = new BasicDataWidget();
}

/*
 *功能：参数初始化
*/
void MemberOperateWidget::InitParameter()
{
    //清空信息
    ui->Id_lineEdit->clear();
    ui->Name_lineEdit->clear();
    ui->Phone_lineEdit->clear();
    ui->Address_lineEdit->clear();

    //将数据绑定到下拉框
    m_pSqlhelp->BindDataToCombox("MemberGrade","MemGradeName",ui->Vip_comboBox);

    ui->Birthday_dateEdit->setDate(QDate::currentDate());
    ui->Regist_dateEdit->setDate(QDate::currentDate());
}

void MemberOperateWidget::CreateConnect()
{
    connect(ui->Vip_comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(on_Vip_comboBox_activated(QString)));
    connect(m_pBascWidget,SIGNAL(signal_VipInfoOk()),this,SLOT(slot_updateVipInfo()));
}

/*
 *功能：用来接收会员主界面传过来的信息
 *调用：会员主界面调用
 *Info:标题栏显示对应的数据
 *flag:1：添加会员信息
 *     0：修改会员信息
 *id:会员编号
*/
void MemberOperateWidget::SetTitleInfo(QString Info, int flag, QString &id)
{
    //显示标题栏
    ui->lab_Title->setText(Info);
    m_pFlags = flag;
    memberId = id;
    qDebug() <<"m_pFlags:"<<m_pFlags<<",memberId:"<<memberId;
    //如果是添加会员信息，则清空上次的信息,否则显示所修改会员的信息
    if (m_pFlags)
    {
        this->setWindowTitle("添加会员信息");
        ClearMemberInfo();
    }else
    {
        this->setWindowTitle("修改会员信息");
        ShowMemberInfo();
    }
}

/*
 *处理添加会员信息操作
 *调用：本文件调用
*/
void MemberOperateWidget::AddMemberInfo()
{
    /****获取输入的管理员的信息*/
    QString member_id = ui->Id_lineEdit->text();
    QString member_name = ui->Name_lineEdit->text();
    QString member_phone = ui->Phone_lineEdit->text();
    QString member_address = ui->Address_lineEdit->text();
    QString member_class = ui->Vip_comboBox->itemText(ui->Vip_comboBox->currentIndex());
    QString member_grade = ui->Grade_lineEdit->text();
    QString member_cost = ui->Money_lineEdit->text();
    QString member_register = ui->Regist_dateEdit->text();
    QString member_birthday = ui->Birthday_dateEdit->text();
    QString member_email = ui->Email_lineEdit->text();
    QString member_remark = ui->Remark_lineEdit->text();

    QString CurrentTime = QDate::currentDate().toString("yyyy-MM-dd");

    if(ui->Id_lineEdit->text().isEmpty())
    {
        myHelper::ShowMessageBoxInfo("会员编号不能为空，请输入会员编号!");
    }
    else if(ui->Name_lineEdit->text().isEmpty())
    {
        myHelper::ShowMessageBoxInfo("会员姓名不能为空，请输入会员姓名!");
    }
    else
    {

        QSqlQuery query;
        QString sql = "INSERT INTO Member (MemberId, MemberName,MemberPhone,MemberAddress,";
        sql += "MemberClass,MemberGrade,MemberCost,MemberRegister,MemberBirthday,";
        sql += "MemberEmail,MemberRemark)";
        sql += "VALUES (:MemberId,:MemberName,:MemberPhone,:MemberAddress,:MemberClass,";
        sql += ":MemberGrade,:MemberCost,:MemberRegister,:MemberBirthday,:MemberEmail,:MemberRemark)";

        qDebug() <<sql;
        bool ok = query.prepare(sql);
        query.bindValue(":MemberId",member_id);
        query.bindValue(":MemberName",member_name);
        query.bindValue(":MemberPhone",member_phone);
        query.bindValue(":MemberAddress",member_address);
        query.bindValue(":MemberClass",member_class);
        query.bindValue(":MemberGrade",member_grade);
        query.bindValue(":MemberCost",member_cost);
        query.bindValue(":MemberRegister",member_register);
        query.bindValue(":MemberBirthday",member_birthday);
        query.bindValue(":MemberEmail",member_email);
        query.bindValue(":MemberRemark",member_remark);

        query.setForwardOnly(true);
        query.exec();

        if(ok)
        {
            myHelper::ShowMessageBoxInfo("添加会员信息成功!");
        }else
        {
            myHelper::ShowMessageBoxInfo("添加会员信息失败!"+query.lastError().text());
        }
    }
}

/*
 *功能：修改会员信息操作
 *调用：本文件调用
*/
void MemberOperateWidget::ModifyMemberInfo()
{
    QString sql = "UPDATE Member SET MemberName='"+ui->Name_lineEdit->text()+"',";
    sql += "MemberPhone='"+ui->Phone_lineEdit->text()+"',";
    sql += "MemberEmail='"+ui->Email_lineEdit->text()+"',";
    sql +="MemberAddress='"+ui->Address_lineEdit->text()+"',";
    sql += "MemberRemark='"+ui->Remark_lineEdit->text()+"' ";
    sql +=" WHERE MemberId="+ui->Id_lineEdit->text();
    qDebug() <<"Update Member:"<<sql;

    if (m_pSqlhelp->ExecuteSql(sql))
    {
        myHelper::ShowMessageBoxInfo("修改会员信息成功!");
        return;
    }
}

void MemberOperateWidget::ShowMemberInfo()
{
    QString sql = "SELECT *FROM Member WHERE MemberId=" + memberId;
    //QString sql = "SELECT *FROM Member WHERE MemberId=100004" ;
    qDebug() <<"sql:"<<sql;
    QSqlQuery query;
    query.exec(sql);
    while(query.next())
    {
        ui->Id_lineEdit->setText(memberId);
        ui->Name_lineEdit->setText(query.value(1).toString());
        ui->Phone_lineEdit->setText(query.value(2).toString());
        ui->Address_lineEdit->setText(query.value(3).toString());
        ui->Vip_comboBox->setItemText(0,query.value(4).toString());
        ui->Grade_lineEdit->setText(query.value(5).toString());
        ui->Money_lineEdit->setText(query.value(6).toString());
        ui->Regist_dateEdit->setDate(query.value(7).toDate());
        ui->Birthday_dateEdit->setDate(query.value(8).toDate());
        ui->Email_lineEdit->setText(query.value(9).toString());
    }

    ui->Id_lineEdit->setEnabled(false);
    ui->Regist_dateEdit->setEnabled(false);
    ui->Birthday_dateEdit->setEnabled(false);
    ui->Money_lineEdit->setEnabled(false);
    ui->Grade_lineEdit->setEnabled(false);
    ui->pbnGetMemberId->setEnabled(false);
}

void MemberOperateWidget::ClearMemberInfo()
{
    ui->Id_lineEdit->clear();
    ui->Name_lineEdit->clear();
    ui->Phone_lineEdit->clear();
    ui->Address_lineEdit->clear();
    ui->Grade_lineEdit->clear();
    ui->Money_lineEdit->clear();
    ui->Email_lineEdit->clear();
    ui->Remark_lineEdit->clear();

    ui->Id_lineEdit->setEnabled(true);
    ui->Regist_dateEdit->setEnabled(true);
    ui->Birthday_dateEdit->setEnabled(true);
    ui->Money_lineEdit->setEnabled(true);
    ui->Grade_lineEdit->setEnabled(true);
    ui->pbnGetMemberId->setEnabled(true);
}

void MemberOperateWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton))
    {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void MemberOperateWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void MemberOperateWidget::mouseReleaseEvent(QMouseEvent *e)
{
    mousePressed = false;
}

void MemberOperateWidget::paintEvent(QPaintEvent *)
{
    //生成一张位图
    QBitmap objBitmap(size());
    //QPainter用于在位图上绘画
    QPainter painter(&objBitmap);
    //填充位图矩形框(用白色填充)
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    //在位图上画圆角矩形(用黑色填充)
    painter.drawRoundedRect(this->rect(),10,10);
    //使用setmask过滤即可
    setMask(objBitmap);
}

void MemberOperateWidget::on_btnMenu_Close_clicked()
{
    this->close();
}

void MemberOperateWidget::on_pbnCnacel_clicked()
{
    this->close();
}

/*
 *功能：获取会员ID
*/
void MemberOperateWidget::on_pbnGetMemberId_clicked()
{
    QString member_Id = "";
    member_Id = QString::number(m_pSqlhelp->GetTableMaxId("Member","MemberId").toInt() + 1,10);
    ui->Id_lineEdit->setText(member_Id);
}

void MemberOperateWidget::on_pbnSave_clicked()
{
    if (m_pFlags)
    {
        AddMemberInfo();
    }
    else
    {
        ModifyMemberInfo();
    }

    emit signal_addMenberOk();
}

void MemberOperateWidget::on_Find_pushButton_clicked()
{
    //m_pBascWidget = new BasicDataWidget();
    m_pBascWidget->SetPbnCheck("VIP");
    m_pBascWidget->exec();
}

void MemberOperateWidget::on_pbnCardRule_clicked()
{
}

void MemberOperateWidget::on_Vip_comboBox_activated(const QString &arg1)
{
    QString sql = "SELECT MemGradeNDiscount FROM MemberGrade WHERE 1 = 1";

    sql += " AND MemGradeName = '" + arg1 + "'";
    QString DisValue = "";
    DisValue = m_pSqlhelp->GetTableColumnValue("MemberGrade",sql);
    ui->labDiscount->setText(DisValue+"折");
}

/*
 *更新VIP信息，用于接受basicdatawidget中修改完VIP信息后发送的信号
*/
void MemberOperateWidget::slot_updateVipInfo()
{
    ui->Vip_comboBox->clear();  //先清除内容，在重新添加
    //将数据绑定到下拉框
    m_pSqlhelp->BindDataToCombox("MemberGrade","MemGradeName",ui->Vip_comboBox);
}

