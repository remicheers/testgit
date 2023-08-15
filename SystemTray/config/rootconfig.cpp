#include "rootconfig.h"

RootConfig::RootConfig() {

}

RootConfig::~RootConfig() {
    //将QList<MenuConfig *>delete掉
    QList<MenuConfig*>::iterator iter = this->menuConfig.begin();
    for (; iter != this->menuConfig.end(); iter++) {
        if ((*iter) != nullptr) {
            delete (*iter);
            *iter = nullptr;
        }
    }
}
