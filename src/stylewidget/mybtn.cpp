/******************************
自定义按钮类
*******************************/
#include "mybtn.h"
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QBitmap>
#include <QBrush>
#include <QFont>
#include <QPalette>
#include <QLatin1String>
#include <QDebug>

extern int TOOLWIDGET_W;
extern int TOOLWIDGET_H;
extern int TOOLICON_WH;

//构造函数
MyBtn::MyBtn(const QString &strImage,const QString &strInfo,QWidget *parent):QToolButton(parent),
    m_bOver(false),m_bPress(false),m_strImage(strImage),m_strInfo(strInfo)
{
    //文本颜色
    QPalette objPalette = palette();
    objPalette.setColor(QPalette::ButtonText, QColor(230,230,230));
    setPalette(objPalette);
    //文本粗体
    QFont &objFont = const_cast<QFont &>(font());
    objFont.setWeight(QFont::Bold);
    //样式
    setStyleSheet(QLatin1String("QToolButton{border:0px;}"));
    //大小
    setIconSize(QSize(TOOLICON_WH + 5,TOOLICON_WH + 5));
    resize(TOOLWIDGET_W,TOOLWIDGET_H + 5);
    //设置文本在图标下边
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //设置图像文本
    setIcon(QPixmap(strImage));
    setText(strInfo);
    //连接press信号槽，表示按钮按下时
    connect(this,SIGNAL(pressed()),this,SLOT(slot_pressed()));
}

MyBtn::MyBtn(QWidget *parent)
{
}

//析构函数
MyBtn::~MyBtn()
{
}

//设置图片名字
void MyBtn::setPicName(QString pic_name)
{
    this->pic_name = pic_name;
    setFixedSize(QPixmap(pic_name).size());
}

//重绘事件
void MyBtn::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //如果按钮被按下
    if(m_bPress)
    {
        //绘制被按下时的效果
        painterinfo(10,160,&painter);
    }
    else if(m_bOver)//如果按钮没有被按下并且鼠标移到按钮上
    {
        //绘制鼠标移到按钮上的按钮效果
        painterinfo(30,100,&painter);
    }
    //调用基类的重绘事件以显示图像文本等
    QToolButton::paintEvent(event);
}

//绘制背景渐变
void MyBtn::painterinfo(int nTopPartOpacity,int nBottomPartOpacity,QPainter *pPainter)
{

    QPen objPen(Qt::NoBrush,1); //设置画笔
    pPainter->setPen(objPen);

    QLinearGradient objLinear(rect().topLeft(),rect().bottomLeft());//设置渐变画刷

    objLinear.setColorAt(0,QColor(255,255,255,nTopPartOpacity));    //顶部颜色和透明度

    objLinear.setColorAt(0.5,QColor(255,255,255,50));//中间颜色和透明度---白色--透明度%50

    objLinear.setColorAt(1,QColor(255,255,255,nBottomPartOpacity)); //底部颜色和透明度
    QBrush objBrush(objLinear);
    pPainter->setBrush(objBrush);

    pPainter->drawRoundedRect(rect(),5,5); //画圆角矩形
}

//enterEvent--鼠标移到按钮上事件
void MyBtn::enterEvent(QEvent *event)
{
    SetOver(true);
}

//leaveEvent--鼠标离开按钮事件
void MyBtn::leaveEvent(QEvent *event)
{
    SetOver(false);
}

//设置鼠标在按钮上停留
void MyBtn::SetOver(bool bEnable)
{
    if (bEnable != m_bOver)
    {
        //设置m_bOver标志位
        m_bOver = bEnable;
        //更新
        update();
    }
}


//设置按钮被按下
void MyBtn::SetPress(bool bEnable)
{
    if (bEnable!= m_bPress)
    {
        //设置m_bOver标志位
        m_bPress = bEnable;
        //更新
        update();
    }
}


//slot_pressed--槽函数
void MyBtn::slot_pressed()
{
    SetPress(true);
    emit signal_parent(this);
}
