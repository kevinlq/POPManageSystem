#include "statubar.h"
#include "constvalue.h"
#include "serverhelp.h"
#include "myapp.h"

#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLatin1String>
#include <QCursor>

const int L_MARGIN =10;
const int R_MARGIN = 50;
const int S_WIDGET = 20;


StatuBar::StatuBar(QWidget *parent): QWidget(parent)
{
    CreateWidget();
    SetWidgetStyle();
    CreateLayout();

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    setMouseTracking(true);

    mouseCursor = new QCursor(QPixmap(DEFAULT_CURSOR),15,10);
    this->setCursor(*mouseCursor);
}
//创建部件
void StatuBar::CreateWidget()
{
    m_pLabelProVer = new QLabel("技术支持：公众号 destone",this);
    m_pLabelBklib = new QLabel(Myapp::CurrentUserName,this);
    m_pBtnUpdate = new QToolButton(this);
    m_pBtnUpdate->setText("检查更新");
    m_pBtn360Clound = new QPushButton(this);
    m_pBtn360Clound->setIcon(QPixmap(QLatin1String(":/image/netstateok.png")));
    m_pBtn360Clound->setText("已经连接至数据库");

    const QObjectList &objList = children();
    for(int nIndex=0; nIndex<objList.count();++nIndex)
    {
        ((QWidget*)(objList.at(nIndex)))->setMouseTracking(true);
    }
}

//设置部件样式
void StatuBar::SetWidgetStyle()
{
    setStyleSheet(QLatin1String("QLabel,QPushButton,QToolButton{color:#CCCCCC;font-size:12px;font-weight:bold;}QPushButton,QToolButton{border:0px;}"));
    m_pBtnUpdate->setStyleSheet(QLatin1String("QToolButton:hover{color:#FFFFFF;}"));
}

//创建布局
void StatuBar::CreateLayout()
{
    m_pLayout = new QHBoxLayout(this);
    m_pLayout->addWidget(m_pLabelProVer);
    m_pLayout->addWidget(m_pLabelBklib);
    m_pLayout->addWidget(m_pBtnUpdate);
    m_pLayout->addStretch(1);
    m_pLayout->addWidget(m_pBtn360Clound);
    m_pLayout->setContentsMargins(L_MARGIN,0,R_MARGIN,0);
    m_pLayout->setSpacing(S_WIDGET);
    setLayout(m_pLayout);
}


StatuBar::~StatuBar()
{
}

QSize	StatuBar::sizeHint () const
{
    return QSize(0,28);
}

