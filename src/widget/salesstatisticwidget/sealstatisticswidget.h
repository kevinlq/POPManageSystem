#ifndef SEALSTATISTICSWIDGET_H
#define SEALSTATISTICSWIDGET_H

#include <QWidget>

namespace Ui {
class SealStatisticsWidget;
}

class SealStatisticsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SealStatisticsWidget(QWidget *parent = 0);
    ~SealStatisticsWidget();
private slots:
    void on_ckYGrid_stateChanged(int arg1);

    void on_ckXGrid_stateChanged(int arg1);

    void on_ckText_stateChanged(int arg1);

    void on_ckBackground_stateChanged(int arg1);

    void on_ckMove_stateChanged(int arg1);

private:

    void InitPlot();
    void InitPlotForm();
    void LoadPlot();
    
private:
    Ui::SealStatisticsWidget *ui;

    int xTickCount;
    int yTickCount;
    int currentCount;
    int currentColor;
    QStringList plot_data;                  //Ъ§Он
    QVector<double> plot_keys;              //
    QVector<double> plot_values;            //
    qint16 StrHexToShort(QString strHex);

    QList<QColor> Plot_DotColor;
    QList<QColor> Plot_LineColor;
    QList<QColor> Plot_BGColor;
};

#endif // SEALSTATISTICSWIDGET_H
