QT  +=gui core
QT  +=widgets
QT  +=sql
QT  +=network

TARGET = OrderSystemService
TEMPLATE = app

include(src/mainwidget/mainwidget.pri)
include(src/help/help.pri)
include(src/ui/ui.pri)
include(src/database/database.pri)
include(src/tcpservice/tcpservice.pri)
include(src/protocol/protocol.pri)
include(src/ui/loginwidget/loginwidget.pri)

INCLUDEPATH += $$PWD/src/mainwidget
INCLUDEPATH += $$PWD/src/help
INCLUDEPATH += $$PWD/src/ui/orderinfowidget
INCLUDEPATH += $$PWD/src/ui/menuinfowidget
INCLUDEPATH += $$PWD/src/ui
INCLUDEPATH += $$PWD/src/database
INCLUDEPATH += $$PWD/src/tcpservice
INCLUDEPATH += $$PWD/src/protocol
INCLUDEPATH += $$PWD/src/ui/loginwidget

MOC_DIR = temp/moc
RCC_DIR = temp/rccr
OBJECTS_DIR = temp/obj
UI_HEADERS_DIR = temp/ui
DESTDIR = bin

RESOURCES += \
    image.qrc
win32:RC_FILE=main.rc
