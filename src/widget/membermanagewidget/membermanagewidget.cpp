#include "membermanagewidget.h"
#include "ui_membermanagewidget.h"
#include "sqlhelp.h"
#include "myhelper.h"
#include <QSqlQueryModel>
#include "mycheckboxtablemodel.h"
#include "memberoperatewidget.h"
#include "exportexcelobject.h"

#include <QDebug>
#include <QCompleter>
#include <QStandardItemModel>
#include <QToolTip>
#include <QModelIndex>
#include <QFileDialog>

MemberManageWidget::MemberManageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemberManageWidget)
{
    ui->setupUi(this);

    m_pSqlhelp = new SqlHelp();//实例化数据库操作对象，以备以后调用
    this->InitForm();
    this->InitParameters();
    this->CreateConnect();
}

MemberManageWidget::~MemberManageWidget()
{
    delete ui;
}

void MemberManageWidget::InitForm()
{
    //初始化表格列名和列宽
    ColumnNames[0] = "会员编号";
    ColumnNames[1] = "姓名";
    ColumnNames[2] = "手机号码";
    ColumnNames[3] = "地址";
    ColumnNames[4] = "等级";
    ColumnNames[5] = "积分";
    ColumnNames[6] = "消费金额";
    ColumnNames[7] = "注册日期";
    ColumnNames[8] = "生日";
    ColumnNames[9] = "邮箱";
    ColumnNames[10] = "备注";

    ColumnWidths[0] = 90;
    ColumnWidths[1] = 70;
    ColumnWidths[2] = 100;
    ColumnWidths[3] = 280;
    ColumnWidths[4] = 80;
    ColumnWidths[5] = 70;
    ColumnWidths[6] = 80;
    ColumnWidths[7] = 120;
    ColumnWidths[8] = 120;
    ColumnWidths[9] = 160;
    ColumnWidths[10] = 180;

    //设置需要显示数据的表格和翻页的按钮
    m_pSqlhelp->SetControlTable(ui->tableView,ui->labInfo,ui->btnFirst,ui->btnPre,ui->btnNext,ui->btnLast);
    //加载初始数据,按编号正序显示
    m_pSqlhelp->BindData("Member","MemberId","",12,ColumnNames,ColumnWidths);
}

/*
 *程序默认参数初始化
*/
void MemberManageWidget::InitParameters()
{
    this->SetDataHide();

    this->setMouseTracking(true);

    ui->dateEdit_From->setDate(QDate::currentDate());
    ui->dateEdit_To->setDate(QDate::currentDate());
}

void MemberManageWidget::CreateConnect()
{
    connect(ui->Choose_comboBox,SIGNAL(activated(int)),
            this,SLOT(slot_ShowControl(int)));

    m_pMemberOperate = new MemberOperateWidget();
    connect(m_pMemberOperate,SIGNAL(signal_addMenberOk()),
            this,SLOT(slot_RefrechInfo()));

    //connect(ui->tableView,SIGNAL(activated(QModelIndex))
    //connect(ui->tableView,SIGNAL(entered(QModelIndex)),
    // this,SLOT(slot_ShowToolTip(QModelIndex)));
}

/*
 *功能：实现自动补全电子邮箱
*/
void MemberManageWidget::CompleteEmail()
{
    m_pStandarModel = new QStandardItemModel(0,1,this);
    m_pCompleter = new QCompleter(m_pStandarModel,this);
    ui->FindlineEdit->setCompleter(m_pCompleter);

    connect(m_pCompleter,SIGNAL(activated(const QString&)),
            this,SLOT(slot_OnEmailChoosed(const QString&)));
    connect(ui->FindlineEdit,SIGNAL(textChanged(const QString&)),
            this,SLOT(slot_OnTextChanged(const QString&)));
}

