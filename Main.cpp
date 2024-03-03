#include "qapplication.h"
#include "ECS/XTankWorld.h"
#include "QtWidget/PaintWidget.h"
#include <thread>

void StartWorld(XTankWorld* xTankWorld) {
    xTankWorld->Init();
    xTankWorld->Start();
}

int main(int argc, char* argv[]) {

    QApplication a(argc, argv);

    XTankWorld* xTankWorld = new XTankWorld();

    std::thread logicThread(StartWorld, xTankWorld);

    PaintWidget paintWidget;
    paintWidget.SetWorld(xTankWorld);
    paintWidget.show();

    int res = a.exec();

    logicThread.join();

    delete xTankWorld;

    return res;
}