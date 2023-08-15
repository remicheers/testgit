QT += core
QT += xml

CONFIG += c++17
DESTDIR += bin
TEMPLATE = lib
CONFIG += plugin
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia
# TODO: 此插件在编译 release 版本时，需要把优化选项关闭才能正常加载。后续需要排查一下原因。
QMAKE_CXXFLAGS_RELEASE += -O0

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($${PWD}/../../Common/Logger/log4qt.pri)
include($${PWD}/../../Common/PluginInterFace/PluginInterFace.pri)
include($${PWD}/../../Common/PluginsRegistry/PluginRegistry.pri)
include($${PWD}/../../Common/MenuManager/MenuManager.pri)
include($${PWD}/../../Common/MenuConsumer/MenuConsumer.pri)


SOURCES += \
    config/menucreator.cpp \
    config/rootconfig.cpp \
    config/trayiconconfig.cpp \
    config/menuconfig.cpp \
    config/xmlreader.cpp \
    SystemTray.cpp \


HEADERS += \
    config/menucreator.h \
    config/rootconfig.h \
    config/trayiconconfig.h \
    config/menuconfig.h \
    config/xmlreader.h \
    SystemTray.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

copy_config.files = \
     $${PWD}/config.ini
copy_config.path  = $${OUT_PWD}/$${DESTDIR}
COPIES += copy_config


# 图标
RC_ICONS = ./resources/trayLogo.ico

RESOURCES += \
    resources.qrc


