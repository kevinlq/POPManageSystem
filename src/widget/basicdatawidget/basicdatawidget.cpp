#include "basicdatawidget.h"
#include "ui_basicdatawidget.h"
#include "myhelper.h"
#include "sqlhelp.h"
#include "iconhelper.h"

#include <QMouseEvent>
#include <QSignalMapper>
#include <QDebug>
#include <QTableView>
#include <QCompleter>
#include <QStandardItemModel>

static int flag = 0;        //定义一个标志，用来区分添加和修改(1:修改，0：添加)

BasicDataWidget::BasicDataWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BasicDataWidget)
{
    ui->setupUi(this);

    this->InitForm();
    this->InitParamete();
    this->CreateConnect();
    this->ShowDataInfo();
    this->CompleteEmail();
}

BasicDataWidget::~BasicDataWidget()
{
    delete ui;
}

void BasicDataWidget::InitForm()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("基础资料");

    m_pSqlhelp = new SqlHelp();

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf00a), 12);

    location = this->geometry();
    mousePressed = false;

    QList<QPushButton *>btn = ui->gbxRight->findChildren<QPushButton *>();
    foreach (QPushButton *b,btn)
    {
        connect(b,SIGNAL(clicked()),this,SLOT(slot_buttonClick()));
    }
}

void BasicDataWidget::InitParamete()
{
    /****************设置按钮只有一个被选中****************/
    m_pVecBtn.push_back(ui->PbnBrand);
    m_pVecBtn.push_back(ui->PbnCategoriy);
    m_pVecBtn.push_back(ui->PbnColor);
    m_pVecBtn.push_back(ui->PbnUnit);
    m_pVecBtn.push_back(ui->PbnSupplier);
    m_pVecBtn.push_back(ui->PbnVIP);
    m_pVecBtn.push_back(ui->PbnAdjust);
    m_pVecBtn.push_back(ui->PbnAllocation);

    for (int i = 0; i != m_pVecBtn.size(); ++i)
    {
        m_pVecBtn[i]->setCheckable(true);
        m_pVecBtn[i]->setAutoExclusive(true);
    }

    /***************按钮默认状态初始化*******************/
    //VIP信息
    ui->letVipId->setEnabled(false);
    ui->Vip_Name_lineEdit->setEnabled(false);
    ui->Discounts_lineEdit->setEnabled(false);
}

void BasicDataWidget::CreateConnect()
{
    connect(ui->VipTableView,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_ShowVipInfo()));
    connect(ui->Unit_tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_ShowUnitInfo()));
    connect(ui->Supplier_tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_ShowSupplyInfo()));
    connect(ui->Brand_tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_ShowGoodsBrandInfo()));
}

/*
 *功能：设置某个按钮被选中，同时显示对应的界面
 *说明：外部调用。通过外部传入不同的参数进行不同的显示
*/
void BasicDataWidget::SetPbnCheck(QString name)
{
    if (name == "Brand")
    {
        m_pVecBtn.at(0)->setChecked(true);
        this->SetShowWidget(0);
    }else if (name == "类别管理")
    {
        m_pVecBtn.at(1)->setChecked(true);
        this->SetShowWidget(1);
    }else if (name == "Color")
    {
        m_pVecBtn.at(2)->setChecked(true);
        this->SetShowWidget(2);
    }else if (name == "Unit")
    {
        m_pVecBtn.at(3)->setChecked(true);
        this->SetShowWidget(3);
    }
    else if (name == "Supply")
    {
        m_pVecBtn.at(4)->setChecked(true);
        this->SetShowWidget(4);
    }else if (name == "VIP")
    {
        m_pVecBtn.at(5)->setChecked(true);
        this->SetShowWidget(5);
    }else if (name == "库存调整")
    {
        m_pVecBtn.at(6)->setChecked(true);
        this->SetShowWidget(6);
    }else if (name == "库存调拨")
    {
        m_pVecBtn.at(7)->setChecked(true);
        this->SetShowWidget(7);
    }
    qDebug() <<name;
}

void BasicDataWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton))
    {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void BasicDataWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void BasicDataWidget::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

/*
 *将数据库中的信息显示在对应的界面上
*/
void BasicDataWidget::ShowDataInfo()
{
    //商品颜色数据绑定
    columnColorNames[0] = "颜色编号";
    columnColorNames[1] = "颜色名称";
    columnColorWidths[0] = 90;
    columnColorWidths[1] = 90;
    m_pSqlhelp->BindTable("Color",ui->Color_tableView,columnColorNames,columnColorWidths);

    //商品品牌数据绑定
    columnTypeNames[0] = "品牌编号";
    columnTypeNames[1] = "品牌名称";
    columnTypeWidths[0] = 90;
    columnTypeWidths[1] = 90;
    m_pSqlhelp->BindTable("GoodsBrand",ui->Brand_tableView,columnTypeNames,columnTypeWidths);

    //供货商信息绑定
    columnSupplyNames[0] = "编号";
    columnSupplyNames[1] = "名字";
    columnSupplyNames[2] = "联系人";
    columnSupplyNames[3] = "地址";
    columnSupplyNames[4] = "电话";
    columnSupplyNames[5] = "邮箱";
    columnSupplyNames[6] = "备注";
    columnSupplyWidths[0] = 90;
    columnSupplyWidths[1] = 170;
    columnSupplyWidths[2] = 90;
    columnSupplyWidths[3] = 190;
    columnSupplyWidths[4] = 90;
    columnSupplyWidths[5] = 100;
    columnSupplyWidths[6] = 100;
    m_pSqlhelp->BindTable("Supply",ui->Supplier_tableView,columnSupplyNames,columnSupplyWidths);

    //VIP等级数据绑定
    columnVipNames[0] = "类别编号";
    columnVipNames[1] = "类别名称";
    columnVipNames[2] = "折扣";
    columnVipWidths[0] = 60;
    columnVipWidths[1] = 80;
    columnVipWidths[2] = 60;
    m_pSqlhelp->BindTable("MemberGrade",ui->VipTableView,columnVipNames,columnVipWidths);

    //商品单位数据绑定
    columnUnitNames[0] = "单位编号";
    columnUnitNames[1] = "单位名称";
    columnUnitWidths[0] = 80;
    columnUnitWidths[1] = 80;
    m_pSqlhelp->BindTable("GoodsUnit",ui->Unit_tableView,columnUnitNames,columnUnitWidths);
}

/*
 *功能：用来切换不同的界面，逻辑控制
 *说明：本文件调用
*/
void BasicDataWidget::SetShowWidget(int pageNum)
{
    if (pageNum == 0)
    {
        ui->labName->setText("品牌管理");
        ui->stackedWidget->setCurrentIndex(eBrand);
    }else if (pageNum == 1)
    {
        ui->labName->setText("类别管理");
        ui->stackedWidget->setCurrentIndex(eCategorig);
    }else if (pageNum == 2)
    {
        ui->labName->setText("颜色管理");
        ui->stackedWidget->setCurrentIndex(eColor);
    }else if (pageNum == 3)
    {
        ui->labName->setText("计量单位");
        ui->stackedWidget->setCurrentIndex(eUnit);
    }else if (pageNum == 4)
    {
        ui->labName->setText("供货商管理");
        ui->stackedWidget->setCurrentIndex(eSupplier);
    }else if (pageNum == 5)
    {
        ui->labName->setText("VIP会员类别");
        ui->stackedWidget->setCurrentIndex(eVip);
    }else if (pageNum == 6)
    {
        ui->labName->setText("库存调整类别");
        ui->stackedWidget->setCurrentIndex(eAdjust);
    }else if (pageNum == 7)
    {
        ui->labName->setText("库存调拨对象");
        ui->stackedWidget->setCurrentIndex(eAllocation);
    }
}

/*
 *功能：返回某个tableview中----0列的值
*/
QString BasicDataWidget::GetTbleviewColumn(QTableView *tableview)
{
    int row = tableview->currentIndex().row();
    QAbstractItemModel *model = tableview->model();
    QModelIndex index  = model->index(row,0);
    QString memberId = model->data(index).toString();        //得到当前选中行的第一列值
    return memberId;
}

void BasicDataWidget::CompleteEmail()
{
    m_pStandarModel = new QStandardItemModel(0,1,this);
    m_pCompleter = new QCompleter(m_pStandarModel,this);
    ui->letSupplyEmail->setCompleter(m_pCompleter);

    connect(m_pCompleter,SIGNAL(activated(const QString&)),
            this,SLOT(slot_OnEmailChoosed(const QString&)));
    connect(ui->letSupplyEmail,SIGNAL(textChanged(const QString&)),
            this,SLOT(slot_OnTextChanged(const QString&)));
}

void BasicDataWidget::on_btnMenu_Close_clicked()
{
    this->close();
}

/*
 *功能：根据所按下的按钮显示不同的界面
 *说明：判断那个按钮按下是通过判断按钮的objectname得到的
*/
void BasicDataWidget::slot_buttonClick()
{
    //QObject::Sender()返回发送信号的对象的指针，返回类型为QObject *
    QPushButton *btn = (QPushButton *)sender();
    QString objectName = btn->objectName();

    if (objectName == "PbnCategoriy")
    {
        this->SetShowWidget(1);

    }else if (objectName == "PbnColor")
    {
        this->SetShowWidget(2);

    }else if (objectName == "PbnUnit")
    {
        this->SetShowWidget(3);

    }else if (objectName == "PbnSupplier")
    {
        this->SetShowWidget(4);

    }else if (objectName == "PbnVIP")
    {
        this->SetShowWidget(5);

    }else if (objectName == "PbnAdjust")
    {
        this->SetShowWidget(6);

    }else if (objectName == "PbnAllocation")
    {
        this->SetShowWidget(7);

    }else if (objectName == "PbnBrand")
    {
        this->SetShowWidget(0);
    }
}

void BasicDataWidget::slot_ShowVipInfo()
{
    int row = ui->VipTableView->currentIndex().row();
    QAbstractItemModel *model = ui->VipTableView->model();
    QModelIndex indexId = model->index(row,0);
    QModelIndex indexName = model->index(row,1);
    QModelIndex indexType = model->index(row,2);
    ui->letVipId->setText(model->data(indexId).toString());
    ui->Vip_Name_lineEdit->setText(model->data(indexName).toString());
    ui->Discounts_lineEdit->setText(model->data(indexType).toString());
}

void BasicDataWidget::slot_ShowUnitInfo()
{
    int row = ui->Unit_tableView->currentIndex().row();
    QAbstractItemModel *model = ui->Unit_tableView->model();
    QModelIndex indexName = model->index(row,1);
    ui->letUnitName->setText(model->data(indexName).toString());
}

void BasicDataWidget::slot_ShowSupplyInfo()
{
    int row = ui->Supplier_tableView->currentIndex().row();
    QAbstractItemModel *model = ui->Supplier_tableView->model();
    QModelIndex indexName = model->index(row,1);
    QModelIndex indexContact = model->index(row,2);
    QModelIndex indexAddress = model->index(row,3);
    QModelIndex indexPhone = model->index(row,4);
    QModelIndex indexEmail = model->index(row,5);
    QModelIndex indexRemark = model->index(row,6);
    ui->letSupplyId->setText(model->data(indexName).toString());
    ui->letSupplyAddress->setText(model->data(indexAddress).toString());
    ui->letSupplyName->setText(model->data(indexContact).toString());
    ui->letSupplyPhone->setText(model->data(indexPhone).toString());
    ui->letSupplyEmail->setText(model->data(indexEmail).toString());
    ui->letSupplyRemark->setText(model->data(indexRemark).toString());
}

void BasicDataWidget::slot_ShowGoodsBrandInfo()
{
    int row = ui->Brand_tableView->currentIndex().row();
    QAbstractItemModel *model = ui->Brand_tableView->model();
    QModelIndex nameIndex = model->index(row,1);
    ui->letBrandName->setText(model->data(nameIndex).toString());
}

void BasicDataWidget::slot_OnEmailChoosed(const QString &email)
{
    ui->letSupplyEmail->clear();
    ui->letSupplyEmail->setText(email);
}

void BasicDataWidget::slot_OnTextChanged(const QString &str)
{
    if (str.contains("@"))
    {
        return;
    }

    QStringList strlist;
    strlist << "@qq.com" <<"@126.com" <<"@163.com" <<"@189.com" <<"@gmail.com";

    for (int i = 0; i < strlist.size(); ++i)
    {
        m_pStandarModel->insertRow(0);
        m_pStandarModel->setData(m_pStandarModel->index(0,0),str + strlist.at(i));
    }
}

/************************************VIP等级逻辑操作**************************************/
void BasicDataWidget::on_pbnVipAdd_clicked()
{

    QString memberId = QString::number( m_pSqlhelp->GetTableMaxId("MemberGrade","MemGradeId").toInt() + 1,10);
    ui->letVipId->setText(memberId);
    ui->letVipId->setEnabled(false);
    ui->Vip_Name_lineEdit->clear();
    ui->Discounts_lineEdit->clear();
    //ui->letVipId->setEnabled(true);
    ui->Vip_Name_lineEdit->setEnabled(true);
    ui->Discounts_lineEdit->setEnabled(true);
    ui->pbnVipAdd->setEnabled(false);
    ui->pbnVipModify->setEnabled(false);

    ui->pbnVipSave->setEnabled(true);
    ui->pbnVipCancel->setEnabled(true);
    ui->pbnVipDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnVipCancel_clicked()
{
    ui->pbnVipAdd->setEnabled(true);
    ui->pbnVipModify->setEnabled(true);
    ui->pbnVipSave->setEnabled(false);
    ui->pbnVipCancel->setEnabled(false);
    ui->pbnVipDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnVipModify_clicked()
{
    flag = 1;
    ui->letVipId->setEnabled(false);
    ui->Vip_Name_lineEdit->setEnabled(true);
    ui->Discounts_lineEdit->setEnabled(true);
    ui->pbnVipAdd->setEnabled(false);
    ui->pbnVipModify->setEnabled(false);
    ui->pbnVipSave->setEnabled(true);
    ui->pbnVipCancel->setEnabled(true);
    ui->pbnVipDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnVipSave_clicked()
{
    if (flag)   //修改
    {
        flag = 0;
        QString sql = "UPDATE MemberGrade SET MemGradeName = '" + ui->Vip_Name_lineEdit->text()+"',";
        sql += " MemGradeNDiscount = " +ui->Discounts_lineEdit->text();
        sql += " WHERE MemGradeId = " +ui->letVipId->text();
        qDebug() <<sql;
        if (m_pSqlhelp->ExecuteSql(sql))
        {
            myHelper::ShowMessageBoxInfo("修改VIP会员信息成功!");
            ShowDataInfo();
        }else
        {
            myHelper::ShowMessageBoxError("修改VIP会员信息失败!");
        }
    }else   //添加信息
    {
        //QString memberId =QString::number( m_pSqlhelp->GetTableMaxId("MemberGrade","MemGradeId").toInt() + 1,10);
        QString sql = "INSERT INTO MemberGrade VALUES("+ui->letVipId->text() +",'"+ui->Vip_Name_lineEdit->text()+"','";
        sql += ""+ui->Discounts_lineEdit->text()+"')";
        qDebug() <<sql;
        if (m_pSqlhelp->ExecuteSql(sql))
        {
            myHelper::ShowMessageBoxInfo("添加VIP会员信息成功!");
            ShowDataInfo();
        }else
        {
            myHelper::ShowMessageBoxError("添加VIP会员信息失败!");
        }
    }

    emit signal_VipInfoOk();

    ui->pbnVipAdd->setEnabled(true);
    ui->pbnVipModify->setEnabled(true);
    ui->pbnVipSave->setEnabled(false);
    ui->pbnVipCancel->setEnabled(false);
    ui->pbnVipDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnVipDelete_clicked()
{
    QString sql = " WHERE MemGradeId ='"+ui->letVipId->text()+"'";
    qDebug() <<sql;
    if (m_pSqlhelp->DeleteTableInfo("MemberGrade",sql))
    {
        myHelper::ShowMessageBoxInfo("删除VIP会员信息成功!");
        ShowDataInfo();
    }else
    {
        myHelper::ShowMessageBoxError("删除VIP会员信息失败!");
    }
    emit signal_VipInfoOk();

    ui->pbnVipAdd->setEnabled(true);
    ui->pbnVipModify->setEnabled(true);
    ui->pbnVipSave->setEnabled(false);
    ui->pbnVipCancel->setEnabled(false);
    ui->pbnVipDelete->setEnabled(true);
}

/*******************************商品单位逻辑操作*****************************************/
void BasicDataWidget::on_pbnUnitAdd_clicked()
{
    ui->letUnitName->clear();
    ui->pbnUnitAdd->setEnabled(false);
    ui->pbnUnitModiyf->setEnabled(false);
    ui->pbnUnitSave->setEnabled(true);
    ui->pbnUnitCanclel->setEnabled(true);
    ui->pbnUnitDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnUnitModiyf_clicked()
{
    flag = 1;
    ui->pbnUnitAdd->setEnabled(false);
    ui->pbnUnitModiyf->setEnabled(false);
    ui->pbnUnitSave->setEnabled(true);
    ui->pbnUnitCanclel->setEnabled(true);
    ui->pbnUnitDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnUnitSave_clicked()
{
    if (flag)
    {
        flag = 0;
        QString UnitId = GetTbleviewColumn(ui->Unit_tableView);
        QString sql = "UPDATE GoodsUnit SET UnitName='"+ui->letUnitName->text()+"'";
        sql += " WHERE UnitId = " + UnitId;
        qDebug() <<"Update Unit:"<<sql;
        if (m_pSqlhelp->ExecuteSql(sql))
        {
            myHelper::ShowMessageBoxInfo("修改商品单位信息成功!");
            ShowDataInfo();
        }else
        {
            myHelper::ShowMessageBoxError("修改商品单位信息失败!");
        }
    }else
    {
        //添加
        QString UnitId = QString::number(m_pSqlhelp->GetTableMaxId("GoodsUnit","UnitId").toInt() + 1,10);
        QString sql = "INSERT INTO GoodsUnit(UnitId,UnitName) VALUES(" + UnitId;
        sql += ",'"+ui->letUnitName->text()+"')";
        qDebug() <<"INSERT Unit:"<<sql;
        if (m_pSqlhelp->ExecuteSql(sql))
        {
            myHelper::ShowMessageBoxInfo("添加商品单位信息成功!");
            ShowDataInfo();
        }else
        {
            myHelper::ShowMessageBoxError("添加商品单位信息失败!");
        }
    }

    emit signal_UnitInfoOk();

    ui->pbnUnitAdd->setEnabled(true);
    ui->pbnUnitModiyf->setEnabled(true);
    ui->pbnUnitSave->setEnabled(false);
    ui->pbnUnitCanclel->setEnabled(false);
    ui->pbnUnitDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnUnitCanclel_clicked()
{
    ui->pbnUnitCanclel->setEnabled(false);
    ui->pbnUnitAdd->setEnabled(true);
    ui->pbnUnitModiyf->setEnabled(true);
    ui->pbnUnitSave->setEnabled(true);
    ui->pbnUnitDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnUnitDelete_clicked()
{
    QString UnitId = this->GetTbleviewColumn(ui->Unit_tableView);
    QString where = " WHERE UnitId = "+UnitId;
    qDebug() <<"Delete Unit info:"<<where;
    if (m_pSqlhelp->DeleteTableInfo("GoodsUnit",where))
    {
        myHelper::ShowMessageBoxInfo("删除商品单位信息成功!");
        ShowDataInfo();
    }else
    {
        myHelper::ShowMessageBoxError("删除商品单位信息失败!");
    }

    emit signal_UnitInfoOk();

    ui->pbnUnitAdd->setEnabled(true);
    ui->pbnUnitModiyf->setEnabled(true);
    ui->pbnUnitSave->setEnabled(false);
    ui->pbnUnitCanclel->setEnabled(false);
    ui->pbnUnitDelete->setEnabled(true);
}

/**************************供货商信息逻辑操作**********************************/
void BasicDataWidget::on_pbnAddSuupe_clicked()
{
    ui->letSupplyId->clear();
    ui->letSupplyName->clear();
    ui->letSupplyPhone->clear();
    ui->letSupplyPhone->clear();
    ui->letSupplyEmail->clear();
    ui->letSupplyRemark->clear();

    ui->pbnAddSuupe->setEnabled(false);
    ui->pbnModifySup->setEnabled(false);
    ui->pbnSaveSupply->setEnabled(true);
    ui->pbnSupplyCancel->setEnabled(true);
    ui->pbnDeleteSupply->setEnabled(false);
}

void BasicDataWidget::on_pbnModifySup_clicked()
{
    flag = 1;
    ui->pbnModifySup->setEnabled(false);
    ui->pbnAddSuupe->setEnabled(false);
    ui->pbnSaveSupply->setEnabled(true);
    ui->pbnSupplyCancel->setEnabled(true);
    ui->pbnDeleteSupply->setEnabled(true);
}

void BasicDataWidget::on_pbnSaveSupply_clicked()
{
    if (flag)
    {
        flag = 0;
        QString SupplyId = GetTbleviewColumn(ui->Supplier_tableView);
        QString sql = "UPDATE Supply SET SupplyName ='"+ui->letSupplyId->text() + "',";
        sql += " Contact = '"+ui->letSupplyName->text()+"',";
        sql += " SupplyAddress = '" + ui->letSupplyAddress->text()+"',";
        sql += " SupplyPhone = '" + ui->letSupplyPhone->text() + "',";
        sql += " SupplyEmail = '" + ui->letSupplyEmail->text() + "',";
        sql += " SupplyRemark = '" + ui->letSupplyRemark->text() + "'";
        sql += " WHERE SupplyId = " + SupplyId;
        qDebug() <<sql;

        if (m_pSqlhelp->ExecuteSql(sql))
        {
            myHelper::ShowMessageBoxInfo("修改供应商信息成功!");
            return;
        }else
        {
            myHelper::ShowMessageBoxInfo("修改供应商信息失败!");
            return;
        }
    }else
    {
        QString SupplyId = QString::number(m_pSqlhelp->GetTableMaxId("Supply","SupplyId").toInt() + 1,10);
        QString sql = "INSERT INTO Supply(SupplyId,SupplyName,Contact,SupplyAddress";
        sql += ",SupplyPhone,SupplyEmail,SupplyRemark) VALUES(" + SupplyId + ",'";
        sql += ui->letSupplyId->text() + "','";
        sql += ui->letSupplyName->text() + "','" + ui->letSupplyAddress->text() + "','";
        sql += ui->letSupplyPhone->text() + "','" + ui->letSupplyEmail->text() + "','";
        sql += ui->letSupplyRemark->text() + "')";
        qDebug() <<sql;

        if (m_pSqlhelp->ExecuteSql(sql))
        {
            myHelper::ShowMessageBoxInfo("添加信息成功!");
        }else
        {
            myHelper::ShowMessageBoxInfo("添加信息失败");
        }
    }
    ShowDataInfo();
    emit signal_SupplyInfoOk();

    ui->pbnSaveSupply->setEnabled(false);
    ui->pbnSupplyCancel->setEnabled(false);
    ui->pbnModifySup->setEnabled(true);
    ui->pbnAddSuupe->setEnabled(true);
    ui->pbnDeleteSupply->setEnabled(true);
}

void BasicDataWidget::on_pbnSupplyCancel_clicked()
{
    ui->pbnAddSuupe->setEnabled(true);
    ui->pbnModifySup->setEnabled(true);
    ui->pbnSaveSupply->setEnabled(true);
    ui->pbnSupplyCancel->setEnabled(false);
    ui->pbnDeleteSupply->setEnabled(true);
}

void BasicDataWidget::on_pbnDeleteSupply_clicked()
{
    QString SupplyId = this->GetTbleviewColumn(ui->Supplier_tableView);
    QString where = " WHERE  SupplyId = " + SupplyId;

    qDebug() <<"Delete Supply info:";

    if (myHelper::ShowMessageBoxQuesion("你确定要删除该条记录吗？"))
    {
        if (m_pSqlhelp->DeleteTableInfo("Supply",where))
        {
            myHelper::ShowMessageBoxInfo("删除成功!");
        }else
        {
            myHelper::ShowMessageBoxError("删除失败!");
        }
    }else
    {
        return;
    }

    ShowDataInfo();
    emit signal_SupplyInfoOk();

    ui->pbnAddSuupe->setEnabled(true);
    ui->pbnModifySup->setEnabled(true);
    ui->pbnSaveSupply->setEnabled(false);
    ui->pbnSupplyCancel->setEnabled(false);
    ui->pbnDeleteSupply->setEnabled(true);
}

/*****************************商品品牌逻辑操作**********************************/
void BasicDataWidget::on_pbnBrandAdd_clicked()
{
    ui->letBrandName->clear();

    ui->pbnBrandAdd->setEnabled(false);
    ui->pbnBrandModify->setEnabled(false);
    ui->pbnBrandSave->setEnabled(true);
    ui->pbnBrandCancel->setEnabled(true);
    ui->pbnBrandDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnBrandModify_clicked()
{
    flag = 1;

    ui->pbnBrandModify->setEnabled(false);
    ui->pbnBrandAdd->setEnabled(false);
    ui->pbnBrandSave->setEnabled(true);
    ui->pbnBrandCancel->setEnabled(true);
    ui->pbnBrandDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnBrandSave_clicked()
{
    if (flag)
    {
        flag = 0;
        QString goodsBrandId = GetTbleviewColumn(ui->Brand_tableView);
        QString sql = "UPDATE GoodsBrand SET BrandName = '" + ui->letBrandName->text()+"'";
        sql += " WHERE BrandId = '" + goodsBrandId+"'";
        qDebug() <<"Update GoodsBrand info:";
        qDebug() <<sql;

        if (m_pSqlhelp->ExecuteSql(sql))
        {
            myHelper::ShowMessageBoxInfo("修改商品品牌信息成功!");
            return;
        }else
        {
            myHelper::ShowMessageBoxError("修改信息失败!");
            return;
        }
    }else
    {
        QString GoodsBrandId = QString::number(m_pSqlhelp->GetTableMaxId("GoodsBrand","BrandId").toInt() + 1);
        QString sql = "INSERT INTO GoodsBrand(BrandId,BrandName) VALUES(";
        sql += "'" + GoodsBrandId+"','" + ui->letBrandName->text()+"')";
        qDebug() <<"INSERT GoodsBrand info:";
        qDebug() <<sql;

        if (m_pSqlhelp->ExecuteSql(sql))
        {
            myHelper::ShowMessageBoxInfo("添加信息成功!");
        }else
        {
            myHelper::ShowMessageBoxError("添加信息失败");
        }
    }
     ShowDataInfo();
     emit signal_GoodsBrandInfoOk();

     ui->pbnBrandSave->setEnabled(false);
     ui->pbnBrandCancel->setEnabled(false);
     ui->pbnBrandModify->setEnabled(true);
     ui->pbnBrandAdd->setEnabled(true);
     ui->pbnBrandDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnBrandCancel_clicked()
{
    ui->pbnBrandCancel->setEnabled(false);
    ui->pbnBrandAdd->setEnabled(true);
    ui->pbnBrandModify->setEnabled(true);
    ui->pbnBrandSave->setEnabled(false);
    ui->pbnBrandDelete->setEnabled(true);
}

void BasicDataWidget::on_pbnBrandDelete_clicked()
{
    QString brandId = this->GetTbleviewColumn(ui->Brand_tableView);
    QString where = " WHERE BrandId = '" + brandId+"'";
    qDebug() <<"Delete GoodsBrand info:";
    if (myHelper::ShowMessageBoxQuesion("你确定要删除该条记录吗？"))
    {
        if (m_pSqlhelp->DeleteTableInfo("GoodsBrand",where))
        {
            myHelper::ShowMessageBoxInfo("删除成功!");
        }else
        {
            myHelper::ShowMessageBoxError("删除失败!");
        }
    }else
    {
        return;
    }

    ShowDataInfo();
    emit signal_GoodsBrandInfoOk();

    ui->pbnBrandCancel->setEnabled(false);
    ui->pbnBrandAdd->setEnabled(true);
    ui->pbnBrandModify->setEnabled(true);
    ui->pbnBrandSave->setEnabled(true);
    ui->pbnBrandModify->setEnabled(true);
}
