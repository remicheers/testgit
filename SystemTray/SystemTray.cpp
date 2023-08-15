
#include "SystemTray.h"
#include <QApplication> //qAPP
#include <QDomDocument>
#include <QFile>
#include <QProcess>
#include <QFileInfo>
#include <functional>
#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QSharedPointer>


static SystemTray* s_SystemTray = nullptr;

SystemTray::SystemTray(QWidget* parent)
    :QSystemTrayIcon((QObject*)parent)
{
    this->pluginInfo.name = "SystemTray";
    this->pluginInfo.type = "dll";
    this->pluginInfo.initStatus = true;
    this->configPath = "config.ini";
    //    if (parent) {
    //        //配置事件
    //        //        connect(defaultAction, &QAction::triggered, (MainWindow*)parent, &MainWindow::defaultConfigClicked);
    //        //        connect(serverAction, &QAction::triggered, (MainWindow*)parent, &MainWindow::serverClicked);
    //        //        connect(clientAction, &QAction::triggered, (MainWindow*)parent, &MainWindow::clientClicked);
    //        connect(showMainAction, &QAction::triggered, parent, &QWidget::showNormal);  // "显示主界面"菜单响应事件。
    //        connect(this->aboutAction, &QAction::triggered, (MainWindow*)parent, &MainWindow::showAboutDialog); // "关于"菜单响应事件。


    //        // 语言切换事件
    //        connect(langZHAction, &QAction::triggered, (MainWindow*)parent, [=](){ ((MainWindow*)parent)->languageChanged("zh_CN"); });
    //        connect(langENAction, &QAction::triggered, (MainWindow*)parent, [=](){ ((MainWindow*)parent)->languageChanged("en_US"); });
    //        connect(langTWAction, &QAction::triggered, (MainWindow*)parent, [=](){ ((MainWindow*)parent)->languageChanged("zh_TW"); });
    //    }
}

void SystemTray::initTray() {
    parentWidget = parent;
    // 初始化菜单
    this->trayMenu = new QMenu(parent);

    // 显示主界面菜单
    QAction* showMainAction = new QAction(parent);
    showMainAction->setText(tr("Main Menu"));
    showMainAction->setIcon(QIcon(":/SystemTray/resources/home.png"));
    this->trayMenu->addAction(showMainAction);
    this->separator = this->trayMenu->addSeparator();

    QSharedPointer<MenuManager> menuManager = MenuManager::getInstance();
    menuManager->addConsumer("SystemTray",this);




//    this->addMenu(menu);
//    std::unordered_map<std::string,MenuConsumer*> consumerList = *(menuManager->getConsumerList());
//    for(auto it = consumerList.begin(); it != consumerList.end(); ++it) {
//        logger()->info() << "menu name: " << it->first.c_str();
//        it->second->addMenu("SystemTray",menu);
//    }

    // 分隔符
//    this->separator = this->trayMenu->addSeparator();


    // 语言选项
//    QAction* langZHAction = new QAction(parent);
//    langZHAction->setText(tr("chinese"));
//    langZHAction->setIcon(QIcon(":/SystemTray/resources/chinese.png"));
//    QAction* langENAction = new QAction(parent);
//    langENAction->setText(tr("english"));
//    langENAction->setIcon(QIcon(":/SystemTray/resources/english.png"));
//    QAction* langTWAction = new QAction(parent);
//    langTWAction->setText(tr("Chinese Traditional"));
//    langTWAction->setIcon(QIcon(":/SystemTray/resources/chinese.png"));
//    this->langMenu = new QMenu(parent);
//    this->langMenu->setTitle(tr("language"));
//    this->langMenu->setIcon(QIcon(":/SystemTray/resources/language.png"));
//    this->langMenu->addAction(langZHAction);
//    this->langMenu->addAction(langENAction);
//    this->langMenu->addAction(langTWAction);
//    this->trayMenu->addMenu(this->langMenu);

    // 读取配置文件路径
//    QString xmlPath = "F:/Desktop/SystemTray/tray/fileBrowser1.xml";
//    logger()->info() << "config/trayConfigPath: " << xmlPath;

    // 创建一个MenuCreator
    this->menuCreator = new MenuCreator(parentWidget, "");
    // 获得托盘图标
    QString trayIconPath = this->menuCreator->getTrayIconPath();
//    logger()->info() << "trayIconPath: " << trayIconPath;
    // 设置托盘图标
    this->setIcon(QIcon(trayIconPath));

    this->menuCreator->create(this->trayMenu);

    // 关于菜单
//    this->aboutAction = new QAction(parent);
//    this->aboutAction->setText(tr("About"));
//    this->aboutAction->setIcon(QIcon(":/SystemTray/resources/about.png"));
//    this->trayMenu->addAction(this->aboutAction);

    // 退出菜单
    this->quitAction = new QAction(parent);
    this->quitAction->setText(tr("Exit"));
    this->quitAction->setIcon(QIcon(":/SystemTray/resources/exit.png"));
    this->trayMenu->addAction(this->quitAction);

    //设置this->trayMenu为系统托盘的上下文菜单
    this->setContextMenu(this->trayMenu);

    // 事件绑定
    connect(this, &QSystemTrayIcon::activated, this, &SystemTray::customActivated);  // 使用自定义的事件处理函数。
    connect(this->quitAction, &QAction::triggered, this, &SystemTray::quitApp); // "退出"菜单响应事件。
    connect(showMainAction, &QAction::triggered, this, &SystemTray::showMainMenu); // 展示插件列表响应事件。



    this->setToolTip("CubeTray");
    this->show();
    this->showMessage(tr("Friendly reminder"), tr("The tray program is on"), QSystemTrayIcon::Information, 3000);
}

