#include "stockmanagewidget.h"
#include "ui_stockmanagewidget.h"
#include "sqlhelp.h"
#include "myhelper.h"
#include <QDebug>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QModelIndex>

StockManageWidget::StockManageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockManageWidget)
{
    ui->setupUi(this);

    m_pSqlhelp = new SqlHelp();
    this->InitForm();
    this->InitWidget();
    this->InitParameter();
}

StockManageWidget::~StockManageWidget()
{
    delete ui;
}

void StockManageWidget::InitForm()
{
    //初始化表格列名和列宽
    ColumnNames[0] = "商品条码";
    ColumnNames[1] = "类别";
    ColumnNames[2] = "品牌";
    ColumnNames[3] = "商品名称";
    ColumnNames[4] = "规格型号";
    ColumnNames[5] = "颜色";
    ColumnNames[6] = "数量";
    ColumnNames[7] = "单位";
    ColumnNames[8] = "销售价";
    ColumnNames[9] = "合计";
    ColumnNames[10] = "进价";
    ColumnNames[11] = "总计";
    ColumnNames[12] = "进货日期";

    ColumnWidths[0] = 90;
    ColumnWidths[1] = 80;
    ColumnWidths[2] = 90;
    ColumnWidths[3] = 120;
    ColumnWidths[4] = 90;
    ColumnWidths[5] = 70;
    ColumnWidths[6] = 70;
    ColumnWidths[7] = 80;
    ColumnWidths[8] = 80;
    ColumnWidths[9] = 80;
    ColumnWidths[10] = 80;
    ColumnWidths[11] = 100;
    ColumnWidths[12] = 100;

    //设置需要显示数据的表格和翻页的按钮
    m_pSqlhelp->SetControlTable(ui->tableView,ui->labInfo,ui->btnFirst,ui->btnPre,ui->btnNext,ui->btnLast);
    //加载初始数据,按正序显示，("desc"倒序)
    m_pSqlhelp->BindData("Goods","GoodsId","",13,ColumnNames,ColumnWidths);
}

void StockManageWidget::InitWidget()
{
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabel("商品类别信息");

    QTreeWidgetItem *allNote = new QTreeWidgetItem(QStringList("全部类别"));
    //追加顶层树节点
    ui->treeWidget->addTopLevelItem(allNote);
    ui->treeWidget->expand(ui->treeWidget->model()->index(1,0));

    //类别节点
    QTreeWidgetItem *smoke = new QTreeWidgetItem(QStringList("烟"));
    QTreeWidgetItem *liquor = new QTreeWidgetItem(QStringList("酒"));
    QTreeWidgetItem *DailyChemicalCaps = new QTreeWidgetItem(QStringList("日化"));

    //类别详细节点
    QTreeWidgetItem *ChineseCigarettes = new QTreeWidgetItem(QStringList("中华"));

    QTreeWidgetItem *soap = new QTreeWidgetItem(QStringList("香皂"));
    QTreeWidgetItem *TransSoap = new QTreeWidgetItem(QStringList("透明皂"));
    QTreeWidgetItem *detergent = new QTreeWidgetItem(QStringList("洗衣粉"));

    QTreeWidgetItem *beer = new QTreeWidgetItem(QStringList("啤酒"));
    QTreeWidgetItem *samshu = new QTreeWidgetItem(QStringList("白酒"));
    QTreeWidgetItem *wine = new QTreeWidgetItem(QStringList("白酒"));

    //添加到子节点中
    allNote->addChild(smoke);
    allNote->addChild(liquor);
    allNote->addChild(DailyChemicalCaps);

    //添加子节点的孩子
    smoke->addChild(ChineseCigarettes);
    liquor->addChild(beer);
    liquor->addChild(samshu);
    liquor->addChild(wine);
    DailyChemicalCaps->addChild(soap);
    DailyChemicalCaps->addChild(TransSoap);
    DailyChemicalCaps->addChild(detergent);

    connect(ui->treeWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_treeClicked(QModelIndex)));
}

void StockManageWidget::InitParameter()
{
    //没有实现
    //    m_pMyhelp = new myHelper;
    //    m_pMyhelp->SetCompleteEmail(ui->ledGoodsId);
}

void StockManageWidget::slot_treeClicked(const QModelIndex &index)
{
    QString childNodeName = ui->treeWidget->currentItem()->text(0);
    QString sql = "SELECT *FROM Goods WHERE 1 = 1";

    if (childNodeName == "全部类别")
    {
        sql +="";
    }else if (childNodeName == "烟" ||childNodeName == "酒" ||
              childNodeName == "日化")
    {
        sql +=" AND GoodsCategories = '" + childNodeName+"'";
    }

    m_pSqlhelp->BindTableWhere(sql,ui->tableView,ColumnNames,ColumnWidths);
//    qDebug() <<childNodeName<<","<<index;
}

/*
 *功能：根据条件进行筛选--显示
*/
void StockManageWidget::on_pbnFind_clicked()
{
    QString sql = "SELECT * FROM Goods WHERE 1 = 1";
    if (ui->letContent->text().isEmpty())
    {
        myHelper::ShowMessageBoxError("请输入要检索的内容!!");
    }else
    {
        //检索顺序：按下拉框索引"商品条码","商品名称 ","商品规格","商品颜色"
        if (ui->cbxSift->currentIndex() == 0)
        {
            sql += " AND GoodsId = " + ui->letContent->text();
        }else if (ui->cbxSift->currentIndex() == 1)
        {
            sql += " AND GoodsName = '" + ui->letContent->text() + "'";
        }else if (ui->cbxSift->currentIndex() == 2)
        {
            sql += " AND Goods = '" + ui->letContent->text() + "'";
        }else if (ui->cbxSift->currentIndex() == 3)
        {
            sql += " AND GoodsColor = '" + ui->letContent->text() +"'";
        }
        qDebug() <<"SELECT Goods info:"<<sql;
        m_pSqlhelp->BindTableWhere(sql,ui->tableView,ColumnNames,ColumnWidths);
    }
}

void StockManageWidget::on_pbnShowAll_clicked()
{
    this->InitForm();
}

void StockManageWidget::on_pbnModify_clicked()
{
    if (ui->tableView->currentIndex().row() > 0)
    {

    }else
    {
        myHelper::ShowMessageBoxError("请选选中要修改的商品信息!");
    }
}
