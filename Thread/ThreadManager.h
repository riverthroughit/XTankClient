#pragma once


#include "Util/Macro.h"
#include <memory>
#include <thread>

class XTankWorld;

class ThreadManager {

	SINGLETON(ThreadManager);

private:

	XTankWorld* xTankWorld;
	std::unique_ptr<std::thread> gameThread;
	std::unique_ptr<std::thread> socketThread;

	bool isGameThreadEnd{};
	bool isSocketThreadEnd{};

public:

	void InvokeGameThread();

	void InvokeSocketThread();

	void ShutDownGameThread();

	void ShutDownSocketThread();

	bool IsGameThreadEnd() { return isGameThreadEnd; }

	bool IsSocketThreadEnd() { return isSocketThreadEnd; }

};