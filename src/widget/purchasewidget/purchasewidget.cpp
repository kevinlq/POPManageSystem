#include "purchasewidget.h"
#include "ui_purchasewidget.h"
#include "sqlhelp.h"
#include "myhelper.h"
#include "basicdatawidget.h"
#include <QDebug>
#include <QStandardItemModel>
#include <QCompleter>
#include <QSqlQuery>
#include <QSqlError>

static int IdLocation = 0;
bool flag = false;

PurchaseWidget::PurchaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PurchaseWidget)
{
    ui->setupUi(this);

    this->InitForm();
    this->InitGoodsInfo();
    this->InitParamete();
    this->CreateConnect();
}

PurchaseWidget::~PurchaseWidget()
{
    delete ui;
}

void PurchaseWidget::InitForm()
{
    m_pSqlhelp = new SqlHelp();
    m_pStandModel = new QStandardItemModel(this);
    m_pBascWidget = new BasicDataWidget();
}

void PurchaseWidget::InitParamete()
{
    ui->dateEdit->setDate(QDate::currentDate());

    m_pSqlhelp->BindDataToCombox("Supply","SupplyName",ui->Supplier_comboBox);  //供货商名称绑定
    m_pSqlhelp->BindDataToCombox("Color","ColorName",ui->Goods_Color_comboBox); //商品颜色绑定
    m_pSqlhelp->BindDataToCombox("GoodsUnit","UnitName",ui->Goods_Unit_comboBox);//商品单位绑定
    m_pSqlhelp->BindDataToCombox("GoodsBrand","BrandName",ui->Goods_Brand_comboBox);//品牌绑定
    m_pSqlhelp->BindDataToCombox("GoodsType","TypeName",ui->Goods_Categories_comboBox);//商品类别绑定

    ui->Check_pushButton->setEnabled(false);        //审核按钮不可用
    ui->Delete_pushButton->setEnabled(false);       //删除按钮不可用
    ui->labTip->setText("新单");
}

void PurchaseWidget::CreateConnect()
{
    connect(m_pBascWidget,SIGNAL(signal_UnitInfoOk()),this,SLOT(slot_UpdateUnitIfno()));
    connect(m_pBascWidget,SIGNAL(signal_SupplyInfoOk()),this,SLOT(slot_UpdateSupplyInfo()));
    connect(m_pBascWidget,SIGNAL(signal_GoodsBrandInfoOk()),
            this,SLOT(slot_UpdateGoodsBandInfo()));
}

/*
 *功能：初始化商品信息
*/
void PurchaseWidget::InitGoodsInfo()
{
    m_pStandModel->setHorizontalHeaderItem(0,new QStandardItem("商品编号"));
    m_pStandModel->setHorizontalHeaderItem(1,new QStandardItem("商品类别"));
    m_pStandModel->setHorizontalHeaderItem(2,new QStandardItem("品牌"));
    m_pStandModel->setHorizontalHeaderItem(3,new QStandardItem("商品名称"));
    m_pStandModel->setHorizontalHeaderItem(4,new QStandardItem("规格型号"));
    m_pStandModel->setHorizontalHeaderItem(5,new QStandardItem("颜色"));
    m_pStandModel->setHorizontalHeaderItem(6,new QStandardItem("数量"));
    m_pStandModel->setHorizontalHeaderItem(7,new QStandardItem("单位"));
    m_pStandModel->setHorizontalHeaderItem(8,new QStandardItem("售价"));
    m_pStandModel->setHorizontalHeaderItem(9,new QStandardItem("合计"));
    m_pStandModel->setHorizontalHeaderItem(10,new QStandardItem("进价"));
    m_pStandModel->setHorizontalHeaderItem(11,new QStandardItem("总计"));

    ui->tableView->setModel(m_pStandModel);
}

bool PurchaseWidget::GetGoodsId(QString& id)
{
    QVariantList idlist;
    for (int i =0; i < m_pStandModel->rowCount(); ++i)
    {
        idlist <<m_pStandModel->index(i,0).data().toString();

        if(idlist.contains(id))
            IdLocation = i;
        return true;
    }
    return false;
}

void PurchaseWidget::ClearTableview()
{
    m_pStandModel->removeRows(0,ui->tableView->currentIndex().row());
}

void PurchaseWidget::slot_UpdateUnitIfno()
{
    ui->Goods_Unit_comboBox->clear();
    m_pSqlhelp->BindDataToCombox("GoodsUnit","UnitName",ui->Goods_Unit_comboBox);
}

