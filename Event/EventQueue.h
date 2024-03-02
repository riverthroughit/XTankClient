#pragma once

#include<tuple>
#include <vector>
#include "Util/Macro.h"

template<typename... ArgsType>
struct EventQueue {

	SINGLETON(EventQueue);

	using EventTuple = std::tuple<ArgsType...>;
	using FuncType = void(*)(ArgsType...);

	std::vector<EventTuple> eventTuples;

public:
	void PushEventArgs(ArgsType... args) {
		eventTuples.emplace_back(args...);
	}

	void InvokeEvents(FuncType func) {
		for (auto& eventTuple : eventTuples) {
			std::apply([&](auto... args) {func(args...)}, eventTuple);
		}
	}

};