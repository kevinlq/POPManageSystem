/***************************************************************
 *Copyright(c) 2014-2015 Company By LH Unigress
 *All rights reserved.
 *文件名称：软件主界面控制文件
 *简要描述：将在所有子界面，并初始化.
 *
 *当前版本：V3.0
 *作者：Kelvin Li
 *创作日期：2015/07
 *说明：
******************************************************************/
#include "constvalue.h"
#include "mainwindow.h"
#include "titlebar.h"
#include "toolbar.h"
#include "contentwidget.h"
#include "statubar.h"
#include "prosealwidget.h"
#include "purchasewidget.h"
#include "stockmanagewidget.h"
#include "membermanagewidget.h"
#include "sealstatisticswidget.h"
#include "systemsetdialog.h"

#include <QVBoxLayout>
#include <QPaintEvent>
#include <QBitmap>
#include <QIcon>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QLatin1String>
#include <QCursor>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QSystemTrayIcon>
#include <QDebug>

//单例模式构建方法
MainWindow* MainWindow::m_pInstance = NULL;
MainWindow* MainWindow::Instance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new MainWindow();
    }

    return m_pInstance;
}

/*
 *启动 动画的实现
*/
void MainWindow::StartAnimation()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

/*
 *关闭动画的实现
*/
void MainWindow::CloseWindowAnimation()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation,SIGNAL(finished()),this,SLOT(close()));
}


//构造函数
MainWindow::MainWindow(QWidget *parent): QFrame(parent),m_bLeftBtnPress(false),
    m_ptPressGlobal(0,0),m_eDirection(eNone),m_bMaxWin(false)
{
    setWindowFlags(Qt::FramelessWindowHint);

    setObjectName(QLatin1String("MainFrame"));

    m_pSkinName = "1";
    setStyleSheet(QLatin1String("#MainFrame {background-image:url(:/image/skin/2.jpg);border:1px solid black;}"));

    CreateWidget();

    CreateLayout();

    CreateConnect();

    setMinimumWidth(MINI_W);
    setMinimumHeight(MINI_H);

    setMouseTracking(true);

    setWindowIcon(QIcon(QLatin1String(DEFAULT_ICON)));

    m_pMouseCursor = new QCursor(QPixmap(DEFAULT_CURSOR),15,10);
    this->setCursor(*m_pMouseCursor);

    this->StartAnimation();
    this->setWindowTitle("大众超市收银管理系统");
}


//创建部件
void MainWindow::CreateWidget()
{
    m_pStackedWdget = new QStackedWidget(this);

    m_pTitleBar = new TitleBar(this);                   //创建标题栏

    m_pToolBar = new ToolBar(this);                     //创建工具栏

    m_pContentWidget = new ContentWidget(this);         //创建内容区域

    m_pStatuBar = new StatuBar(this);                   //创建状态栏

    m_ProSealWidget = new ProSealWidget();              //前台销售界面

    m_pPurchaseWidget = new PurchaseWidget();           //采购进货界面

    m_pStockmanageWidget = new StockManageWidget();     //库存管理界面

    m_pMemberManageWidget = new MemberManageWidget();   //会员管理

    m_pSealStaticticWidget = new SealStatisticsWidget();//销售统计界面

    m_pSystemSetDialog = new SystemSetDialog();          //系统设置界面

    QPalette palette;
    QColor color(239,246,250);
    palette.setBrush(QPalette::Window, QBrush(color));
    m_pStackedWdget->setPalette(palette);
    m_pStackedWdget->setAutoFillBackground(true);

    //将子界面加入到堆栈窗体中
    m_pStackedWdget->addWidget(m_ProSealWidget);
    m_pStackedWdget->addWidget(m_pPurchaseWidget);
    m_pStackedWdget->addWidget(m_pStockmanageWidget);
    m_pStackedWdget->addWidget(m_pMemberManageWidget);
    m_pStackedWdget->addWidget(m_pContentWidget);
    m_pStackedWdget->addWidget(m_pSealStaticticWidget);

}


