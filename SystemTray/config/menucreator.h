#ifndef MENUCREATOR_H
#define MENUCREATOR_H
#include <QString>
#include "xmlreader.h"
#include <QMenu>
#include <QCoreApplication>
#include <QApplication>
#include <QTranslator>
#include <functional>
#include <QMetaObject>
#include <QSystemTrayIcon>
#include <QProcess>
#include "log4qt/logger.h"

using namespace std;

class MenuCreator: public QObject {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    MenuCreator(QWidget* parent, QString xmlPath);
    ~MenuCreator();
    QString getTrayIconPath();
    void create(QMenu* menu);
    void getNode(MenuConfig*, QAction* p);
    Q_INVOKABLE void clickChinese();//中文国际化
    Q_INVOKABLE void clickEnglish();//英文国际化
    void fun1(QString& funcName, QAction* a);

    Q_INVOKABLE void quitApp();//希望quitApp能够被反射，只需要在类的函数声明前加入Q_INVOKABLE关键字
    void exeFunc(QString& dirPath, QString& exePath);
    void urlFunc(QString& url);
    void deleteProcess(QProcess* pro);

    //xml文件读取
    XmlReader* xmlReader;
    //rootconfig
    RootConfig* root;
    QTranslator* translator;

    QWidget* parentWidget;//父窗体
};

#endif // MENUCREATOR_H
