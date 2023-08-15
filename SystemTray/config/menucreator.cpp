#include "menucreator.h"
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QString>
#include <QFile>
#include <QAction>
#include <QFileInfo>
#include <QMessageBox>
#include <QUrl>
#include <QDesktopServices>
#include "rootconfig.h"
#include "menuconfig.h"
#include "trayiconconfig.h"

MenuCreator::MenuCreator(QWidget* parent, QString xmlPath) {
    //父窗体
    this->parentWidget = parent;

    this->translator = new QTranslator();

    //实例化一个ReadXml实例
    this->xmlReader = new XmlReader();

    //从配置文件从读取xml路径
    /*QSettings setting("./config.ini", QSettings::IniFormat);
    setting.beginGroup("config");
    //xml文件路径
    QString xmlPath=setting.value("trayConfigPath").toString();
    setting.endGroup();*/

    //将xml文件路径作为ReadXmlFile的参数，返回的是RootConfig类类型指针
    this->root = this->xmlReader->ReadXmlFile(xmlPath);

}

/**
 * @brief MenuCreator::getTrayIconPath
 * 函数功能：返回从xml配置文件定义的托盘图标。
 */

QString MenuCreator::getTrayIconPath() {
    //得到托盘图标路径
    QString trayIconPath = "";
    //如果xml为有效xml路径的话，则获取iconPath
    if (this->root->trayIconConfig.iconPath != "") { //xml抛弃 针对对象 this->root->trayIconConfig->iconPath判断
        trayIconPath = this->root->trayIconConfig.iconPath;
    }
    QImage iconImage(trayIconPath);
    //如果托盘图标路径不存在，则设置托盘图标为:/SystemTray/resources/Logo.png
    if (iconImage.isNull()) {
        trayIconPath = ":/SystemTray/resources/trayLogo1.png";
    }
    return trayIconPath;
}

/**
 * @brief MenuCreator::create
 * 函数功能：创建托盘菜单项。
 * @param menu：表示创建菜单的父菜单
 */

void MenuCreator::create(QMenu* menu) {
    for (int i = 0; i < this->root->menuConfig.count(); i++) {
        int kind = this->root->menuConfig.at(i)->kind;
        //0如果为分割符
        if (kind == 0) {
            menu->addSeparator();
        } else {
            QAction* action = new QAction(menu);
            menu->addAction(action);
            action->setText(root->menuConfig.at(i)->id);
            action->setIcon(QIcon(root->menuConfig.at(i)->img));

            //1表示通用的菜单项
            if (kind == 1) {
                //如果不是信号和槽，使用qRegisterMetaType注册数据类型
                QString slotFunc = root->menuConfig.at(i)->slot;
                //为了connect的槽函数中无参，用std::bind将转换成无参的函数func
                //auto func=std::bind(&MenuCreator::fun1,this,slotFunc);
                //connect(action, &QAction::triggered, this,func);
            }
            //2表示插件
            else if (kind == 2) {

            }
            //3表示外部程序
            else if (kind == 3) {
                //从xml中获取工作目录和.exe路径
                QString dirPath = root->menuConfig.at(i)->dirPath;
                QString exePath = root->menuConfig.at(i)->exePath;
                //将有参的函数变为无参的函数
                auto func = std::bind(&MenuCreator::exeFunc, this, dirPath, exePath);
                connect(action, &QAction::triggered, this, func);
            }
            //4表示网页
            else if (kind == 4) {
                QString url = root->menuConfig.at(i)->url;
                auto urlFun = std::bind(&MenuCreator::urlFunc, this, url);
                connect(action, &QAction::triggered, this, urlFun);
            }
            this->getNode(root->menuConfig.at(i), action);
        }
    }

}

