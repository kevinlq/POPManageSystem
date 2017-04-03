/***************************************************************
 *Copyright(c) 2014-2015 Company By LH Unigress
 *All rights reserved.
 *文件名称：登录日志头文件
 *简要描述：保存登录时的状态信息
 *
 *当前版本：V2.0
 *作者：Kelvin Li
 *创作日期：2014/10
 *说明：
*****************************************************************/
#ifndef MYAPP_H
#define MYAPP_H

#include "myhelper.h"

class Myapp
{
public:
    Myapp();
    static QString AppPath;                 //应用程序路径
    static QString CurrentImage;            //设置当前主界面对应图片

    static QString CurrentUserName;         //当前用户名
    static QString CurrentUserPwd;          //当前用户密码
    static QString CurrentUserType;         //当前用户类型(管理员、售货员……)
    static QString CurrentUserId;           //当前用户的编号

    static QString LastLoginter;            //最后一次登录用户名，用来自动登录时加载的用户信息

    static QString AppTitle;                //软件标题
    static QString CompanyName;             //公司名称
    static QString Contacts;                //联系人
    static QString Address;                 //公司地址
    static QString PhoneNumber;             //电话号码


    static void ReadConfig();               //读取配置文件,在main函数最开始加载程序载入
    static void WriteConfig();              //写入配置文件,在更改配置文件、程序关闭时调用
};

#endif // MYAPP_H
