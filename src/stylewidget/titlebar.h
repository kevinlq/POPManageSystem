#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QPoint>

class QMouseEvent;
class QPaintEvent;
class QLabel;
class QToolButton;
class QHBoxLayout;
class QCursor;
class QAction;
class QMenu;

//自定义标题栏
class TitleBar : public QWidget
{
    Q_OBJECT

    //枚举，按钮状态
    enum eBtnMoustState{
        eBtnStateNone,//无效
        eBtnStateDefault,//默认值(如按钮初始显示)
        eBtnStateHover,//鼠标移到按钮上状态
        eBtnStatePress//鼠标按下按钮时状态
    };

public:
    TitleBar(QWidget *parent = 0);
    ~TitleBar();

private:
    //创建子部件
    void CreateWidget();
    //设置子部件样式
    void SetWidgetStyle();
    //创建布局管理
    void CreateLayout();
    //设置事件过滤
    void CreateEventFiter();
    //得到按钮图片路径
    const QString GetBtnImagePath(QToolButton *pBtn,bool bInit=false);
    //设置按钮的显示图片
    void SetBtnIcon(QToolButton *pBtn,eBtnMoustState state,bool bInit=false);

private slots:
    //press
    void slot_btnpress();
    //click
    void slot_btnclick();

signals:
    void signal_min();
    void signal_maxrestore();
    void signal_close();
    void signal_changeskin();
//事件覆盖
protected:
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event);
    //鼠标双击事件
    void mouseDoubleClickEvent(QMouseEvent *event);
    //事件过滤
    bool eventFilter(QObject *obj, QEvent *event);

public:
    QSize sizeHint() const;

private:
    QLabel *m_pLabelIcon;
    QLabel *m_pLabelTitle;
    QLabel *m_pLabelVersion;
    QToolButton *m_pBtnSkin;
    QToolButton *m_pBtnMenu;
    QToolButton *m_pBtnMin;
    QToolButton *m_pBtnMax;
    QToolButton *m_pBtnClose;
    QHBoxLayout *m_pLayout;
    bool m_bLeftButtonPress;
    QPoint m_ptPressGlobal;
    QCursor *mouseCursor;   //鼠标样式
};
#endif // TITLEBAR_H
