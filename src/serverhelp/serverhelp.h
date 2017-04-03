#ifndef SERVERHELP_H
#define SERVERHELP_H
#include <QtCore>

class ServerHelp
{
public:
    ~ServerHelp();
    ServerHelp(QString userName,QString userPwd);
    void SetUserName(const QString &);
    void SetUserPwd(const QString &);
    QString GetUserName();
    QString GetUserPwd();

private:
    QString m_pUserName;
    QString m_pUserPwd;
    
};

#endif // SERVERHELP_H
