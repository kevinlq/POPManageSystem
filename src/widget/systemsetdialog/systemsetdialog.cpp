#include "systemsetdialog.h"
#include "ui_systemsetdialog.h"
#include "myhelper.h"
#include "iconhelper.h"
#include "sqlhelp.h"
#include "constvalue.h"
#include "myapp.h"

#include <QMouseEvent>

SystemSetDialog::SystemSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemSetDialog)
{
    ui->setupUi(this);

    this->InitForm();
    this->InitParamete();
}

SystemSetDialog::~SystemSetDialog()
{
    delete ui;
}


void SystemSetDialog::InitForm()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->lab_Title->setText("系统设置");
    this->setWindowIcon(QIcon(DEFAULT_ICON));

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf00a), 12);

    location = this->geometry();
    mousePressed = false;

    m_pSqlhelp = new SqlHelp();

    QList<QPushButton *>btn = ui->groupBoxButton->findChildren<QPushButton*>();
    foreach (QPushButton*b,btn)
    {
        connect(b,SIGNAL(clicked()),this,SLOT(slot_buttonClick()));
    }

//    ui->letComName->setText(Myapp::CompanyName);
//    ui->letComMan->setText(Myapp::Contacts);
//    ui->letComPhone->setText(Myapp::PhoneNumber);
//    ui->letComAddress->setText(Myapp::Address);
}

void SystemSetDialog::InitParamete()
{
    m_pVecBtn.push_back(ui->pbncompany);
    m_pVecBtn.push_back(ui->pbnvipset);
    m_pVecBtn.push_back(ui->pbnprint);
    m_pVecBtn.push_back(ui->pbndis);
    m_pVecBtn.push_back(ui->pbnany);

    for (int i =0; i < m_pVecBtn.size(); ++i)
    {
        m_pVecBtn[i]->setCheckable(true);
        m_pVecBtn[i]->setAutoExclusive(true);
    }
}

void SystemSetDialog::CreateConnect()
{
}

void SystemSetDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton))
    {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void SystemSetDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void SystemSetDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void SystemSetDialog::slot_buttonClick()
{
    //QObject::Sender()返回发送信号的对象的指针，返回类型为QObject *
    QPushButton *btn = (QPushButton *)sender();
    QString objectName = btn->objectName();
    if (objectName == "pbncompany")
    {
        m_pVecBtn.at(0)->setChecked(true);
        ui->stackedWidget->setCurrentIndex(eCompanyWidget);
    }else if(objectName == "pbnvipset")
    {
        m_pVecBtn.at(1)->setChecked(true);
        ui->stackedWidget->setCurrentIndex(eVipWidget);
    }else if (objectName == "pbnprint")
    {
        m_pVecBtn.at(2)->setChecked(true);
        ui->stackedWidget->setCurrentIndex(ePrintWidget);
    }else  if (objectName == "pbndis")
    {
        m_pVecBtn.at(3)->setChecked(true);
        ui->stackedWidget->setCurrentIndex(eDisWidget);
    }else if (objectName == "pbnany")
    {
        m_pVecBtn.at(4)->setChecked(true);
        ui->stackedWidget->setCurrentIndex(eAnyWidget);
    }
}

void SystemSetDialog::on_btnMenu_Close_clicked()
{
    this->close();
}

/**********************公司信息设置**************/
void SystemSetDialog::on_pbnSaveCompanyOk_clicked()
{
    Myapp::CompanyName = ui->letComName->text();
    Myapp::Contacts = ui->letComMan->text();
    Myapp::PhoneNumber = ui->letComPhone->text();
    Myapp::Address = ui->letComAddress->text();

    Myapp::WriteConfig();
    done(1);
    this->close();
}
