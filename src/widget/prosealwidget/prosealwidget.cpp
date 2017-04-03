/***************************************************************
 *Copyright(c) 2014-2015 Company By LH Unigress
 *All rights reserved.
 *文件名称：前台销售管理头文件
 *简要描述：前台销售管理，加载数据库的商品的信息，根据顾客的消费进行相应的结账操作
 *
 *当前版本：V3.0
 *作者：Kelvin Li
 *创作日期：2015/07
 *Email:creazylq@163.com
 *说明：主要用于显示顾客购买商品的细节显示，顾客购买商品后对数据库中数据进行相应的修改
******************************************************************/
#include "prosealwidget.h"
#include "ui_prosealwidget.h"
#include "constvalue.h"
#include "basicdatawidget.h"
#include "myhelper.h"
#include "sqlhelp.h"
#include "serverhelp.h"
#include "myapp.h"
#include "paymoneywidget.h"

#include <QCursor>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QDebug>
#include <QModelIndex>
#include <QKeyEvent>
#include <QSqlError>
#include <QCompleter>
#include <QStringListModel>

ProSealWidget::ProSealWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProSealWidget)
{
    ui->setupUi(this);
    this->InitForm();
    this->InitParameter();
    this->CreeateConnect();
    this->InitGoodsHeaderTitle();
    this->CompleteGoodsId();
}

ProSealWidget::~ProSealWidget()
{
    delete ui;
}


void ProSealWidget::InitForm()
{
    setMouseTracking(true);
    //设置鼠标样式
    mouseCursor = new QCursor(QPixmap(DEFAULT_CURSOR),15,10);
    this->setCursor(*mouseCursor);

    m_pBasicWidget = new BasicDataWidget();
    m_pStandModel = new QStandardItemModel();
    m_psqlhelp = new SqlHelp();

    m_pPaywidget = new PayMoneyWidget();
}


/*
 *功能：默认参数初始化
*/
void ProSealWidget::InitParameter()
{
    ui->cobUser->addItem(Myapp::CurrentUserName);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->labNewSignal->setText("新单");

    this->setFocusPolicy(Qt::StrongFocus);
    ui->letFind->setFocus();
}

/*
 *功能：实现信号和槽链接
*/
void ProSealWidget::CreeateConnect()
{
    connect(m_pPaywidget,SIGNAL(signal_paymentOk()),this,SLOT(slot_UpdateSellInfo()));
}

void ProSealWidget::InitGoodsHeaderTitle()
{
    //初始化表格列名和列宽
    ColumnNames[0] = "商品编号";
    ColumnNames[1] = "商品品牌";
    ColumnNames[2] = "商品名称";
    ColumnNames[3] = "规格型号";
    ColumnNames[4] = "颜色";
    ColumnNames[5] = "数量";
    ColumnNames[6] = "单位";
    ColumnNames[7] = "售价";
    ColumnNames[8] = "合计";

    ColumnWidths[0] = 90;
    ColumnWidths[1] = 80;
    ColumnWidths[2] = 120;
    ColumnWidths[3] = 100;
    ColumnWidths[4] = 60;
    ColumnWidths[5] = 60;
    ColumnWidths[6] = 90;
    ColumnWidths[7] = 100;
    ColumnWidths[8] = 120;

    m_psqlhelp->BindTable("Sell",ui->tableView,ColumnNames,ColumnWidths);
}

void ProSealWidget::on_pbnFindMmeber_clicked()
{
    m_pBasicWidget->SetPbnCheck("VIP");
    m_pBasicWidget->exec();
}