//遍历.xml配置文件，动态生成托盘菜单项
void MenuCreator::getNode(MenuConfig* menu, QAction* p) {
    //如果为空指针的话，则返回
    if (menu == nullptr) {
        return;
    }
    QList<MenuConfig*> list = menu->subMenus;
    int count = list.count();
    if (count <= 0) {
        return;
    }
    //当当前节点存在子节点时，则创建一个菜单
    QMenu* newmenu = new QMenu(this->parentWidget);
    for (int i = 0; i < count; i++) {
        MenuConfig* menuconfig = list.at(i);
        int kind = menuconfig->kind;
        //0如果为分割符
        if (kind == 0) {
            newmenu->addSeparator();
        } else {
            QAction* action = new QAction(newmenu);
            newmenu->addAction(action);
            action->setText(menuconfig->id);
            action->setIcon(QIcon(menuconfig->img));

            //1表示通用的菜单项
            if (kind == 1) {
                //如果不是信号和槽，使用qRegisterMetaType注册数据类型
                QString slotFunc = menuconfig->slot;
                logger()->info() << "slot:" << slotFunc;
                fun1(slotFunc, action);
                //为了connect的槽函数中无参，用std::bind将转换成无参的函数func
                //auto func=std::bind(&MenuCreator::fun1,this,slotFunc);
                //connect(action, &QAction::triggered, this,func);
            }
            //2表示插件
            else if (kind == 2) {

            }
            //3表示外部程序
            else if (kind == 3) {
                //从xml中获取工作目录和.exe路径
                QString dirPath = menuconfig->dirPath;
                QString exePath = menuconfig->exePath;
                //将有参的函数变为无参的函数
                auto func = std::bind(&MenuCreator::exeFunc, this, dirPath, exePath);
                connect(action, &QAction::triggered, this, func);
            }
            //4表示网页
            else if (kind == 4) {
                QString url = menuconfig->url;
                auto urlFun = std::bind(&MenuCreator::urlFunc, this, url);
                connect(action, &QAction::triggered, this, urlFun);
            }
            this->getNode(menuconfig, action);
        }
    }
    p->setMenu(newmenu);
}

void MenuCreator::clickChinese() {
    if (this->translator->load("./resources/tr_zh.qm")) {
        qApp->installTranslator(this->translator);
        logger()->info() << "国际化为中文成功";
    } else {
        logger()->info() << "国际化为中文失败";
    }
}


void MenuCreator::clickEnglish() {
    if (this->translator->load("./resources/tr_en.qm")) {
        qApp->installTranslator(this->translator);
        logger()->info() << "国际化为英文成功";
    } else {
        logger()->info() << "国际化为英文失败";
    }
}

MenuCreator::~MenuCreator() {
    delete this->root;
    delete this->translator;
    delete this->xmlReader;
}

void MenuCreator::fun1(QString& funcName, QAction* action) {
//    if (funcName == "MainWindow::serverClicked") {
//        connect(action, &QAction::triggered, (MainWindow*)this->parentWidget, &MainWindow::serverClicked);
//    } else if (funcName == "MainWindow::clientClicked") {
//        connect(action, &QAction::triggered, (MainWindow*)this->parentWidget, &MainWindow::clientClicked);
//    } else
    if (funcName == "MenuCreator::clickChinese") {
        connect(action, &QAction::triggered, this, &MenuCreator::clickChinese);

    } else if (funcName == "MenuCreator::clickEnglish") {
        connect(action, &QAction::triggered, this, &MenuCreator::clickEnglish);
    }
}

void MenuCreator::quitApp() {
    qApp->quit();
}

//函数功能：调用外部程序
void MenuCreator::exeFunc(QString& dirPath, QString& exePath) {
    //创建进程myProcess，用于调用外部程序
    QProcess* myProcess = new QProcess();
    //加载工作路径
    QFileInfo dir(dirPath);
    //如果为目录
    if (dir.isDir()) {
        //设置外部程序的工作路径
        myProcess->setWorkingDirectory(dirPath);
        logger()->info() << "托盘菜单调用外部程序设置的工作目录为:" << dirPath;
    } else {
        logger()->info() << "托盘菜单调用外部程序设置的工作路径不是一个有效的路径：" << dirPath;
    }
    //加载.exe文件路径
    QFileInfo exeFile(exePath);
    if (exeFile.isFile()) {
        myProcess->start(exePath);//startDetached
    } else {
        logger()->info() << ".exe文件路径不是一个有效的路径" << exePath;
        QMessageBox::information(NULL, "消息提示框", exePath + ": " + "不是有效的路径");
    }
    auto deleteFun = std::bind(&MenuCreator::deleteProcess, this, myProcess);
    //当myProcess结束的时候，调用deleteProcess槽函数
    connect(myProcess, &QProcess::finished, this, deleteFun);
}

//打开网页 传参为对应的url
void MenuCreator::urlFunc(QString& url) {
    //打开链接为url的网页
    bool res = QDesktopServices::openUrl(QUrl(url));
    if (res) { //成功情况
        logger()->info() << "打开" << url << "执行成功";
    } else { //失败情况
        //QMessageBox::information(NULL,"消息提示框",url+": "+"不是正确的url路径");
        logger()->info() << "打开" << url << "执行失败";
    }
}

//删除创建外部程序的指针
void MenuCreator::deleteProcess(QProcess* pro) {
    //当pro不为nullptr的时候
    if (pro) {
        delete pro;
    }
}