void PurchaseWidget::slot_UpdateSupplyInfo()
{
    ui->Supplier_comboBox->clear();
    m_pSqlhelp->BindDataToCombox("Supply","SupplyName",ui->Supplier_comboBox);  //供货商名称绑定
}

void PurchaseWidget::slot_UpdateGoodsBandInfo()
{
    ui->Goods_Brand_comboBox->clear();
     m_pSqlhelp->BindDataToCombox("GoodsBrand","BrandName",ui->Goods_Brand_comboBox);//品牌绑定
}

void PurchaseWidget::on_tbnFindSupply_clicked()
{
    //m_pBascWidget = new BasicDataWidget();
    m_pBascWidget->SetPbnCheck("Supply");
    m_pBascWidget->exec();
}

void PurchaseWidget::on_pbnFindColor_clicked()
{
    //m_pBascWidget = new BasicDataWidget();
    m_pBascWidget->SetPbnCheck("Color");
    m_pBascWidget->exec();
}

void PurchaseWidget::on_pbnFindUnit_clicked()
{
    //m_pBascWidget = new BasicDataWidget();
    m_pBascWidget->SetPbnCheck("Unit");
    m_pBascWidget->exec();
}

/*
 *功能：实现商品入库(先加入到tableview中)
*/
void PurchaseWidget::on_pbnAddGoodsOk_clicked()
{
    static int columnCount = 0;
    QString goods_id = ui->Goods_Id_lineEdit->text();                        /*商品编号*/
    QString goods_gategories = ui->Goods_Categories_comboBox->currentText(); /*商品类别*/
    QString goods_brand = ui->Goods_Brand_comboBox->currentText();           /*商品品牌*/
    QString goods_name = ui->Goods_Name_lineEdit->text();                    /*商品名字*/
    QString goods_tpye = ui->Goods_Type_comboBox->currentText();             /*商品规格型号*/
    QString goods_color = ui->Goods_Color_comboBox->currentText();           /*商品颜色*/
    int goods_seal_price = ui->Goods_Seal_Price_lineEdit->text().toFloat();  /*商品售价*/
    int goods_num = ui->Goods_Num_lineEdit->text().toFloat();                /*商品数量*/
    QString goods_units = ui->Goods_Unit_comboBox->currentText();            /*商品单位*/
    int goods_bid_price = ui->Goods_Bid_Price_lineEdit->text().toFloat();    /*商品进价*/
    qint32 goods_sum = goods_num * goods_seal_price;
    qint32 goods_total = goods_num * goods_bid_price;

    if (goods_id.isEmpty() || goods_name.isEmpty())
    {
        myHelper::ShowMessageBoxError("请填商品信息!");
    }else
    {
        QVariantList idlist;
        for (int i =0; i < m_pStandModel->rowCount(); ++i)
        {
            idlist <<m_pStandModel->index(i,0).data().toString();

        }
        if(idlist.contains(goods_id))
        {
            IdLocation = idlist.indexOf(goods_id);
            qDebug() <<"same";
            qDebug() <<idlist.indexOf(goods_id);
            int num = m_pStandModel->index(IdLocation,6).data().toInt();
            int sumOld = m_pStandModel->index(IdLocation,9).data().toInt();
            int totalOld = m_pStandModel->index(IdLocation,11).data().toInt();
            m_pStandModel->setItem(IdLocation,6,new QStandardItem(QString::number((num + goods_num),10)));
            m_pStandModel->setItem(IdLocation,9,new QStandardItem(QString::number((sumOld + goods_sum),10)));
            m_pStandModel->setItem(IdLocation,11,new QStandardItem(QString::number((totalOld + goods_total),10)));
        }
        else
        {
            m_pStandModel->setItem(columnCount,0,new QStandardItem(goods_id));
            m_pStandModel->setItem(columnCount,1,new QStandardItem(goods_gategories));
            m_pStandModel->setItem(columnCount,2,new QStandardItem(goods_brand));
            m_pStandModel->setItem(columnCount,3,new QStandardItem(goods_name));
            m_pStandModel->setItem(columnCount,4,new QStandardItem(goods_tpye));
            m_pStandModel->setItem(columnCount,5,new QStandardItem(goods_color));
            m_pStandModel->setItem(columnCount,6,new QStandardItem(QString::number(goods_num,10)));
            m_pStandModel->setItem(columnCount,7,new QStandardItem(goods_units));
            m_pStandModel->setItem(columnCount,8,new QStandardItem(QString::number(goods_seal_price,10)));
            m_pStandModel->setItem(columnCount,9,new QStandardItem(QString::number(goods_sum,10)));
            m_pStandModel->setItem(columnCount,10,new QStandardItem(QString::number(goods_bid_price,10)));
            m_pStandModel->setItem(columnCount,11,new QStandardItem(QString::number(goods_total,10)));
            columnCount++;
        }
    }
}