void SystemTray::translateUI(){
    QList <QAction*> actionList = this->trayMenu->actions();
    QList <QAction*> actionList1 = this->langMenu->actions();
    actionList[0]->setText(tr("Main Menu"));
    this->aboutAction->setText(tr("about"));
    this->quitAction->setText(tr("exit"));
    this->langMenu->setTitle(tr("language"));
    actionList1[0]->setText(tr("chinese"));
    actionList1[1]->setText(tr("english"));
    actionList1[2]->setText(tr("Chinese Traditional"));
    logger()->info()<<"执行translateUI";
}

/**
 * @brief SystemTray::~SystemTray
 * 析构函数。清理资源。
 */
SystemTray::~SystemTray() {
    delete this->menuCreator;
}

/**
 * @brief SystemTray::customActivated
 * 自定义事件处理函数。
 * @param reason
 */
void SystemTray::customActivated(QSystemTrayIcon::ActivationReason reason) {
    if (parentWidget) {
        switch (reason) {
        case QSystemTrayIcon::Context:
            break;
        case QSystemTrayIcon::Trigger:
            break;
        case QSystemTrayIcon::DoubleClick:
            //  双击托盘显示主界面。
            if (parentWidget) {
                parentWidget->showNormal();
            }
            break;
        default:
            break;
        }
    }
}

/**
 * @brief SystemTray::quitApp
 * 退出程序。
 */
//quitApp
void SystemTray::quitApp() {
    this->hide();
    this->showMainMenu(true);
    qApp->quit();
}

void SystemTray::showMainMenu(bool isClose) {
    logger()->info() << "click MainMenu";
    //读取配置文件信息
    int epos = this->rootPath.lastIndexOf("/");
    QString rootPath = this->rootPath.mid(0,epos);
    QScopedPointer<QSettings> config(new QSettings(rootPath + "/" + this->configPath,QSettings::IniFormat));
    QString dirPath = config->value("config/dirPath").toString();
    QString exePath = config->value("config/exePath").toString();
    if(isRunning == false && isClose == false) {
        this->mainMenu = new QProcess(nullptr);
        this->mainMenu->setWorkingDirectory(dirPath);
        this->mainMenu->start(exePath);

        if(this->mainMenu->processId() >0 ) {
            logger()->info() << exePath << " is running!";
            isRunning = true;
        }
        else
        {
            logger()->info() << exePath << " is not running!";
        }

        QObject::connect(this->mainMenu, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                         [this] (int exitCode, QProcess::ExitStatus exitStatus) {

                             logger()->warn() << "process was terminated! "
                                        << "Exit code: " << exitCode
                                        << "Exit status: " << exitStatus;

                             // restart
                             isRunning = false;
                         });
    }
    else if(isRunning == true && isClose == true)
    {

        logger()->info() << "quit mainMenu";
        this->mainMenu->kill();
    }
    else
    {
        logger()->info() << "the main Menu is shown or quit main program ";
    }




//    int state = pro->state();
//    if(state == 0) {
//        logger()->info() << exePath << "is not running!";
//    }
//    else {
//        logger()->info() << exePath << "is running!";
//    }



}



/**
 * @brief SystemTray::addMenu
 * 添加菜单
 */
void SystemTray::addMenu(QString name, QMenu* menu) {
    logger()->info() << "add menu: " << name;
    QAction *pluginAction = this->trayMenu->insertMenu(this->separator,menu);
    pluginMenus.insert(name, pluginAction);
    //把分隔符的变量给记录下来
}

/**
 * @brief SystemTray::removeMenu
 * 删除菜单
 */
void SystemTray::removeMenu(QString name) {
    logger()->info() << "remove menu: " << name;
    QAction* action = pluginMenus.value(name);
    this->trayMenu->removeAction(action);
    pluginMenus.remove(name);
}



bool SystemTray::initPlugin(QString rootPath) {
    this->parent = nullptr;
    this->rootPath = rootPath;
    this->initTray();
}

void SystemTray::destroyPlugin() {

}

bool SystemTray::enable() {

}

bool SystemTray::disable() {

}

QString SystemTray::getRootPath() {

}

bool SystemTray::isEnabled() {

}

bool SystemTray::run(int argc, char *argv[]) {
    return true;
}

const PluginInfo* SystemTray::getPluginInfo() {
    return &this->pluginInfo;
}



