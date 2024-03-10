#pragma once

#include "Util/Macro.h"
#include <unordered_map>
#include <QWidget>
#include <memory>
#include "WidgetConfig.h"

class WidgetManager {

	SINGLETON(WidgetManager);

private:
	std::unordered_map<WIDGET_NAME::Type, std::unique_ptr<QWidget>> widgetsMap;

public:

	void Init();

	void ShowWidget(WIDGET_NAME::Type name) {
		auto ite = widgetsMap.find(name);
		assert(ite != widgetsMap.end());
		ite->second->show();
	}

	void CloseWidget(WIDGET_NAME::Type name) {
		auto ite = widgetsMap.find(name);
		assert(ite != widgetsMap.end());
		ite->second->close();
	}

	QWidget* GetWidget(WIDGET_NAME::Type name) {
		auto ite = widgetsMap.find(name);
		assert(ite != widgetsMap.end());
		return ite->second.get();
	}

};