//创建设置布局
void MainWindow::CreateLayout()
{
    //创建布局
    m_pMainLayout = new QVBoxLayout(this);
    //将部件加入到布局中
    m_pMainLayout->addWidget(m_pTitleBar);
    m_pMainLayout->addWidget(m_pToolBar);
    m_pMainLayout->addWidget(m_pStackedWdget);
    m_pMainLayout->addWidget(m_pStatuBar);
    //设置间距与边缘空白
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setContentsMargins(0,0,0,0);
}


//创建信号槽连接
void MainWindow::CreateConnect()
{
    connect(m_pTitleBar,SIGNAL(signal_min()),this,SLOT(slot_mainmin()));
    connect(m_pTitleBar,SIGNAL(signal_maxrestore()),this,SLOT(slot_mainmax()));
    connect(m_pTitleBar,SIGNAL(signal_close()),this,SLOT(slot_close()));
    connect(m_pTitleBar,SIGNAL(signal_changeskin()),this,SLOT(slot_changeskin()));

    connect(m_pToolBar,SIGNAL(signal_turnPage(int)),this,SLOT(slot_turnPage(int)));
}


//确定点的位置方向
MainWindow::enum_Direction MainWindow::PointValid(int nXRelative,int nYRelative)
{
    int nTop = qAbs(nYRelative)<VALUE_DIS ?eTop:eNone;
    int nRight = qAbs(nXRelative-rect().right())<VALUE_DIS ? eRight:eNone;
    int nBottom = qAbs(nYRelative-rect().bottom())<VALUE_DIS ? eBottom:eNone;
    int nLeft = qAbs(nXRelative)<VALUE_DIS ? eLeft:eNone;
    return enum_Direction(nTop + nRight + nBottom + nLeft);
}


//设置鼠标样式
void MainWindow::SetCursorStyle(enum_Direction direction)
{
    //设置上下左右以及右上、右下、左上、坐下的鼠标形状
    switch(direction)
    {
    case eTop:
    case eBottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case eRight:
    case eLeft:
        setCursor(Qt::SizeHorCursor);
        break;
    case eTopRight:
    case eBottomLeft:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case eRightBottom:
    case eLeftTop:
        setCursor(Qt::SizeFDiagCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

//设置鼠标拖动的窗口位置信息
void MainWindow::SetDrayMove(int nXGlobal,int nYGlobal,enum_Direction direction)
{
    //计算偏差
    int ndX = nXGlobal - m_ptPressGlobal.x();
    int ndY = nYGlobal - m_ptPressGlobal.y();
    //获得主窗口位置信息
    QRect rectWindow = geometry();
    //判别方向
    if(direction & eTop)
    {
        rectWindow.setTop(rectWindow.top()+ndY);
    }
    if(direction & eRight)
    {
        rectWindow.setRight(rectWindow.right()+ndX);
    }
    if(direction & eBottom)
    {
        rectWindow.setBottom(rectWindow.bottom()+ndY);
    }
    if(direction & eLeft)
    {
        rectWindow.setLeft(rectWindow.left()+ndX);
    }
    if(rectWindow.width()< minimumWidth() || rectWindow.height()<minimumHeight())
    {
        return;
    }
    //重新设置窗口位置为新位置信息
    setGeometry(rectWindow);
}

//鼠标按下事件
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_ptPressGlobal = event->globalPos();
        m_bLeftBtnPress = true;
    }
}

//鼠标移动事件
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_bLeftBtnPress)
    {
        m_eDirection = PointValid(event->x(),event->y());
        SetCursorStyle(m_eDirection);
    }
    else
    {
        int nXGlobal = event->globalX();
        int nYGlobal = event->globalY();
        SetDrayMove(nXGlobal,nYGlobal,m_eDirection);
        m_ptPressGlobal =QPoint(nXGlobal,nYGlobal);
    }
}

//鼠标释放事件
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bLeftBtnPress = false;
        m_eDirection = eNone;
    }
}

