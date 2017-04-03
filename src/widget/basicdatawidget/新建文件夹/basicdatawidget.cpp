#include "basicdatawidget.h"
#include "ui_basicdatawidget.h"

BasicDataWidget::BasicDataWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BasicDataWidget)
{
    ui->setupUi(this);

    this->InitForm();
}

BasicDataWidget::~BasicDataWidget()
{
    delete ui;
}

void BasicDataWidget::InitForm()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
}
