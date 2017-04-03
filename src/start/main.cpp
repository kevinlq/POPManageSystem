/***************************************************************
 *Copyright(c) 2014-2015 Company By LH Unigress
 *All rights reserved.
 *文件名称：主程序入口文件
 *简要描述：打开数据库，加载程序启动的各个配置参数
 *
 *当前版本：V3.0
 *作者：Kelvin Li
 *创作日期：2015/07
 *说明：本程序在V2.0基础上，进行了 代码重构，界面美化
******************************************************************/
#include <QApplication>
#include "mainwindow.h"

#include <QSharedMemory>
#include <QMessageBox>
#include <QFile>
#include "database.h"
#include "myhelper.h"
#include "loginwidget.h"
#include "paymoneywidget.h"

#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif

//程序主函数入口
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    myHelper::SetUTF8Code();

    QFont font = app.font();
    font.setFamily(QString("楷体"));
    font.setPointSize(12);
    app.setFont(font);

    //加载QSS样式表
    myHelper::SetStyle("style");

    //使程序只能运行一个实例
    QSharedMemory shared_memory("POP");

    if (!shared_memory.create(1))
    {
        myHelper::ShowMessageBoxError("程序已经运行，软件将自动关闭!");
        return 1;
    }

    //判断当前数据库文件是否存在(如果数据库打开失败则终止应用程序)
    if (!myHelper::FileIsExist("POP.db"))
    {
        myHelper::ShowMessageBoxError("数据库文件不存在,程序将自动关闭!");
        return 1;
    }

    if (!createConnection())
    {
        myHelper::ShowMessageBoxError("打开数据库失败，程序将自动关闭!");
        return 1;
    }

    LoginWidget *login = new LoginWidget();
    login->show();

    return app.exec();
}
