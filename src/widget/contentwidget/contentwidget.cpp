#include "contentwidget.h"
#include "constvalue.h"
#include "qcustomplot.h"

#include <QPaintEvent>
#include <QPainter>
#include <QCursor>
#include <QTimer>
#include <QLabel>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVector>

//构造函数
ContentWidget::ContentWidget(QWidget *parent): QWidget(parent)
{
    setMouseTracking(true);

    //设置鼠标样式
    mouseCursor = new QCursor(QPixmap(DEFAULT_CURSOR),15,10);
    this->setCursor(*mouseCursor);

    this->InitWidget();
}

//析构函数
ContentWidget::~ContentWidget()
{
}

void ContentWidget::InitWidget()
{
//    QCustomPlot *customPlot = new QCustomPlot();
//    customPlot->resize(300,300);

//    QVector<double> x(101),y(101);

//    for (int i = 0; i < 101; i++)
//    {
//        x[i] = i/5 - 10;
//        y[i] = qPow(x[i],3);
//    }
//    customPlot->addGraph();
//    customPlot->graph(0)->setData(x,y);

//    customPlot->xAxis->setLabel("X");
//    customPlot->yAxis->setLabel("Y");

//    customPlot->xAxis->setRange(-11,11);
//    customPlot->yAxis->setRange(-1100,2200);
}

//重绘事件
void ContentWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen objPen(Qt::transparent);
    painter.setPen(objPen);
    QBrush objBrush(QColor(255,255,255,120));
    painter.setBrush(objBrush);
    painter.drawRect(this->rect());
}

