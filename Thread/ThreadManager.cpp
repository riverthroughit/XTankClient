#include "ThreadManager.h"
#include "ECS/XTankWorld.h"
#include "Socket/SocketClient.h"


void ThreadManager::InvokeGameThread()
{
	if (gameThread == nullptr) {
		xTankWorld = new XTankWorld();
		gameThread = std::make_unique<std::thread>([&]() {
			xTankWorld->Init();
			xTankWorld->Start();		
		});
		isGameThreadEnd = false;
	}
}

void ThreadManager::InvokeSocketThread()
{
	if (socketThread == nullptr) {
		socketThread = std::make_unique<std::thread>([&]() {
			SocketClient::Instance().ConnectToServer();
			SocketClient::Instance().SocketClientThread();
		});
		isSocketThreadEnd = false;
	}
}

void ThreadManager::ShutDownGameThread()
{
	if (gameThread) {
		isGameThreadEnd = true;
		gameThread->join();
		gameThread.reset();
		delete xTankWorld;
		xTankWorld = nullptr;
	}
}

void ThreadManager::ShutDownSocketThread()
{
	if (socketThread) {
		isSocketThreadEnd = true;
		socketThread->join();
		socketThread.reset();
	}
}