/*
 *功能：从数据库中查找商品信息，有--显示在tableview上，否则提示没有该商品
*/
void ProSealWidget::on_pbnAdd_clicked()
{
    QString FindGoodsId = ui->letFind->text();

    if (FindGoodsId.isEmpty())
    {
        myHelper::ShowMessageBoxError("商品编号不能为空！！");
        ui->letFind->setFocus();
    }else
    {
        if (m_psqlhelp->FindIDiSEist("Goods","GoodsId",FindGoodsId))    //库存有该商品
        {
            if(m_psqlhelp->FindIDiSEist("Sell","GoodsId",FindGoodsId))
            {
                //tablevie 已经存在该商品，增加数量处理
                this->AutoAddGoodsNum(FindGoodsId);

            }else
            {
                //直接添加商品即可
                this->AutoShowGoodsInfo(FindGoodsId);
                myHelper::Sleep(100);
                this->AutoSaveSellRecord(FindGoodsId);
            }
            this->AutoshowGoodsPrice();

        }else       //库存无该商品
        {
            myHelper::ShowMessageBoxError("没有该商品信息!请录入该商品！");
        }
    }
}

void ProSealWidget::slot_editComplete()
{
    m_pStringListModel->setStringList(idlist);
}


void ProSealWidget::slot_UpdateSellInfo()
{
    this->DeleteTableviewInfo();
    ui->letFind->clear();
}

void ProSealWidget::keyPressEvent(QKeyEvent *key)
{
    switch(key->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        this->on_pbnAdd_clicked();
        break;
    case Qt::Key_F5:
        this->on_pbnAccount_clicked();
        break;
    case Qt::Key_F2:
        this->AutoSaveSellRecord(ui->letFind->text());
        break;
    default:
        break;
    }
}

void ProSealWidget::CompleteGoodsId()
{
    QSqlQuery query;
    query.exec("SELECT GoodsId FROM Goods");
    while(query.next())
    {
        idlist << query.value(0).toString();
    }

    m_pStringListModel = new QStringListModel(idlist,this);
    m_pCompleter = new QCompleter(this);
    m_pCompleter->setCaseSensitivity(Qt::CaseSensitive);
    m_pCompleter->setModel(m_pStringListModel);
    ui->letFind->setCompleter(m_pCompleter);

    connect(ui->letFind,SIGNAL(editingFinished()),this,SLOT(slot_editComplete()));

}


/*
 *功能：自动显示商品信息
*/
void ProSealWidget::AutoShowGoodsInfo(QString goodsId)
{
    QStringList goodsInfoList;
    QSqlQuery query;
    QString sql =  "SELECT GoodsId,GoodsBrand,GoodsName,GoodsType,GoodsColor,GoodsUnits,GoodsSealPrice";
    sql += " FROM Goods WHERE GoodsId = " + goodsId;
    qDebug()<<sql;
    query.exec(sql);
    while(query.next())
    {
        goodsInfoList << query.value(0).toString() << query.value(1).toString()
                      << query.value(2).toString() << query.value(3).toString()
                      << query.value(4).toString() << query.value(5).toString()
                      << query.value(6).toString();
    }
    qDebug() <<goodsInfoList;
    myHelper::Sleep(100);

    sql = "INSERT INTO Sell(GoodsId,GoodsBrand,GoodsName,GoodsType,GoodsColor,GoodsUnit,GoodsSealPrice,";
    sql += "GoodsNum,GoodsTotalPrice) VALUES('";
    sql += goodsInfoList.at(0) + "','" + goodsInfoList.at(1) + "','" + goodsInfoList.at(2) + "','";
    sql += goodsInfoList.at(3) + "','" + goodsInfoList.at(4) + "','" + goodsInfoList.at(5) + "',";
    sql += goodsInfoList.at(6) + ", 1," + goodsInfoList.at(6) + ")" ;
    qDebug() <<sql;
    query.exec(sql);
    myHelper::Sleep(100);

    this->InitGoodsHeaderTitle();
}

