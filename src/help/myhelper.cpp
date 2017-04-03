/***************************************************************
 *Copyright(c) 2014-2015 Company By LH Unigress
 *All rights reserved.
 *文件名称：帮助及参数设置文件
 *简要描述：设置一些提示框、系统编码格式、皮肤、字符格式
 *
 *当前版本：V2.0
 *作者：Kelvin Li
 *创作日期：2014/10
 *说明：
*****************************************************************/
#include "myhelper.h"
#include <QApplication>
#include "frmmessagebox.h"

#include <QTime>
#include <QProcess>
#include <QSqlQuery>
#include <QDebug>
#include <QCompleter>
#include <QStandardItemModel>
#include <QLineEdit>

myHelper::myHelper(QObject *parent) :
    QObject(parent)
{
}


/*设置编码格式为GB2312*/
void myHelper::SetGBK2312Code()
{
    QTextCodec *codec=QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
}

/*设置编码格式为UTF8*/
void myHelper::SetUTF8Code()
{
#if(QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);

#endif
}

/*设置皮肤样式*/
void myHelper::SetStyle(const QString &StyleName)
{
    QFile file(QString(":/image/qss/%1.qss").arg(StyleName));
    file.open(QFile::ReadOnly);
    QString qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);
    qApp->setPalette(QPalette(QColor("#F0F0F0")));
}

/*加载中文字符*/
void myHelper::SetChinese()
{
    QTranslator *translator = new QTranslator(qApp);
    translator->load(":/image/qt_zh_CN.qm");
    qApp->installTranslator(translator);
}



/*显示消息框   仅确认按钮*/
void myHelper::ShowMessageBoxInfo(QString info)
{
    frmMessageBox *msg = new frmMessageBox;
    msg->SetMessage(info,0);
    msg->exec();
}

/*
 *函数功能：显示错误框,仅确定按钮
 *输入参数：显示的消息内容
 *输出参数：无
 *返回值：无
*/
void myHelper::ShowMessageBoxError(QString info)
{
    frmMessageBox *msg = new frmMessageBox;
    msg->SetMessage(info,2);
    msg->exec();
}

//显示询问框,确定和取消按钮
int myHelper::ShowMessageBoxQuesion(QString info)
{
    frmMessageBox *msg = new frmMessageBox;
    msg->SetMessage(info,1);
    return msg->exec();
}

QString myHelper::ShowInputBox(QWidget *frm, QString info)
{
    bool ok;
    return QInputDialog::getText(frm,"提示",info,QLineEdit::Password,"",&ok);
}

/*
 *显示用户付款信息框，确认所填信息是否正确
*/
int myHelper::ShowMessageBoxPayInfo(QString info)
{
    frmMessageBox *msg = new frmMessageBox;
    msg->SetMessage(info,3);
    msg->exec();
}

/*
 *检验文件是否存在
*/
bool myHelper::FileIsExist(QString strFile)
{
    QFile tempFile(strFile);
    return tempFile.exists();
}

/*
 *复制文件
*/
bool myHelper::CopyFile(QString sourceFile,QString targetFile)
{
    if(FileIsExist(targetFile))
    {
        int ret = QMessageBox::information(0,"提示","文件已经存在,是否替换?",QMessageBox::Yes | QMessageBox::No);
        if(ret != QMessageBox::Yes)
        {
            return false;
        }
    }
    return QFile::copy(sourceFile,targetFile);
}

/*
 *函数功能：延时函数
 *输入参数：所要延时的时间
 *输出参数：无
 *返回值：无
*/
void myHelper::Sleep(int sec)
{
    QTime dieTime = QTime::currentTime().addMSecs(sec);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}

/*
 *函数功能：设置显示时间
 *输入参数：输入年，月，日，时，分秒
 *输出参数：将系统时间显示在对应控件上
*/
void myHelper::SetSystemDateTime(int year, int month, int day, int hour, int min, int sec)
{
    QProcess p(0);

    p.start("cmd");
    p.waitForStarted();
    p.write(QString("date %1-%2-%3\n").arg(year).arg(month).arg(day).toLatin1());
    p.closeWriteChannel();
    p.waitForFinished(1000);
    p.close();

    p.start("cmd");
    p.waitForStarted();
    p.write(QString("time %1:%2:%3.00\n").arg(hour).arg(min).arg(sec).toLatin1());
    p.closeWriteChannel();
    p.waitForFinished(1000);
    p.close();
}

/*
 *函数功能：登录数据记录
 *输入参数：数据表名，动作，内容，触发者
*/
void myHelper::MyLoginBlog(QString tablename, QString Trigger, QString BlogContent, QString UserName)
{
    QSqlQuery query;
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    qDebug() <<currentDateTime;

    QString sql = "insert into "+tablename+" values(:TriggerTimer,:Trigger,:TriggretContent,:TriggetUser)";
    //qDebug() <<sql;
    query.prepare(sql);
    query.bindValue(":TriggerTimer",currentDateTime);
    query.bindValue(":Trigger",Trigger);
    query.bindValue(":TriggerContent",BlogContent);
    query.bindValue(":TriggerUser",UserName);
    query.setForwardOnly(true);
    query.exec();
}
