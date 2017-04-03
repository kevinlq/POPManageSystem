#include "exportexcelobject.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QVariant>

/*
 * 用QODBC 链接Excel
 * 然后对QTableView执行SQL语句来处理数据
*/


int ExportExcelObject::export2Excel()
{
    if(fieldList.size() <= 0)
    {
        qDebug() << "ExportExcelObject::export2Excel failed: No fields defined.";
        return -1;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "excelexport");
    if(!db.isValid())
    {
        qDebug() << "ExportExcelObject::export2Excel failed: QODBC not supported.";
        return -2;
    }
    // set the dsn string
    QString dsn = QString("DRIVER={Microsoft Excel Driver (*.xls)};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%1\";DBQ=%2").
                  arg(excelFilePath).arg(excelFilePath);
    db.setDatabaseName(dsn);
    if(!db.open())
    {
        qDebug() << "ExportExcelObject::export2Excel failed: Create Excel file failed by DRIVER={Microsoft Excel Driver (*.xls)}.";
        //QSqlDatabase::removeDatabase("excelexport");
        return -3;
    }

    QSqlQuery query(db);

    //drop the table if it's already exists
    QString s, sSql = QString("DROP TABLE [%1] (").arg(sheetName);
    query.exec(sSql);

    //create the table (sheet in Excel file)
    sSql = QString("CREATE TABLE [%1] (").arg(sheetName);
    for (int i = 0; i < fieldList.size(); i++)
    {
        s = QString("[%1] %2").arg(fieldList.at(i)->sFieldName).arg(fieldList.at(i)->sFieldType);
        sSql += s;
        if(i < fieldList.size() - 1)
            sSql += " , ";
    }

    sSql += ")";
    query.prepare(sSql);

    if(!query.exec())
    {
        qDebug() << "ExportExcelObject::export2Excel failed: Create Excel sheet failed.";
        //db.close();
        //QSqlDatabase::removeDatabase("excelexport");
        return -4;
    }

    //add all rows
    sSql = QString("INSERT INTO [%1] (").arg(sheetName);
    for (int i = 0; i < fieldList.size(); i++)
    {
        sSql += fieldList.at(i)->sFieldName;
        if(i < fieldList.size() - 1)
            sSql += " , ";
    }
    sSql += ") VALUES (";
    for (int i = 0; i < fieldList.size(); i++)
    {
        sSql += QString(":data%1").arg(i);
        if(i < fieldList.size() - 1)
            sSql += " , ";
    }
    sSql += ")";

    qDebug() << sSql;

    int r, iRet = 0;
    for(r = 0 ; r < tableView->model()->rowCount() ; r++)
    {
        query.prepare(sSql);
        for (int c = 0; c < fieldList.size(); c++)
        {
            query.bindValue(QString(":data%1").arg(c), tableView->model()->data(tableView->model()->index(r, fieldList.at(c)->iCol)));
        }

        if(query.exec())
            iRet++;

        if(r % 10 == 0)
            emit exportedRowCount(r);
    }

    emit exportedRowCount(r);

    return iRet;
}
