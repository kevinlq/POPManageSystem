#ifndef L_H
#define L_H
#include<QDebug>
#include<QString>

using namespace std;

class L
{
private:
    L();
public:

    ~L();
    static void e(QString str);
    static void d(QString str);
    static void i(QString str);

    static  void e(QString tag,QString str);
    static  void d(QString tag,QString str);
    static  void i(QString tag,QString str);



    static  QString wraper(QString str);
    static  QString wraper(int id);
    static  QString w(QString str);
    static  QString w(int id);
    static  QString wd(QString str);
    static  QString wd(int id);

    static  QString DOT ;
    /*

    static void e(string str);
    static void d(string str);
    static void i(string str);
    static void e(string tag,string str);
    static void d(string tag,string str);
    static void i(string tag,string str);
    */
};
#endif // L_H


/*
 l.cpp
*/