/*
 *功能：自动增加商品数量
*/
void ProSealWidget::AutoAddGoodsNum(QString goodsId)
{
    int sealnum,GoodsTotalPrice;
    QSqlQuery query;
    QString sql = "SELECT GoodsNum,GoodsSealPrice FROM Sell WHERE GoodsId = '" +goodsId + "'";
    qDebug() <<"select goods num:\r\n"<<sql;
    query.exec(sql);
    while(query.next())
    {
        sealnum = query.value(0).toInt() + 1;
        GoodsTotalPrice = query.value(1).toInt() * sealnum;

        qDebug() <<"goods num:"<<sealnum;
        qDebug() <<"goods total price:" <<GoodsTotalPrice;
    }

    myHelper::Sleep(100);

    sql = "UPDATE Sell SET GoodsNum = " + QString::number((sealnum)) +",";
    sql += "GoodsTotalPrice = " + QString::number(GoodsTotalPrice);
    sql += " WHERE GoodsId = '";
    sql += goodsId +"'";
    qDebug() <<"update goods num:"<<sql;
    query.exec(sql);

    myHelper::Sleep(100);

    this->InitGoodsHeaderTitle();
}

/*
 *功能：自动保存销售记录,以便以后进行统计操作
 *说明：检索商品信息后，会自动添加信息至sell_detail表中
*/
void ProSealWidget::AutoSaveSellRecord(QString goodsId)
{
    QSqlQuery query;
    static int count = 1;
    QStringList recordlist;
    QString sellId = QDate::currentDate().toString("yyyyMMdd") + "0300";
    sellId += QString::number( m_psqlhelp->GetTableMaxId("Sell_detail","sell_id").toLong() + count );
    qDebug() << "sellId:" << sellId;
    QString userName = ui->cobUser->currentText();
    QString bargainData = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss");

    QString sql  ="SELECT GoodsId,GoodsNum,GoodsSealPrice,GoodsTotalPrice FROM Sell";
    sql += " WHERE GoodsId = " + goodsId;
    qDebug() <<"save goods record:"<<sql;
    query.exec(sql);
    while(query.next())
    {
        recordlist <<query.value(0).toString() << query.value(1).toString()
                  <<query.value(2).toString() <<query.value(3).toString();
    }

    qDebug() <<recordlist;
    myHelper::Sleep(100);

    sql = "INSERT INTO Sell_detail(sell_id,GoodsId,single_sum,bargain_price,";
    sql += "bargainTotalPrice,bargain_date,UserName)VALUES('";
    sql += sellId + "','" + recordlist.at(0) + "'," + recordlist.at(1) + ",";
    sql += recordlist.at(2) +"," + recordlist.at(3) + ",'" + bargainData +"','";
    sql += userName + "')";
    qDebug() << "insert sell recored to sell_detail:"<<sql;
    query.exec(sql);
    count++;
}

/*
 *功能：计算商品总价格并进行显示
 *说明：将计算的数目显示在对应的label上
*/
void ProSealWidget::AutoshowGoodsPrice()
{
    QSqlQuery query;
    QString sql = "SELECT SUM(GoodsNum),SUM(GoodsTotalPrice) FROM Sell";
    query.exec(sql);
    while ( query.next())
    {
        ui->labNum->setText("￥" + query.value(0).toString());
        ui->labelPrice->setText("￥" + query.value(1).toString());
        ui->labOriginalPrice->setText("￥" + query.value(1).toString());
    }
}

void ProSealWidget::on_pbnNew_clicked()
{
    ui->labNewSignal->setText("新单");
}

/*
*功能：删除商品信息
*/
void ProSealWidget::on_pbnDelete_clicked()
{
    this->DeleteTableviewInfo();
}

/*
*功能：结账
*/
void ProSealWidget::on_pbnAccount_clicked()
{
    m_pPaywidget->SetGoodsMoneyInfo(ui->labelPrice->text().mid(1));
    m_pPaywidget->exec();
}

/*
 *功能：更新tableview
 *说明：删除sell表中的信息，更新tableveiw
*/
void ProSealWidget::DeleteTableviewInfo()
{
    m_psqlhelp->DeleteTableInfo("Sell","");
    this->InitGoodsHeaderTitle();
}
