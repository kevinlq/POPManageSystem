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
#ifndef PROSEALWIDGET_H
#define PROSEALWIDGET_H

#include <QWidget>
#include <QModelIndex>

class QCursor;
class BasicDataWidget;
class QStandardItemModel;
class SqlHelp;
class PayMoneyWidget;
class QCompleter;
class QStringListModel;

namespace Ui {
class ProSealWidget;
}

class ProSealWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProSealWidget(QWidget *parent = 0);
    ~ProSealWidget();
    void InitForm();
    void InitParameter();
    void CreeateConnect();

    void InitGoodsHeaderTitle();

private slots:

    void on_pbnAdd_clicked();
    void on_pbnFindMmeber_clicked();

private slots:

    void slot_editComplete();                   //自动补全商品ID

    void slot_UpdateSellInfo();                  //更新显示

    void on_pbnNew_clicked();

    void on_pbnDelete_clicked();

    void on_pbnAccount_clicked();

private:

    void DeleteTableviewInfo();                 //删除tableview中的信息
    void keyPressEvent(QKeyEvent *);

    void CompleteGoodsId();                      //自动补全商品ID
    void AutoShowGoodsInfo(QString goodsId);
    void AutoAddGoodsNum(QString goodsId);      //自动增加商品数量
    void AutoSaveSellRecord(QString goodsId);   //自动保存销售记录

    void AutoshowGoodsPrice();                  //自动显示商品的价格

private:
    Ui::ProSealWidget *ui;
    QCursor *mouseCursor;

    BasicDataWidget *m_pBasicWidget;
    QStandardItemModel *m_pStandModel;

    SqlHelp *m_psqlhelp;
    PayMoneyWidget *m_pPaywidget;

    QStringList idlist;
    QCompleter *m_pCompleter;
    QStringListModel *m_pStringListModel;

    QString ColumnNames[9];                     //列明数组对象声明
    int ColumnWidths[9];                        //列宽数组声明

};

#endif // PROSEALWIDGET_H
