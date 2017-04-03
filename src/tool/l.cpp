#include"l.h"

/*
 * log
*/
L::L(){}

L::~L(){}
QString L::DOT=",";

void  L::i(QString str){
    qDebug()<<"info    "<<str;
}

void  L::d(QString str){
    qDebug()<<"debug   "<<str;
}
void  L::e(QString str){
    qDebug()<<"error   "<<str;
}

void  L::i(QString tag,QString str){
    qDebug()<<"info    " <<tag<<"  "<<str;
}

void  L::d(QString tag,QString str){
    qDebug()<<"debug   " <<tag<<"  "<<str;
}


void  L::e(QString tag,QString str){
    qDebug()<<"error   " <<tag<<"  "<<str;
}

QString L::wraper(QString str){
    return "'"+str+"'";
}

QString L::wraper(int id){
    return QString::number(id);
}

QString L::w(QString str){
    return "'"+str+"'";
}

QString L::w(int id){
    return QString::number(id);
}
QString L::wd(QString str){
    return "'"+str+"'"+DOT;
}

QString L::wd(int id){
    return QString::number(id)+DOT;
}

/*
    static void L::e(string str){};
    static void L::d(string str){};
    static void L::i(string str){};

    static void e(string tag,string str){};
    static void d(string tag,string str){};
    static void i(string tag,string str){};

*/


