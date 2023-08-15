#include "menuconfig.h"

MenuConfig::MenuConfig() {

}

MenuConfig::~MenuConfig() {
    //将QList<MenuConfig *>delete掉
    QList<MenuConfig*>::iterator iter = this->subMenus.begin();
    for (; iter != this->subMenus.end(); iter++) {
        if ((*iter) != nullptr) {
            delete (*iter);
            *iter = nullptr;
        }
    }
}
