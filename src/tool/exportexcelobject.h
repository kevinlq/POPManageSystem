///////////////////////////////////////////////////////////////////////////////

#ifndef EXPORTEXCELOBJECT_H
#define EXPORTEXCELOBJECT_H

#include <QObject>
#include <QTableView>
#include <QStringList>
#include <QSqlDatabase>

class EEO_Field
{
public:
    EEO_Field(const int ic, const QString &sf, const QString &st):
            iCol(ic),sFieldName(sf),sFieldType(st){}

    int     iCol;
    QString sFieldName;
    QString sFieldType;
};

class ExportExcelObject : public QObject
{
    Q_OBJECT
public:
    ExportExcelObject(const QString &filepath, const QString &sheettitle,
                      QTableView *tableview):excelFilePath(filepath),
                      sheetName(sheettitle), tableView(tableview){}

    ~ExportExcelObject() {QSqlDatabase::removeDatabase("excelexport");}

public:
    void setOutputFilePath(const QString &spath)
    {excelFilePath = spath;}
    void setOutputSheetTitle(const QString &ssheet)
    {sheetName = ssheet;}
    void setTableView(QTableView *tableview)
    {tableView = tableview;}

    void addField(const int iCol, const QString &fieldname, const QString &fieldtype)
         {fieldList << new EEO_Field(iCol, fieldname, fieldtype);}

    void removeAllFields()
         {while (!fieldList.isEmpty()) delete fieldList.takeFirst();}

    int export2Excel();

signals:
    void exportedRowCount(int row);

private:
    QString excelFilePath;
    QString sheetName;
    QTableView *tableView;
    QList<EEO_Field *> fieldList;
};

#endif // EXPORTEXCELOBJECT_H
