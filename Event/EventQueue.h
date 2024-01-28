#pragma once

#include<tuple>
#include<vector>
#include<utility>

template<typename Event>
class EventQueue {

	using EventArgs = Event::FuncArgs;
	using EventTuple = std::tuple<EventArgs...>;

	std::vector<EventTuple> eventTuples;

public:
	EventQueue() {

	}

	void PushEventArgs(EventArgs... args) {
		eventTuples.emplace_back(args...);
	}

	void InvokeEvents() {
		Event& event = Event::Instance();
		for (auto& eventTuple : eventTuples) {
			std::apply([&](auto... args) {event.Invoke(args...)}, eventTuple);
		}
	}

};