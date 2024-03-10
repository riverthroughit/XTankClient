#include "WidgetManager.h"
#include "MainMenuWidget.h"
#include "LobbyWidget.h"
#include "RoomWidget.h"
#include "PaintWidget.h"


void WidgetManager::Init()
{
	widgetsMap[WIDGET_NAME::MAIN_MENU] = std::make_unique<MainMenuWidget>();
	widgetsMap[WIDGET_NAME::LOBBY] = std::make_unique<LobbyWidget>();
	widgetsMap[WIDGET_NAME::ROOM] = std::make_unique<RoomWidget>();
	widgetsMap[WIDGET_NAME::PAINT] = std::make_unique<PaintWidget>();
}
