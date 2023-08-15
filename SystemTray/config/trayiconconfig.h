#ifndef TRAYICONCONFIG_H
#define TRAYICONCONFIG_H

#include <QString>

//<trayIcon iconPath="./image/trayLogo.png"></trayIcon>
class TrayIconConfig
{
public:
    TrayIconConfig();

    //托盘菜单图标路径
    QString iconPath;
};

#endif // TRAYICONCONFIG_H
