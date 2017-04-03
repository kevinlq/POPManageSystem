#ifndef PAYMONEYWIDGET_H
#define PAYMONEYWIDGET_H

#include <QDialog>

namespace Ui {
class PayMoneyWidget;
}

class PayMoneyWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit PayMoneyWidget(QWidget *parent = 0);
    ~PayMoneyWidget();
    void InitForm();
    void InitParameter();

    void SetGoodsMoneyInfo(QString money);
protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    
private slots:
    void on_btnMenu_Close_clicked();

    void on_pbnOk_clicked();

    void on_radioButtonCash_clicked();

    void on_radioButtonDwping_clicked();

    void on_radioButtonKind_clicked();

    void on_lineEditCash_textChanged(const QString &arg1);

signals:
    void signal_paymentOk();

private:
    Ui::PayMoneyWidget *ui;

    QPoint mousePoint;
    bool mousePressed;
    QRect location;
};

#endif // PAYMONEYWIDGET_H
