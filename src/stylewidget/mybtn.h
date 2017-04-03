/******************************
自定义按钮类
*******************************/
#ifndef MYBTN_H
#define MYBTN_H

#include <QToolButton>

class QPaintEvent;
class QEvent;
class QMouseEvent;
class QPainter;

//自定义按钮
class MyBtn : public QToolButton
{
    Q_OBJECT
//构造与析构函数
public:
    MyBtn(const QString &strImage,const QString &strInfo,QWidget *parent = 0);
    MyBtn(QWidget *parent = 0);
    ~MyBtn();
    void setPicName(QString pic_name);
//事件重写
protected:
    //重绘事件
    void paintEvent(QPaintEvent *event);
    //enterEvent
    void enterEvent(QEvent *event);
    //leaveEvent
    void leaveEvent(QEvent *event);

    //共有函数
public:
    void SetPress(bool bEnable);
private:
    void SetOver(bool bEnable);
    void painterinfo(int nTopPartOpacity,int nBottomPartOpacity,QPainter *pPainter);

 //槽函数
private slots:

    void slot_pressed();

//自定义信号
signals:
    void signal_parent(void *pObject);


//类成员
private:

    QString pic_name;
    //鼠标是否在该按钮上
    bool m_bOver;
    //是否按下
    bool m_bPress;
    //图片路径
    QString m_strImage;
    //要显示文本信息
    QString m_strInfo;
};

#endif // MYBTN_H
