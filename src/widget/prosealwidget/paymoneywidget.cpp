/***************************************************************
 *Copyright(c) 2014-2015 Company By LH Unigress
 *All rights reserved.
 *文件名称：付款管理文件
 *简要描述：计算顾客购买商品的价格，以及给顾客的找零操作
 *
 *当前版本：V2.0
 *作者：Kelvin Li
 *创作日期：2014/10
 *说明：主要用于显示顾客购买商品的总额，进行消费显示和计算，将销售记录写进销售记录数据表中
 **************************************************************************
 *更新说明：
 *当前版本：V3.0
 *修改时间：2015-8-4 17:04:37
******************************************************************/
#include "paymoneywidget.h"
#include "ui_paymoneywidget.h"
#include "iconhelper.h"
#include "myhelper.h"

#include <QMouseEvent>

PayMoneyWidget::PayMoneyWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PayMoneyWidget)
{
    ui->setupUi(this);
    this->InitForm();
    this->InitParameter();
}

PayMoneyWidget::~PayMoneyWidget()
{
    delete ui;
}

void PayMoneyWidget::InitForm()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("付款结算");
    ui->lab_Title->setText("付款结算");

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf00a), 12);

    location = this->geometry();
    mousePressed = false;
}

void PayMoneyWidget::InitParameter()
{
    ui->radioButtonCash->setChecked(true);

    ui->lineEditOriginalPrice->setEnabled(false);
    ui->lineEditFavorable->setEnabled(false);
    ui->lineEditCash->setEnabled(true);
    ui->lineEditBackCard->setEnabled(false);
    ui->lineEditCredit->setEnabled(false);
    ui->lineEditDiscount->setEnabled(false);

    ui->lineEditFavorable->setText("0.00");
}

void PayMoneyWidget::SetGoodsMoneyInfo(QString money)
{
    ui->lineEditShuoldPay->setText(money);
    ui->lineEditOriginalPrice->setText(money);
    ui->lineEditCash->setText(money);
}

void PayMoneyWidget::mouseMoveEvent(QMouseEvent *e )
{
    if (mousePressed && (e->buttons() && Qt::LeftButton))
    {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void PayMoneyWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}


void PayMoneyWidget::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void PayMoneyWidget::on_btnMenu_Close_clicked()
{
    this->close();
}

/*
 *结账计算
*/
void PayMoneyWidget::on_pbnOk_clicked()
{
    QString actually = ui->lineEditActuallyPay->text();
    QString should = ui->lineEditShuoldPay->text();
    if (actually.toFloat() < should.toFloat())
    {
        myHelper::ShowMessageBoxError("实际收款【"+actually + "】小于应收款【" + should+"】，不允许结账!");
    }else
    {
        emit signal_paymentOk();
        this->close();
    }
}

void PayMoneyWidget::on_radioButtonCash_clicked()
{
    ui->lineEditCash->setEnabled(true);
    ui->lineEditBackCard->setEnabled(false);
    ui->lineEditCredit->setEnabled(false);
    ui->lineEditDiscount->setEnabled(false);
}

void PayMoneyWidget::on_radioButtonDwping_clicked()
{
    ui->lineEditBackCard->setEnabled(true);
    ui->lineEditCredit->setEnabled(false);
    ui->lineEditDiscount->setEnabled(false);
    ui->lineEditCash->setEnabled(false);
}

void PayMoneyWidget::on_radioButtonKind_clicked()
{
    ui->lineEditBackCard->setEnabled(true);
    ui->lineEditCredit->setEnabled(false);
    ui->lineEditDiscount->setEnabled(true);
    ui->lineEditCash->setEnabled(true);
}

/*
 *进行零钱数目的计算
*/
void PayMoneyWidget::on_lineEditCash_textChanged(const QString &arg1)
{
    ui->lineEditActuallyPay->setText(ui->lineEditCash->text());

    //进行计算
    qreal cash = ui->lineEditCash->text().toFloat();
    qreal changemoney = cash - ui->lineEditShuoldPay->text().toFloat();
    ui->lineEditChange->setText(QString::number(changemoney));
}
