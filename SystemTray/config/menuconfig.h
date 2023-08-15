#ifndef MENUCONFIG_H
#define MENUCONFIG_H
#include <QString>
#include <QList>

class MenuConfig
{
public:
    MenuConfig();
    ~MenuConfig();

public:
    int kind;//类别
    QString id;//菜单项名称
    QString slot;//槽函数
    QString img;//图标路径
    QString url;//url
    QString dirPath;//工作目录
    QString exePath;//调用外部程序的exe路径
    QString zh;//中文
    QString tzh;//繁体中文
    QString english;//英文
    QList<MenuConfig *> subMenus;//<menu></menu>的孩子list
};

#endif // MENUCONFIG_H
