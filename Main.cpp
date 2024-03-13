#include "qapplication.h"
#include "ECS/XTankWorld.h"
#include "QtWidget/PaintWidget.h"
#include <thread>
#include "Thread/ThreadManager.h"
#include "QtWidget/WidgetManager.h"



int main(int argc, char* argv[]) {

    //QApplication a(argc, argv);

    //ThreadManager::Instance().InvokeGameThread();

    //PaintWidget paintWidget;
    //paintWidget.SetWorld(ThreadManager::Instance().GetXTankWorld());
    //paintWidget.show();

    //int res = a.exec();

    //ThreadManager::Instance().ShutDownGameThread();

    //return res;

    ThreadManager::Instance().InvokeSocketThread();

    //∑÷±Ê¬ Àı∑≈
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    WidgetManager::Instance().Init();
    WidgetManager::Instance().ShowWidget(WIDGET_NAME::MAIN_MENU);

    int r = a.exec();

    ThreadManager::Instance().ShutDownSocketThread();
    ThreadManager::Instance().ShutDownGameThread();

    return r;
}