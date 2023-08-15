#ifndef READXML_H
#define READXML_H
#include<QString>
#include<QDomElement>
#include "rootconfig.h"
#include "trayiconconfig.h"
#include "menuconfig.h"

class ReadXml
{
public:
    ReadXml();
    //传参是文件名，返回值是RootConfig类型指针
    RootConfig *ReadXmlFile(QString &filename);
    void getNodes(MenuConfig *menuConfig,QDomNode node);

};

#endif // READXML_H
