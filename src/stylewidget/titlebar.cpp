#include "titlebar.h"
#include "mainwindow.h"
#include  "constvalue.h"

#include <QMouseEvent>
#include <QLabel>
#include <QToolButton>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLatin1String>
#include <QCursor>
#include <QAction>
#include <QMenu>

//构造函数
TitleBar::TitleBar(QWidget *parent): QWidget(parent),m_bLeftButtonPress(false)
{
    CreateWidget();

    SetWidgetStyle();

    CreateLayout();

    CreateEventFiter();

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    setMouseTracking(true);

     mouseCursor = new QCursor(QPixmap(DEFAULT_CURSOR),15,10);
     this->setCursor(*mouseCursor);
     m_pLabelVersion->setCursor(*mouseCursor);

}
//创建子部件
void TitleBar::CreateWidget()
{
    //图像标签--logo
    m_pLabelIcon = new QLabel(this);
    QPixmap objPixmap(QLatin1String(":/image/AboutLogo.png"));
    m_pLabelIcon->setPixmap(objPixmap.scaled(TITLE_H,TITLE_H));
    //文本标签--标题
    m_pLabelTitle = new QLabel(this);
    m_pLabelTitle->setText(QString("大众超市收银管理系统  公众号：devstone"));
    //文本标签--样式版本
    m_pLabelVersion = new QLabel(this);
    m_pLabelVersion->setText(QString("V1.0"));
    //设置鼠标形状
    m_pLabelVersion->setCursor(Qt::PointingHandCursor);
    //按钮--更换皮肤
    m_pBtnSkin = new QToolButton(this);
    //设置初始图片
    SetBtnIcon(m_pBtnSkin,eBtnStateDefault,true);
    m_pBtnSkin->setToolTip("切换皮肤");
    //按钮--菜单
    m_pBtnMenu = new QToolButton(this);
    SetBtnIcon(m_pBtnMenu,eBtnStateDefault,true);
    m_pBtnMenu->setToolTip("菜单");
    //按钮--最小化
    m_pBtnMin = new QToolButton(this);
    SetBtnIcon(m_pBtnMin,eBtnStateDefault,true);
    m_pBtnMin->setToolTip("最小化");
    //按钮--最大化/还原
    m_pBtnMax = new QToolButton(this);
    SetBtnIcon(m_pBtnMax,eBtnStateDefault,true);
    m_pBtnMax->setToolTip("最大化");
    //按钮--关闭
    m_pBtnClose = new QToolButton(this);
    SetBtnIcon(m_pBtnClose,eBtnStateDefault,true);
    m_pBtnClose->setToolTip("关闭");
    //获得子部件
    const QObjectList &objList = children();
    for(int nIndex=0; nIndex<objList.count();++nIndex)
    {
        //设置子部件的MouseTracking属性
        ((QWidget*)(objList.at(nIndex)))->setMouseTracking(true);
        //如果是QToolButton部件
        if(0 == qstrcmp(objList.at(nIndex)->metaObject()->className(),"QToolButton"))
        {
            //连接pressed信号为slot_btnpress
            connect(((QToolButton*)(objList.at(nIndex))),SIGNAL(pressed()),this,SLOT(slot_btnpress()));
            //连接clicked信号为slot_btnclick
            connect(((QToolButton*)(objList.at(nIndex))),SIGNAL(clicked()),this,SLOT(slot_btnclick()));
            //设置顶部间距
            ((QToolButton*)(objList.at(nIndex)))->setContentsMargins(0,VALUE_DIS,0,0);
        }
    }
}

