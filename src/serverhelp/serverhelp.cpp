#include "serverhelp.h"


ServerHelp::ServerHelp(QString userName, QString userPwd):
    m_pUserName(userName),m_pUserPwd(userPwd)
{
}

void ServerHelp::SetUserName(const QString &userName)
{
    this->m_pUserName = userName;
}

void ServerHelp::SetUserPwd(const QString &userPwd)
{
    this->m_pUserPwd = userPwd;
}

QString ServerHelp::GetUserName()
{
    return m_pUserName;
}

QString ServerHelp::GetUserPwd()
{
    return m_pUserPwd;
}

