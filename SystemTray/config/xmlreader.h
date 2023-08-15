#ifndef XMLREADER_H
#define XMLREADER_H
#include <QString>
#include <QtXml/QDomDocument>
//#include <QDomElement>
#include "rootconfig.h"
#include "trayiconconfig.h"
#include "menuconfig.h"

class XmlReader {
public:
    XmlReader();
    //传参是文件名，返回值是RootConfig类型指针
    RootConfig* ReadXmlFile(QString& filename);
    void getNodes(MenuConfig* menuConfig, QDomNode node);

};

#endif // XMLREADER_H