void PurchaseWidget::on_pbnNewSignal_clicked()
{
    if (flag || (m_pStandModel->rowCount() < 1 ))
    {
        ui->labTip->setText("新单");
        m_pStandModel->clear();
        //ClearTableview();
        this->InitGoodsInfo();
    }else
    {
        myHelper::ShowMessageBoxError("商品尚未入库，请先保存后在添加新单!");
    }
}

/*
 *将tableview中的商品进保存操作
*/
void PurchaseWidget::on_Save_pushButton_clicked()
{
    if (m_pStandModel->rowCount() < 1)
    {
        myHelper::ShowMessageBoxError("操作失败，没有商品信息!");
    }else
    {
        myHelper::ShowMessageBoxInfo("保存成功，需要审核后才会正式更新库存!");
        ui->Delete_pushButton->setEnabled(true);
        ui->Check_pushButton->setEnabled(true);
    }
}

void PurchaseWidget::on_Delete_pushButton_clicked()
{
    if (myHelper::ShowMessageBoxQuesion("你确定要删除进货单吗？删除后不可恢复？"))
    {
        m_pStandModel->clear();
        this->InitGoodsInfo();
        qDebug() <<"Delete Order form OK!";
    }else
    {
        return;
    }
}

/*
 *功能：商品信息入库，入库后无法直接修改
*/
void PurchaseWidget::on_Check_pushButton_clicked()
{
    int column = m_pStandModel->columnCount();
    int row = m_pStandModel->rowCount();

    qDebug() <<"row:"<<row<<"column:"<<column;

    for (int i = 0; i < row; ++i)
    {
        QVariantList idList,gategoriesList,brandList,nameList,typeList,
                colorList,numList,unitList,selPriceList,sumList,bidList,totalList;

        idList <<m_pStandModel->index(i,0).data().toInt();
        gategoriesList <<m_pStandModel->index(i,1).data().toString();
        brandList <<m_pStandModel->index(i,2).data().toString();
        nameList <<m_pStandModel->index(i,3).data().toString();
        typeList <<m_pStandModel->index(i,4).data().toString();
        colorList <<m_pStandModel->index(i,5).data().toString();
        numList <<m_pStandModel->index(i,6).data().toString();
        unitList<<m_pStandModel->index(i,7).data().toString();
        selPriceList <<m_pStandModel->index(i,8).data().toInt();
        sumList <<m_pStandModel->index(i,9).data().toInt();
        bidList <<m_pStandModel->index(i,10).data().toInt();
        totalList <<m_pStandModel->index(i,11).data().toInt();

        QSqlQuery query;
        QString sql ="INSERT INTO Goods(GoodsId,GoodsCategories,GoodsBrand,";
        sql += "GoodsName,GoodsType,GoodsColor,GoodsNum,GoodsUnits,GoodsSealPrice,";
        sql += "GoodsSum,GoodsBidPrice,GoodsTotal)";
        sql += " VALUES(?,?,?,?,?,?,?,?,?,?,?,?)";

        query.prepare(sql);
        query.addBindValue(idList);
        query.addBindValue(gategoriesList);
        query.addBindValue(brandList);
        query.addBindValue(nameList);
        query.addBindValue(typeList);
        query.addBindValue(colorList);
        query.addBindValue(numList);
        query.addBindValue(unitList);
        query.addBindValue(selPriceList);
        query.addBindValue(sumList);
        query.addBindValue(bidList);
        query.addBindValue(totalList);

        if (!query.execBatch())
        {
            myHelper::ShowMessageBoxError(query.lastError().text());
        }else
        {
            myHelper::ShowMessageBoxInfo("商品入库成功!如需继续填写进货单，请点击左上角【新单】");
            flag = true;
            ui->Save_pushButton->setEnabled(false);
            ui->Check_pushButton->setEnabled(false);
            ui->Delete_pushButton->setEnabled(false);
        }
    }
}

void PurchaseWidget::on_Tutor_pushButton_clicked()
{
    qDebug() <<"row"<<m_pStandModel->rowCount();
    qDebug() <<"column"<< m_pStandModel->columnCount();
}

/*
 *功能：查找品牌(Brand)管理
**/
void PurchaseWidget::on_Find_Brand_pushButton_clicked()
{
    m_pBascWidget->SetPbnCheck("Brand");
    m_pBascWidget->exec();
}
