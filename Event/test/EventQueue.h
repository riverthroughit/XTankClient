#pragma once

#include<tuple>
#include <vector>
#include <utility>
#include "Utils/Macro.h"

template<typename Event>
class EventQueue {

	SINGLETON(EventQueue);

	using EventArgs = Event::FuncArgs;
	using EventTuple = std::tuple<EventArgs...>;

	std::vector<EventTuple> eventTuples;


	void PushEventArgs(EventArgs... args) {
		eventTuples.emplace_back(args...);
	}

	void InvokeEvents() {
		Event& event = Event::Instance();
		for (auto& eventTuple : eventTuples) {
			std::apply([&](auto... args) {event.Invoke(args...)}, eventTuple);
		}
		eventTuples.clear();
	}

};