QT  +=gui core
QT  +=widgets
QT  +=sql
QT  +=network

TARGET = OrderSystemService
TEMPLATE = app

include(src/mainwidget/mainwidget.pri)
include(src/help/help.pri)
include(src/database/database.pri)
include(src/protocol/protocol.pri)

INCLUDEPATH += $$PWD/src/mainwidget
INCLUDEPATH += $$PWD/src/help
INCLUDEPATH += $$PWD/src/database
INCLUDEPATH += $$PWD/src/protocol

MOC_DIR = temp/moc
RCC_DIR = temp/rccr
OBJECTS_DIR = temp/obj
UI_HEADERS_DIR = temp/ui
DESTDIR = bin

RESOURCES += \
    image.qrc
win32:RC_FILE=main.rc

FORMS += \
    orderinfowidget.ui

HEADERS += \
    orderinfowidget.h

SOURCES += \
    orderinfowidget.cpp
