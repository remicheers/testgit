
#include <QCoreApplication>
#include <SystemTray.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SystemTray *systemtray = new SystemTray(nullptr);
    systemtray->setToolTip("CubeTray");
    systemtray->show();
    return a.exec();
}
