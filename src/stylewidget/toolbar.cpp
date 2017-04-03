#include "toolbar.h"
#include "mybtn.h"
#include "constvalue.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <QLabel>
#include <QResizeEvent>
#include <QLatin1String>
#include <QCursor>
#include <QSignalMapper>
#include <QDebug>


//构造函数
ToolBar::ToolBar(QWidget *parent): QWidget(parent)
{
    CreateWidget();
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    setMouseTracking(true);

    mouseCursor = new QCursor(QPixmap(DEFAULT_CURSOR),15,10);
    this->setCursor(*mouseCursor);
}

//CreateWidget创建部件
void ToolBar::CreateWidget()
{
    signalMap = new QSignalMapper(this);

    //文本例子
    m_listMyStr<<"前台销售" <<"采购进货" <<"库存管理"
               <<"会员管理" <<"营业统计" <<"销售统计"
               <<"系统设置" <<"人员管理" <<"财务管理"
               <<"休闲娱乐";

    //创建toolbutton
    for(int nIndex = 0; nIndex < WIDGET_CNT;++nIndex)
    {
        //图像资源路径
        QString strImage = QString(QLatin1String(":/image/tool/%1.png")).arg(nIndex + 1);

        //创建自定义QToolButton(MyBtn)
        MyBtn *tool_button = new MyBtn(strImage,m_listMyStr[nIndex],this);

        m_listMyBtnPoint.append(tool_button);

        //设置toolbutton的位置----move(int x,int y)
        m_listMyBtnPoint.at(nIndex)->move(nIndex * TOOLWIDGET_H + VALUE_DIS,0);

        //该信号槽设置其他按钮的按下状态bool值
        connect(m_listMyBtnPoint.at(nIndex),SIGNAL(signal_parent(void*)),this,SLOT(slot_set(void*)));

        connect(tool_button,SIGNAL(clicked()),signalMap,SLOT(map()));
        signalMap->setMapping(tool_button,nIndex);
    }
    //创建label
    m_pLabel = new QLabel(this);
    m_pLabel->setPixmap(QPixmap(QLatin1String(":/image/logo.png")));

    connect(signalMap,SIGNAL(mapped(int)),this,SLOT(slot_turnPage(int)));

}

//槽函数
void ToolBar:: slot_set(void *pObject)
{
    for(int nIndex = 0;nIndex < WIDGET_CNT;++nIndex)
    {
        if(m_listMyBtnPoint.at(nIndex) != pObject)
        {
            m_listMyBtnPoint.at(nIndex)->SetPress(false);
        }
    }
}


/*
 *页面跳转
*/
void ToolBar::slot_turnPage(int page)
{
    emit signal_turnPage(page);
}

//resizeEvent
void ToolBar::resizeEvent (QResizeEvent * event)
{
    //按钮垂直居中
    m_pLabel->move(rect().width() - m_pLabel->pixmap()->width() - VALUE_DIS,
                   (rect().height()-m_pLabel->pixmap()->height())/2);
}
//sizeHint 为布局管理器提供大小
QSize ToolBar::sizeHint () const
{
    return QSize(0,TOOL_H);
}
//析构函数
ToolBar::~ToolBar()
{
}
