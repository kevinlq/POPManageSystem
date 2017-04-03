#ifndef BASICDATAWIDGET_H
#define BASICDATAWIDGET_H

#include <QDialog>

namespace Ui {
class BasicDataWidget;
}

class BasicDataWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit BasicDataWidget(QWidget *parent = 0);
    ~BasicDataWidget();
    void InitForm();
    
private:
    Ui::BasicDataWidget *ui;
};

#endif // BASICDATAWIDGET_H