/*
 *功能：获取当前选中行的会员编号
*/
QString MemberManageWidget::GetTableviewColumn()
{
    int row = ui->tableView->currentIndex().row();          //获取当前选中的行号
    QAbstractItemModel *model = ui->tableView->model();
    QModelIndex index  = model->index(row,0);
    QString memberId = model->data(index).toString();        //得到当前选中行的第一列值
    return memberId;
}

/*
 *添加会员信息
*/
void MemberManageWidget::on_pbnAddMem_clicked()
{
    //m_pMemberOperate = new MemberOperateWidget();
    QString memberId = GetTableviewColumn();
    m_pMemberOperate->SetTitleInfo("VIP会员登记",1,memberId);
    m_pMemberOperate->exec();
}

/*
 *修改会员信息
*/
void MemberManageWidget::on_Modify_pushButton_clicked()
{
    //m_pMemberOperate = new MemberOperateWidget();
    if (ui->tableView->currentIndex().row() >= 0)
    {
        QString memberId = GetTableviewColumn();
        m_pMemberOperate->SetTitleInfo("VIP会员修改",0,memberId);
        m_pMemberOperate->exec();
    }
    else
    {
        myHelper::ShowMessageBoxError("请先选中要修改的VIP会员！");
        return;
    }

}

void MemberManageWidget::slot_ShowControl(const int index)
{
    if (index == 4)
    {
        SetDataHide();
        CompleteEmail();
    }
    else if(index == 5 || index == 6)
    {
        SetDataShow();
    }
    else
    {
        SetDataHide();
        ui->FindlineEdit->clear();
    }
}

void MemberManageWidget::slot_OnEmailChoosed(const QString &email)
{
    ui->FindlineEdit->clear();
    ui->FindlineEdit->setText(email);
}

void MemberManageWidget::slot_OnTextChanged(const QString &str)
{
    if (str.contains("@"))
    {
        return;
    }

    QStringList strlist;
    strlist <<"@qq.com" <<"@163.com" <<"@189.com" <<"@gmail.com" <<"@126.com";
    for (int i = 0; i < strlist.size(); ++i)
    {
        m_pStandarModel->insertRow(0);
        m_pStandarModel->setData(m_pStandarModel->index(0,0),str + strlist.at(i));
    }
}

/*
 *功能：刷新信息
 *说明：接收memberoperatewidget界面的ok信息
*/
void MemberManageWidget::slot_RefrechInfo()
{
    this->InitForm();
}

/*
 *功能：用于实时显示鼠标移动到tableview中的值
*/
void MemberManageWidget::slot_ShowToolTip(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    int row = index.row();
    QString info = list_info.at(row);
    if (info.isEmpty())
        return;
    QToolTip::showText(QCursor::pos(),info);
}

void MemberManageWidget::SetDataShow()
{
    /*日期控件的设置*/
    ui->dateEdit_From->setCalendarPopup(true);
    ui->dateEdit_To->setCalendarPopup(true);
    QDate today = QDate::currentDate();
    ui->dateEdit_From->setDateRange(today.addDays(-90),today.addDays(90));
    ui->dateEdit_To->setDateRange(today.addDays(-90),today.addDays(90));

    ui->dateEdit_From->show();
    ui->label_From->show();
    ui->dateEdit_To->show();
    ui->label_to->show();

    ui->FindlineEdit->hide();
}

void MemberManageWidget::SetDataHide()
{
    ui->dateEdit_From->setCalendarPopup(true);
    ui->dateEdit_To->setCalendarPopup(true);
    QDate today = QDate::currentDate();
    ui->dateEdit_From->setDateRange(today.addDays(-90),today.addDays(90));
    ui->dateEdit_To->setDateRange(today.addDays(-90),today.addDays(90));

    ui->dateEdit_From->hide();
    ui->label_From->hide();
    ui->dateEdit_To->hide();
    ui->label_to->hide();

    ui->FindlineEdit->show();
}

