#include "qapplication.h"
#include "ECS/XTankWorld.h"
#include "QtWidget/PaintWidget.h"
#include <thread>

void StartWorld() {
    XTankWorld xTankWorld;
    xTankWorld.Init();
    xTankWorld.Start();
}

int main(int argc, char* argv[]) {

    QApplication a(argc, argv);

    std::thread logicThread(StartWorld);

    PaintWidget paintWidget;
    paintWidget.show();

    int res = a.exec();

    logicThread.join();

    return res;
}