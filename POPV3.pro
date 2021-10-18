#-------------------------------------------------
#
# Project created by QtCreator 2015-07-13T13:50:24
#
#-------------------------------------------------
CONFIG += QT
QT += core gui
QT += sql
QT   +=printsupport
TARGET = POPV3

include(src/stylewidget/stylewidget.pri)
include(src/widget/contentwidget/contentwidget.pri)
include(src/widget/prosealwidget/prosealwidget.pri)
include(src/widget/purchasewidget/PurchaseWidget.pri)
include(src/widget/stockmanagewidget/stockmanagewidget.pri)
include(src/widget/membermanagewidget/membermanagewidget.pri)
include(src/database/sqlhelp.pri)
include(src/help/help.pri)
include(src/widget/basicdatawidget/basicdatawidget.pri)
include(src/widget/loginwidget/loginwidget.pri)
include(src/serverhelp/serverhelp.pri)
include(src/widget/salesstatisticwidget/salesstatisticwidget.pri)
include(src/widget/systemsetdialog/systemsetdialog.pri)
include(src/tool/tool.pri)

INCLUDEPATH += $$PWD/src/stylewidget
INCLUDEPATH += $$PWD/src/widget/contentwidget
INCLUDEPATH += $$PWD/src/widget/prosealwidget
INCLUDEPATH += $$PWD/src/widget/purchasewidget
INCLUDEPATH += $$PWD/src/widget/stockmanagewidget
INCLUDEPATH += $$PWD/src/widget/membermanagewidget
INCLUDEPATH += $$PWD/src/database
INCLUDEPATH += $$PWD/src/help
INCLUDEPATH += $$PWD/src/widget/basicdatawidget
INCLUDEPATH += $$PWD/src/widget/loginwidget
INCLUDEPATH += $$PWD/src/widget/salesstatisticwidget
INCLUDEPATH += $$PWD/src/serverhelp
INCLUDEPATH += $$PWD/src/widget/systemsetdialog
INCLUDEPATH += $$PWD/src/tool

HEADERS += \
    src/start/mainwindow.h

SOURCES += \
    src/start/main.cpp \
    src/start/mainwindow.cpp

MOC_DIR = temp/moc
RCC_DIR = temp/rccr
OBJECTS_DIR = temp/obj
DESTDIR = bin

win32:RC_FILE = main.rc
RESOURCES += \
    image.qrc