//设置子部件样式(qss)
void TitleBar::SetWidgetStyle()
{
    //设置标签的文本颜色，大小等以及按钮的边框
    setStyleSheet(QLatin1String("QLabel{color:#CCCCCC;font-size:12px;font-weight:bold;}QToolButton{border:0px;}"));
    //设置左边距
    m_pLabelTitle->setStyleSheet(QLatin1String("margin-left:6px;"));
    //设置右边距以及鼠标移上去时的文本颜色
    m_pLabelVersion->setStyleSheet(QLatin1String("QLabel{margin-right:10px;}QLabel:hover{color:#00AA00;}"));
}
//创建设置布局
void TitleBar::CreateLayout()
{
    //水平布局
    m_pLayout = new QHBoxLayout(this);
    //添加部件
    m_pLayout->addWidget(m_pLabelIcon);
    m_pLayout->addWidget(m_pLabelTitle);
    //添加伸缩项
    m_pLayout->addStretch(1);
    //添加部件
    m_pLayout->addWidget(m_pLabelVersion);
    m_pLayout->addWidget(m_pBtnSkin);
    m_pLayout->addWidget(m_pBtnMenu);
    m_pLayout->addWidget(m_pBtnMin);
    m_pLayout->addWidget(m_pBtnMax);
    m_pLayout->addWidget(m_pBtnClose);
    //设置Margin
    m_pLayout->setContentsMargins(0,0,VALUE_DIS,0);
    //设置部件之间的space
    m_pLayout->setSpacing(0);
    setLayout(m_pLayout);
}
//创建事件过滤器
void TitleBar::CreateEventFiter()
{
    m_pBtnSkin->installEventFilter(this);
    m_pBtnMenu->installEventFilter(this);
    m_pBtnMin->installEventFilter(this);
    m_pBtnMax->installEventFilter(this);
    m_pBtnClose->installEventFilter(this);
}
//获得图片路径(固定值)
const QString TitleBar::GetBtnImagePath(QToolButton *pBtn,bool bInit/*=false*/)
{
    QString strImagePath;
    //皮肤按钮
    if(m_pBtnSkin==pBtn)
    {
        strImagePath = QLatin1String(":/image/SkinButtom.png");
    }
    //菜单按钮
    if(m_pBtnMenu==pBtn)
    {
        strImagePath = QLatin1String(":/image/title_bar_menu.png");
    }
    //最小化
    if(m_pBtnMin==pBtn)
    {
        strImagePath = QLatin1String(":/image/sys_button_min.png");
    }
    //最大化/还原按钮，所以包括最大化和还原两张图片
    if(m_pBtnMax==pBtn)
    {
        //如果是初始设置或者主界面的最大化标志不为真(其中MainWindow::Instance()使用单例设计模式)
        if(bInit==true || MainWindow::Instance()->GetMaxWin()==false)
        {
            //最大化按钮图片路径
            strImagePath = QLatin1String(":/image/sys_button_max.png");
        }
        else
        {
            //还原按钮图片路径
            strImagePath = QLatin1String(":/image/sys_button_restore.png");
        }
    }
    //关闭按钮
    if(m_pBtnClose==pBtn)
    {
        strImagePath = QLatin1String(":/image/sys_button_close.png");
    }
    return strImagePath;
}
//设置按钮不同状态下的图标
void TitleBar::SetBtnIcon(QToolButton *pBtn,eBtnMoustState state,bool bInit/*=false*/)
{
    //获得图片路径
    QString strImagePath = GetBtnImagePath(pBtn,bInit);
    //创建QPixmap对象
    QPixmap objPixmap(strImagePath);
    //得到图像宽和高
    int nPixWidth = objPixmap.width();
    int nPixHeight = objPixmap.height();
    //如果状态不是无效值
    if(state!=eBtnStateNone)
    {
        /*设置按钮图片
        按钮的图片是连续在一起的，如前1/4部分表示默认状态下的图片部分,接后的1/4部分表示鼠标移到按钮状态下的图片部分
        */
        pBtn->setIcon(objPixmap.copy((nPixWidth/4)*(state-1),0,nPixWidth/4,nPixHeight));
        //设置按钮图片大小
        pBtn->setIconSize(QSize(nPixWidth/4,nPixHeight));
    }
}
//鼠标按下事件
void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if(event->y()<VALUE_DIS||event->x()<VALUE_DIS||rect().width()-event->x()<5)
        {
            event->ignore();
            return;
        }
        m_ptPressGlobal = event->globalPos();
        m_bLeftButtonPress = true;
    }
    event->ignore();
}
//鼠标移动事件
void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bLeftButtonPress)
    {
        QPoint ptMove = event->globalPos();
        //移动主窗口
        MainWindow *pMainWindow = (qobject_cast<MainWindow *>(parent()));
        pMainWindow->move(pMainWindow->pos()+ptMove-m_ptPressGlobal);
        //重新设置m_ptPressGlobal;
        m_ptPressGlobal = ptMove;
    }
    event->ignore();
}
//鼠标释放事件
void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bLeftButtonPress = false;
    }
     event->ignore();
}
//鼠标双击事件
void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
       emit signal_maxrestore();
       SetBtnIcon(m_pBtnMax,eBtnStateDefault);
    }
}
//事件过滤
bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    //按钮状态
    eBtnMoustState eState = eBtnStateNone;
    //判断事件类型--QEvent::Enter
    if (event->type() == QEvent::Enter)
    {
        eState = eBtnStateHover;
    }
    //判断事件类型--QEvent::Leave
    if (event->type() == QEvent::Leave)
    {
        eState = eBtnStateDefault;
    }
    //判断事件类型--QEvent::MouseButtonPress
    if (event->type() == QEvent::MouseButtonPress && ((QMouseEvent*)(event))->button()== Qt::LeftButton)
    {
        eState = eBtnStatePress;
    }
    //判断目标
    if(m_pBtnSkin==obj || m_pBtnMenu==obj || m_pBtnMin==obj || m_pBtnMax==obj || m_pBtnClose==obj)
    {
        //如果状态有效
        if(eState != eBtnStateNone)
        {
            //根据状态设置按钮图标
            SetBtnIcon((QToolButton *)obj,eState);
            return false;
        }
    }
    return QWidget::eventFilter(obj,event);
}
//槽函数--slot_btnpress
void TitleBar::slot_btnpress()
{
    //Do nothing
}
//槽函数--slot_btnclick
void TitleBar::slot_btnclick()
{
    QToolButton *pBtn = (QToolButton*)(sender());
    if(pBtn == m_pBtnMin)
    {
        emit signal_min();
    }
    if(pBtn == m_pBtnMax)
    {
        emit signal_maxrestore();
    }
    if(pBtn == m_pBtnClose)
    {
        emit signal_close();
    }
    if (pBtn == m_pBtnSkin)
    {
        emit signal_changeskin();
    }
}

//sizeHint
QSize TitleBar::sizeHint() const
{
    return QSize(0,TITLE_H);
}
//析构函数
TitleBar::~TitleBar()
{
}
