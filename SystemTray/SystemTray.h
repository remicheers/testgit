/**
 * Copyright (c) 2019-2119,CubeGalaxy.com All rights reserved.
 */
#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QCoreApplication>
#include <QMenu>
#include <QAction>
#include <QTranslator>
#include <QEvent>
#include <QtXml/QDomDocument>
#include <QProcess>
#include <QMap>
#include "log4qt/logger.h"
#include "config/xmlreader.h"
#include "config/rootconfig.h"
#include "config/menucreator.h"
#include <PluginRegistry/pluginregistry.h>
#include <QWidget>
#include <MenuManager/menumanager.h>
#include <MenuConsumer/menuconsumer.h>

class SystemTray : public QSystemTrayIcon, public PluginInterface, public MenuConsumer
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_INTERFACE_IID)
    Q_INTERFACES(PluginInterface)
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit SystemTray(QWidget* parent = nullptr);
    ~SystemTray();
    bool initPlugin(QString rootPath);
    void destroyPlugin();
    bool enable();
    bool disable();
    bool isEnabled();
    QString getRootPath();
    bool run(int argc, char *argv[]);
    const PluginInfo *getPluginInfo();
    void addMenu(QString name, QMenu* menu);
    void removeMenu(QString name);
//    void addMenu(QMenu* menu);
//    void removeMenu(QMenu* menu);
    void translateUI();
    void initTray();
    QMenu* langMenu;  //语言菜单
    QProcess* mainMenu;


private slots:
    void customActivated(QSystemTrayIcon::ActivationReason reason);
    Q_INVOKABLE void quitApp(); // 希望quitApp能够被反射，只需要在类的函数声明前加入Q_INVOKABLE关键字
    void showMainMenu(bool isClose = false);

private:
    QWidget* parent;
    QString rootPath;
    QString configPath;
    QMenu *trayMenu;
    QAction* separator; //分隔符 用于添加插件菜单
    QAction* aboutAction; //关于菜单
    QAction* quitAction; //退出菜单
    QWidget *parentWidget;
    MenuCreator *menuCreator; // 用于生成托盘菜单

    QMap<QString, QAction*> pluginMenus;  //存放插件菜单
    QSharedPointer<PluginRegistry> pluginRegistry;  //插件注册管理类
    PluginInfo pluginInfo;
    bool isRunning = false;

    // void translateUI();
    // void changeEvent(QEvent *);
};

#endif // SYSTEMTRAY_H
