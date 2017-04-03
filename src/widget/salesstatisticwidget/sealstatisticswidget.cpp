#include "sealstatisticswidget.h"
#include "ui_sealstatisticswidget.h"
#include "qcustomplot.h"

#define TextColor QColor(0,0,0)         //黑色
#define Plot_NoColor QColor(0,0,0,0)    //黑色

#define TextWidth 2
#define LineWidth 1
#define DotWidth  4

SealStatisticsWidget::SealStatisticsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SealStatisticsWidget)
{
    ui->setupUi(this);

    this->InitPlot();
}

SealStatisticsWidget::~SealStatisticsWidget()
{
    delete ui;
}


void SealStatisticsWidget::InitPlot()
{
    this->InitPlotForm();

    ui->plot->yAxis->setLabel("销售数量");
    ui->plot->xAxis->setLabel("时间");


    //设置坐标颜色，坐标名称颜色
    ui->plot->yAxis->setLabelColor(TextColor);          //Y轴文字为黑色
    ui->plot->xAxis->setLabelColor(TextColor);          //X轴文字为黑色
    ui->plot->xAxis->setTickLabelColor(TextColor);      //X轴标记为黑色
    ui->plot->yAxis->setTickLabelColor(TextColor);
    ui->plot->xAxis->setBasePen(QPen(TextColor, TextWidth));
    ui->plot->yAxis->setBasePen(QPen(TextColor, TextWidth));
    ui->plot->xAxis->setTickPen(QPen(TextColor, TextWidth));
    ui->plot->yAxis->setTickPen(QPen(TextColor, TextWidth));
    ui->plot->xAxis->setSubTickPen(QPen(TextColor, TextWidth));
    ui->plot->yAxis->setSubTickPen(QPen(TextColor, TextWidth));

    //设置坐标背景色
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(234, 246, 247));//白色
    plotGradient.setColorAt(1, QColor(255, 255, 255));
    ui->plot->setBackground(plotGradient);

    //设置画布背景色
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));//灰色
    axisRectGradient.setColorAt(1, QColor(50, 50, 50));
    ui->plot->axisRect()->setBackground(axisRectGradient);

    //设置图例提示位置及背景色
    ui->plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    ui->plot->legend->setBrush(QColor(255, 255, 255, 200));

    ui->plot->addGraph();
    ui->plot->graph(0)->setName("销售额(单位:个)");

    //设置静态曲线的横坐标范围及自适应横坐标
    ui->plot->xAxis->setRange(0, currentCount + 1, Qt::AlignLeft);
    ui->plot->xAxis->setAutoTickCount(xTickCount);
    ui->plot->yAxis->setAutoTickCount(yTickCount);
    ui->plot->graph(0)->rescaleAxes();
    ui->plot->replot();
}

void SealStatisticsWidget::InitPlotForm()
{
    xTickCount = 10;
    yTickCount = 10;
    currentCount = 1000;    //当前最大为值1000
    currentColor = 0;

    Plot_DotColor.append(QColor(5, 189, 251));
    Plot_LineColor.append(QColor(41, 138, 220));    //线的颜色--蓝色
    Plot_BGColor.append(QColor(41, 138, 220, 80));

    Plot_DotColor.append(QColor(236, 110, 0));
    Plot_LineColor.append(QColor(246, 98, 0));      //线的颜色--橘黄色
    Plot_BGColor.append(QColor(246, 98, 0, 80));

    Plot_DotColor.append(QColor(255, 127, 0));
    Plot_LineColor.append(QColor(255, 139, 0));     //线的颜色--橘黄色-浅
    Plot_BGColor.append(QColor(255, 139, 0, 80));

    Plot_DotColor.append(QColor(255, 0, 254));
    Plot_LineColor.append(QColor(255, 52, 178));
    Plot_BGColor.append(QColor(255, 52, 178, 80));

    Plot_DotColor.append(QColor(254, 0, 0));
    Plot_LineColor.append(QColor(255, 48, 48));
    Plot_BGColor.append(QColor(255, 48, 48, 80));

    Plot_DotColor.append(QColor(229, 230, 250));
    Plot_LineColor.append(QColor(237, 237, 237));
    Plot_BGColor.append(QColor(237, 237, 237, 80));

    Plot_DotColor.append(QColor(106, 139, 34));
    Plot_LineColor.append(QColor(106, 142, 34));
    Plot_BGColor.append(QColor(106, 142, 34, 80));
}

void SealStatisticsWidget::LoadPlot()
{
    plot_keys.clear();
    plot_values.clear();

    //如果是基数个数则长度要-1
    if (currentCount % 2 != 0)
    {
        currentCount--;
    }

    for (int i = 0; i < currentCount / 2; i++)
    {
        plot_keys.append(i);
    }
    for (int i = 0; i < currentCount; i = i + 2)
    {
        qint16 value = StrHexToShort(QString("%1%2").arg(plot_data[i]).arg(plot_data[i + 1]));
        qDebug() << plot_data[i] << plot_data[i + 1] << value << i << currentCount;
        plot_values.append(value);
    }

    ui->plot->graph(0)->setData(plot_keys, plot_values);
    ui->plot->xAxis->setAutoTickCount(xTickCount);
    ui->plot->yAxis->setAutoTickCount(yTickCount);
    ui->plot->graph(0)->rescaleAxes();
    ui->plot->replot();
}

qint16 SealStatisticsWidget::StrHexToShort(QString strHex)
{
    bool ok;
    return strHex.toUShort(&ok, 16);
}

/*
 *功能：添加Y轴网络
*/
void SealStatisticsWidget::on_ckYGrid_stateChanged(int arg1)
{
    bool value = (arg1 == 0 ? false:true);
    ui->plot->yAxis->grid()->setSubGridVisible(value);
    ui->plot->replot();
}

/*
 *功能：添加X轴网络
*/
void SealStatisticsWidget::on_ckXGrid_stateChanged(int arg1)
{
    bool value = (arg1 == 0 ? false : true);
    ui->plot->xAxis->grid()->setSubGridVisible(value);
    ui->plot->replot();
}

/*
 *功能：显示图例
*/
void SealStatisticsWidget::on_ckText_stateChanged(int arg1)
{
    bool value = (arg1 == 0 ? false : true);
    ui->plot->legend->setVisible(value);
    ui->plot->replot();
}

/*
 *功能：添加阴影背景
*/
void SealStatisticsWidget::on_ckBackground_stateChanged(int arg1)
{
    bool value = (arg1 == 0 ? false : true);
    if (value)
    {
        ui->plot->graph(0)->setBrush(QBrush(Plot_BGColor[currentColor]));
    } else {
        ui->plot->graph(0)->setBrush(QBrush(Plot_NoColor));
    }
    ui->plot->replot();
}

/*
 *功能：开始移动伸缩
*/
void SealStatisticsWidget::on_ckMove_stateChanged(int arg1)
{
    bool value = (arg1 == 0 ? false : true);
    if (value) {
        ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    } else {
        ui->plot->setInteractions(QCP::iSelectOther);
    }
    ui->plot->replot();
}
