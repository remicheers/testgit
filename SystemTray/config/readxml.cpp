#include "readxml.h"
#include <QFile>
#include <QDomDocument>

ReadXml::ReadXml()
{

}

RootConfig* ReadXml::ReadXmlFile(QString &filename){
    //创建一个TrayIconConfig对象
    RootConfig *result=new RootConfig();
    //先加载文件
    QFile xmlFile(filename);
    //判断文件是否存在
    if(!xmlFile.exists()){
        return result;
    }
    //当打开不成功时
    if(!xmlFile.open(QFile::ReadOnly)){
        return result;
    }
    QDomDocument doc;
    if(!doc.setContent(&xmlFile)){
        xmlFile.close();
        return result;
    }
    xmlFile.close();
    //XML root element
    //得到根节点元素
    QDomElement root=doc.documentElement();
    //得到根节点的孩子节点
    QDomNodeList list=root.childNodes();
    int count=list.count();
    if(count<=0)
        return result;
    //遍历孩子节点
    for(int i=0;i<count;i++){
        QDomNode node=list.at(i);
        //讲节点转化为QDomElement类型
        QDomElement e=node.toElement();
        if(e.nodeName()==("trayIcon")){
            //创建一个TrayIconConfig对象
            TrayIconConfig *trayConfig=new TrayIconConfig();
            trayConfig->iconPath=e.attribute("iconPath");
            result->trayIconConfig=trayConfig;
        }
        else if(e.nodeName()=="menu"){
            MenuConfig *menuConfig=new MenuConfig();
            this->getNodes(menuConfig,node);
            result->menuConfig.append(menuConfig);
        }
    }
    return result;
}

void ReadXml::getNodes(MenuConfig *menu,QDomNode node){
    if(node.isNull()){
        return;
    }
    if(node.nodeName()=="menu"){
        QDomElement e=node.toElement();
        //创建一个menu对象
        //MenuConfig *menu=new MenuConfig();
        //种类
        menu->kind=e.attribute("class").toInt();
        //id名称
        menu->id=e.attribute("id");
        //槽函数
        menu->slot=e.attribute("slot");
        //图片路径
        menu->img=e.attribute("img");
        //url
        menu->url=e.attribute("url");
        //目录
        menu->dirPath=e.attribute("dirPath");
        //exe文件路径
        menu->exePath=e.attribute("exePath");
        //中文
        menu->zh=e.attribute("zh");
        //繁体中文
        menu->tzh=e.attribute("tzh");
        //英文
        menu->english=e.attribute("english");
    }

    QDomNodeList list=node.childNodes();
    if(list.count()>0){
        for(int i=0;i<list.count();i++){
            MenuConfig *menuConfig=new MenuConfig();
            //得到第i个节点
            QDomNode node=list.at(i);
            menu->subMenus.append(menuConfig);
            getNodes(menuConfig,node);
        }
    }
}
