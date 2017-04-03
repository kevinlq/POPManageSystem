#ifndef SYSTEMSETDIALOG_H
#define SYSTEMSETDIALOG_H

#include <QDialog>

class  SqlHelp;
class myHelper;
class IconHelper;

namespace Ui {
class SystemSetDialog;
}

class SystemSetDialog : public QDialog
{
    Q_OBJECT
    //界面对应枚举
    enum eWidgetName{
        eCompanyWidget = 0,
        eVipWidget,
        ePrintWidget,
        eDisWidget,
        eAnyWidget
    };
    
public:
    explicit SystemSetDialog(QWidget *parent = 0);
    ~SystemSetDialog();

private:
    void InitForm();
    void InitParamete();
    void CreateConnect();
protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    
private slots:

    void slot_buttonClick();

    void on_btnMenu_Close_clicked();

    void on_pbnSaveCompanyOk_clicked();

private:
    Ui::SystemSetDialog *ui;

    QPoint mousePoint;
    bool mousePressed;
    QRect location;

    SqlHelp *m_pSqlhelp;
    QVector <QPushButton *> m_pVecBtn;
};

#endif // SYSTEMSETDIALOG_H
