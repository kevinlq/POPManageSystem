#ifndef PURCHASEWIDGET_H
#define PURCHASEWIDGET_H

#include <QWidget>

class SqlHelp;
class BasicDataWidget;
class QStandardItemModel;

namespace Ui {
class PurchaseWidget;
}

class PurchaseWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PurchaseWidget(QWidget *parent = 0);
    ~PurchaseWidget();

    void InitForm();
    void InitParamete();
    void CreateConnect();
    void InitGoodsInfo();               //商品信息初始化
private:
    bool GetGoodsId(QString &id);
    void ClearTableview();              //清除tableview中的内容
    
private slots:

    void slot_UpdateUnitIfno();

    void slot_UpdateSupplyInfo();

    void slot_UpdateGoodsBandInfo();

    void on_tbnFindSupply_clicked();

    void on_pbnFindColor_clicked();

    void on_pbnFindUnit_clicked();

    void on_pbnAddGoodsOk_clicked();

    void on_pbnNewSignal_clicked();

    void on_Save_pushButton_clicked();

    void on_Delete_pushButton_clicked();

    void on_Check_pushButton_clicked();

    void on_Tutor_pushButton_clicked();

    void on_Find_Brand_pushButton_clicked();

private:
    Ui::PurchaseWidget *ui;
    SqlHelp *m_pSqlhelp;
    QStandardItemModel *m_pStandModel;
    BasicDataWidget *m_pBascWidget;

};

#endif // PURCHASEWIDGET_H
