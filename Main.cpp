#include "qapplication.h"
#include "ECS/XTankWorld.h"
#include "QtWidget/PaintWidget.h"
#include <thread>
#include "Thread/ThreadManager.h"


int main(int argc, char* argv[]) {

    QApplication a(argc, argv);

    ThreadManager::Instance().InvokeGameThread();

    PaintWidget paintWidget;
    paintWidget.SetWorld(ThreadManager::Instance().GetXTankWorld());
    paintWidget.show();

    int res = a.exec();

    ThreadManager::Instance().ShutDownGameThread();

    return res;
}