//重绘事件
void MainWindow::paintEvent(QPaintEvent *event)
{
    //生成一张位图
    QBitmap objBitmap(size());
    //QPainter用于在位图上绘画
    QPainter painter(&objBitmap);
    //填充位图矩形框(用白色填充)
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    //在位图上画圆角矩形(用黑色填充)
    painter.drawRoundedRect(this->rect(),10,10);
    //使用setmask过滤即可
    setMask(objBitmap);
}

void MainWindow::keyPressEvent(QKeyEvent *key)
{
    switch(key->key())
    {
        case Qt::Key_F1:
            this->ScreenFull();
            //this->showFullScreen();
            break;
    case Qt::Key_Escape:
            //this->showNormal();
            this->ScreenNormal();
            break;
    default:
            QFrame::keyPressEvent(key);
            break;
    }
}

//获取值--是否最大化
bool MainWindow::GetMaxWin()
{
    return m_bMaxWin;
}

//设置最大化/还原
void MainWindow::SetMaxWin()
{
    if(!m_bMaxWin)
    {
        m_rectRestoreWindow = geometry();
        setGeometry(qApp->desktop()->availableGeometry());
    }
    else
    {
        setGeometry(m_rectRestoreWindow);
    }
    m_bMaxWin = !m_bMaxWin;
}

void MainWindow::ScreenFull()
{
    //this->setGeometry(qApp->desktop()->geometry());
    //this->layout()->setContentsMargins(0,0,0,0);
    this->showFullScreen();
    m_pTitleBar->setVisible(false);
}

void MainWindow::ScreenNormal()
{
    //this->setGeometry(qApp->desktop()->availableGeometry());
    //this->layout()->setContentsMargins(1,1,1,1);
    this->showNormal();
    m_pTitleBar->setVisible(true);
}

//最小化
void MainWindow::slot_mainmin()
{
    this->showMinimized();
}

//最大化/还原
void MainWindow::slot_mainmax()
{
    SetMaxWin();
}

//关闭窗口
void MainWindow::slot_close()
{
    //close();
    this->CloseWindowAnimation();
}

//换肤
void MainWindow::slot_changeskin()
{
    if (m_pSkinName == "1")
    {
        m_pSkinName = "2";
        setStyleSheet(QLatin1String("#MainFrame {background-image:url(:/image/skin/1.jpg);border:1px solid black;}"));
    }else if (m_pSkinName == "2")
    {
        m_pSkinName = "3";
        setStyleSheet(QLatin1String("#MainFrame {background-image:url(:/image/skin/2.jpg);border:1px solid black;}"));
    }else if (m_pSkinName == "3")
    {
        m_pSkinName = "4";
        setStyleSheet(QLatin1String("#MainFrame {background-image:url(:/image/skin/3.jpg);border:1px solid black;}"));
    }else if (m_pSkinName == "4")
    {
        m_pSkinName = "1";
        setStyleSheet(QLatin1String("#MainFrame {background-image:url(:/image/skin/4.jpg);border:1px solid black;}"));
    }
}

/*
 *页面跳转槽函数
*/
void MainWindow::slot_turnPage(int page)
{
    if (page == 0)
    {
        m_pStackedWdget->setCurrentIndex(eProSealWidget);
    }else if(page == 1)
    {
        m_pStackedWdget->setCurrentIndex(ePurchaseWidget);
    }
    else if(page == 2)
    {
        m_pStackedWdget->setCurrentIndex(eStockManageWidget);
    }
    else if (page == 3)
    {
        m_pStackedWdget->setCurrentIndex(eMemberManageWidget);
    }
    else if (page == 4)
    {
        m_pStackedWdget->setCurrentIndex(eContentWidget);
    }else if (page == 5)
    {
        m_pStackedWdget->setCurrentIndex(eSealStatisticWidget);
    }else if(page == 6)
    {
        m_pSystemSetDialog->exec();
    }
}

//析构函数
MainWindow::~MainWindow()
{

}
