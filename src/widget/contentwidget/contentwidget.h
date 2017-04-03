#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QWidget>
class QPaintEvent;
class QCursor;
class QTimer;
class QLabel;
class QSplitter;

//内容部件区域
class ContentWidget : public QWidget
{
    Q_OBJECT
    //构造与析构函数
public:
    ContentWidget(QWidget *parent = 0);
    ~ContentWidget();
    void InitWidget();
protected:
    void paintEvent(QPaintEvent *event);

private:

    QSplitter *splitter ;
    QCursor *mouseCursor;
};

#endif // CONTENTWIDGET_H
