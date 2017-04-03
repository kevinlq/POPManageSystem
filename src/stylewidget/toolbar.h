#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QList>

class MyBtn;
class QLabel;
class QResizeEvent;
class QCursor;
class MyBtn;
class QSignalMapper;

//自定义工具栏
class ToolBar : public QWidget
{
    Q_OBJECT
    //构造与析构函数
public:
    ToolBar(QWidget *parent = 0);
    ~ToolBar();
private:
    void CreateWidget();
private:
    QSize	sizeHint () const;

private slots:
    void slot_set(void *pObject);
    void slot_turnPage(int page);
signals:
    void signal_turnPage(int page);

protected:
    void resizeEvent (QResizeEvent * event);
private:
    QList<MyBtn*> m_listMyBtnPoint; //自定义按钮
    QList<QString> m_listMyStr;     //按钮对应的文本
    QLabel *m_pLabel;               //label，显示logo

    //MyBtn *tool_button;

    QCursor *mouseCursor;
    QSignalMapper *signalMap;
};
#endif // TOOLBAR_H
