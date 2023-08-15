#ifndef ROOTCONFIG_H
#define ROOTCONFIG_H
#include "trayiconconfig.h"
#include "menuconfig.h"
#include <QList>


class RootConfig
{
public:
    RootConfig();
    ~RootConfig();

    //图标
    //TrayIconConfig *trayIconConfig;
    TrayIconConfig trayIconConfig;
    //托盘菜单项list
    QList<MenuConfig *> menuConfig;
};

#endif // ROOTCONFIG_H
