#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFrame>
#include <QPoint>
#include <QSystemTrayIcon>

class QVBoxLayout;
class QPaintEvent;
class QCursor;
class TitleBar;
class ToolBar;
class ContentWidget;
class StatuBar;
class QPropertyAnimation;
class QStackedWidget;

class ProSealWidget;        //前台销售
class PurchaseWidget;       //采购进货
class StockManageWidget;    //库存管理
class MemberManageWidget;   //会员管理
class SealStatisticsWidget; //销售统计界面
class SystemSetDialog;      //系统设置界面

//主窗口声明
class MainWindow : public QFrame
{
    Q_OBJECT

    //方向枚举
    enum enum_Direction
    {
        eNone,
        eTop = 1,
        eRight = 2 ,
        eBottom = 4,
        eLeft = 8,
        eTopRight = eTop + eRight,
        eRightBottom = eRight + eBottom,
        eBottomLeft = eBottom + eLeft,
        eLeftTop = eLeft + eTop
    };

    //界面枚举
    enum enum_Widget
    {
        eProSealWidget = 0,
        ePurchaseWidget,
        eStockManageWidget,
        eMemberManageWidget,
        eContentWidget,
        eSealStatisticWidget
    };
//单例模式
private:
    static MainWindow *m_pInstance;
public:
    static MainWindow* Instance();

    /*
     * 动画效果
     */
    void StartAnimation();
    void CloseWindowAnimation();

//构造与析构函数
private:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

//获取值--是否最大化
public:
    bool GetMaxWin();

//槽函数
private slots:

    void slot_mainmin();            //最小化

    void slot_mainmax();            //最大化/还原

    void slot_close();              //关闭窗口

    void slot_changeskin();         //换肤

    void slot_turnPage(int page);   //页面切换

//成员方法
private:
    //创建部件
    void CreateWidget();
    //创建设置布局
    void CreateLayout();
    //创建信号槽连接
    void CreateConnect();
    //确定点的位置方向
    enum_Direction PointValid(int nXRelative,int nYRelative);
    //设置鼠标形状样式
    void SetCursorStyle(enum_Direction direction);
    //缩放窗口
    void SetDrayMove(int nXGlobal,int nYGlobal,enum_Direction direction);
    //设置值--是否最大化
    void SetMaxWin();

    void ScreenFull();  //全屏显示
    void ScreenNormal();

//事件重写
protected:
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event);
    //重绘事件
    void paintEvent(QPaintEvent *event);

    void keyPressEvent(QKeyEvent *);

//成员变量
private:
    QVBoxLayout *m_pMainLayout;                         //主布局
    TitleBar *m_pTitleBar;                              //标题栏
    ToolBar *m_pToolBar;                                //工具栏
    ContentWidget *m_pContentWidget;                    //内容区域
    StatuBar *m_pStatuBar;                              //状态栏

    QStackedWidget *m_pStackedWdget;                    //堆栈窗口类指针

    bool m_bLeftBtnPress;                               //鼠标左键是否被按下
    QPoint m_ptPressGlobal;                             //主窗口需要移动的相对位移
    enum_Direction m_eDirection;                        //伸缩方向
    bool m_bMaxWin;                                     //是否已经最大化
    QRect m_rectRestoreWindow;                          //窗口恢复的大小

    QString m_pSkinName;
    QCursor *m_pMouseCursor;

    ProSealWidget *m_ProSealWidget;                     //前台销售
    PurchaseWidget *m_pPurchaseWidget;                  //采购进货
    StockManageWidget *m_pStockmanageWidget;            //库存管理
    MemberManageWidget *m_pMemberManageWidget;          //会员管理
    SealStatisticsWidget *m_pSealStaticticWidget;       //销售统计
    SystemSetDialog *m_pSystemSetDialog;                //系统设置

};
#endif // MAINWINDOW_H
