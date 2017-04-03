#ifndef STATUBAR_H
#define STATUBAR_H
#include <QWidget>
class QLabel;
class QToolButton;
class QPushButton;
class QHBoxLayout;
class QCursor;

class ServerHelp;

//自定义状态栏
class StatuBar : public QWidget
{
    Q_OBJECT
    //构造与析构函数
public:
    StatuBar(QWidget *parent = 0);
    ~StatuBar();
    //成员方法
private:
    //创建部件
    void CreateWidget();
    //设置部件样式
    void SetWidgetStyle();
    //创建布局
    void CreateLayout();
    //成员变量
private:
    QLabel *m_pLabelProVer;
    QLabel *m_pLabelBklib;
    QToolButton *m_pBtnUpdate;
    QPushButton *m_pBtn360Clound;
    QHBoxLayout *m_pLayout;

    QCursor *mouseCursor;

public:
    QSize	sizeHint () const;
};

#endif // STATUBAR_H