/*
 *功能：删除当前选中的会员信息
*/
void MemberManageWidget::on_Delete_pushButton_clicked()
{
    if (ui->tableView->currentIndex().row() >= 0)
    {
        QString memberId = GetTableviewColumn();
        qDebug() <<"Delete member info:"<<memberId;

        QString sql = " where MemberId = "+memberId;

        if (myHelper::ShowMessageBoxQuesion("你确定要删除所选中行的VIP会员吗？删除后不可恢复!"))
        {
            if (m_pSqlhelp->DeleteTableInfo("Member",sql))
            {
                this->InitForm();
                qDebug() <<"delete ok!";
            }
        }
    }else
    {
        myHelper::ShowMessageBoxError("请先选中要删除的VIP会员！");
    }
}

void MemberManageWidget::on_Refresh_pushButton_clicked()
{
    this->InitForm();
}

/*
 *功能：根据选择的条件进行查找并显示
*/
void MemberManageWidget::on_Select_pushButton_clicked()
{
    QString sql = "SELECT *FROM Member WHERE 1 = 1";

    if (ui->FindlineEdit->text().isEmpty())
    {
        myHelper::ShowMessageBoxError("请输入查询的内容!!");
    }else
    {
        //VIP卡号,姓名，等级，手机，邮箱，出生日期，办卡日期
        if (ui->Choose_comboBox->currentIndex() == 0)
        {
            sql += " AND MemberId LIKE '"+ui->FindlineEdit->text()+"%'";
        }else if (ui->Choose_comboBox->currentIndex() == 1)
        {
            sql += " AND MemberName LIKE '" +ui->FindlineEdit->text() + "%'";
        }else if (ui->Choose_comboBox->currentIndex() == 2)
        {
            sql += " AND MemberClass LIKE '" +ui->FindlineEdit->text()+"%'";
        }else if (ui->Choose_comboBox->currentIndex() == 3)
        {
            sql += " AND MemberPhone LIKE '" +ui->FindlineEdit->text()+"%'";
        }else if (ui->Choose_comboBox->currentIndex() == 4)
        {
            sql += " AND MemberEmail LIKE '" + ui->FindlineEdit->text() + "%'";
        }else if (ui->Choose_comboBox->currentIndex() == 5)
        {
            sql += " AND MemberBirthday >= " +ui->dateEdit_From->date().toString()+" <= " + ui->dateEdit_To->date().toString();
        }else if (ui->Choose_comboBox->currentIndex() == 6)
        {
            sql += " AND MemberRegister >= "+ui->dateEdit_From->date().toString() +" <= "+ui->dateEdit_To->date().toString();
        }
        m_pSqlhelp->BindTableWhere(sql,ui->tableView,ColumnNames,ColumnWidths);
        qDebug() <<"select member info:"<< sql;
    }
}

void MemberManageWidget::on_pbnShowAll_clicked()
{
    this->InitForm();
}

/*
 * @Function:       on_Tutor_Out_pushButton_clicked()
 * @Description:    导出数据到excel表
 * @Calls:
 * @Input:          无
 * @Output:         无
 * @Return:         无
 * @Others:         其它说明
 */
void MemberManageWidget::on_Tutor_Out_pushButton_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
                                                    tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
        return;

    ExportExcelObject obj(fileName, "MemberInfo", ui->tableView);

    obj.addField(0, "编号", "char(20)");
    obj.addField(1, "姓名", "char(20)");
    obj.addField(2, "手机", "char(20)");
    obj.addField(3, "地址", "char(20)");
    obj.addField(4, "等级", "char(20)");
    obj.addField(5, "积分", "char(20)");
    obj.addField(6, "消费金额", "char(20)");
    obj.addField(7, "注册日期", "char(20)");
    obj.addField(8, "生日", "char(20)");
    obj.addField(9, "邮箱","char(20)");

    int renVal = obj.export2Excel();

    if (renVal > 0){
        myHelper::ShowMessageBoxInfo(QString("导出%1条记录成功!").arg(renVal));
        return;
    }
}
