/***************************************************************
 *Copyright(c) 2014-2015 Company By LH Unigress
 *All rights reserved.
 *文件名称：基础资料管理文件
 *简要描述：加载系统的一些基础资料，初始化相应的设置
 *
 *当前版本：V2.0
 *作者：Kelvin Li
 *Email:creazylq@163.com
 *QQ:936563422
 *创作日期：2014/10
 *说明：提供修改商品的规格和颜色、供货商信息、生产厂信息、商品的拆包、品牌管理、类别管理、VIP类别管理等
 *一些操作，可以对以上信息进行修改、删除、添加。需要管理员权限才能操作
 *
 *更新时间：2015-7-28(对一些函数的封装)
 *更新版本：V3.0
 *        界面布局注意：各个界面对应的逻辑值不能变，否则出现不对应
 *        品牌管理：0    类别管理：1
 *        颜色管理：2    计量单位：3
 *        供货商：  4    VIP   ：5
 *        库存调整：6    库存调拨：7
******************************************************************/
#ifndef BASICDATAWIDGET_H
#define BASICDATAWIDGET_H

#include <QDialog>

class SqlHelp;
class QSignalMapper;
class QTableView;
class QCompleter;
class QStandardItemModel;

namespace Ui {
class BasicDataWidget;
}

class BasicDataWidget : public QDialog
{
    Q_OBJECT

    //按钮界面对应的枚举
    enum eWidgetName{
        eCategorig = 0,
        eColor,
        eUnit,
        eSupplier = eUnit + eUnit,
        eVip,
        eAdjust,
        eAllocation,
        eBrand
    };
    
public:
    explicit BasicDataWidget(QWidget *parent = 0);
    ~BasicDataWidget();
    void InitForm();
    void InitParamete();
    void CreateConnect();

    void SetPbnCheck(QString name);            //设置某个按钮被选中同时显示对应的界面

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
private:
    void ShowDataInfo();
    void SetShowWidget(int pageNum);                    //用来显示不同的界面和对应的标题
    QString GetTbleviewColumn(QTableView *tableview);   //获取tableview某个列的值

    void CompleteEmail();                               //自动补全Email
    
private slots:
    void on_btnMenu_Close_clicked();

private slots:
    void slot_buttonClick();
    void slot_ShowVipInfo();                    //实时显示VIP信息
    void slot_ShowUnitInfo();                   //实时显示商品单位信息
    void slot_ShowSupplyInfo();                 //实时显示供货商信息
    void slot_ShowGoodsBrandInfo();             //实时显示商品品牌信息

    void slot_OnEmailChoosed(const QString &);

    void slot_OnTextChanged(const QString &);

    void on_pbnVipAdd_clicked();

    void on_pbnVipCancel_clicked();

    void on_pbnVipModify_clicked();

    void on_pbnVipSave_clicked();

    void on_pbnVipDelete_clicked();

    void on_pbnUnitAdd_clicked();

    void on_pbnUnitModiyf_clicked();

    void on_pbnUnitSave_clicked();

    void on_pbnUnitCanclel_clicked();

    void on_pbnUnitDelete_clicked();

    void on_pbnAddSuupe_clicked();

    void on_pbnModifySup_clicked();

    void on_pbnSaveSupply_clicked();

    void on_pbnSupplyCancel_clicked();

    void on_pbnDeleteSupply_clicked();

    void on_pbnBrandAdd_clicked();

    void on_pbnBrandModify_clicked();

    void on_pbnBrandSave_clicked();

    void on_pbnBrandCancel_clicked();

    void on_pbnBrandDelete_clicked();

signals:
    void signal_VipInfoOk();            //Vip信息操作ok
    void signal_UnitInfoOk();           //商品单位信息操作OK
    void signal_SupplyInfoOk();         //供货商信息操作ok
    void signal_GoodsBrandInfoOk();     //商品品牌信息操作ok

private:
    Ui::BasicDataWidget *ui;

    QCompleter *m_pCompleter;
    QStandardItemModel *m_pStandarModel;

    QPoint mousePoint;
    bool mousePressed;
    QRect location;

    SqlHelp *m_pSqlhelp;

    QVector <QPushButton *> m_pVecBtn;

    //颜色数据表标题
    QString columnColorNames[2];
    int columnColorWidths[2];
    //类型数据表标题
    QString columnTypeNames[2];
    int columnTypeWidths[2];
    //供货商数据表标题
    QString columnSupplyNames[7];
    int columnSupplyWidths[7];

    //VIP等级标题
    QString columnVipNames[3];
    int columnVipWidths[3];

    //商品单位数据表标题
    QString columnUnitNames[2];
    int columnUnitWidths[2];
};

#endif // BASICDATAWIDGET_H
