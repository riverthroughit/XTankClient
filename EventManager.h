#pragma once
#include<tuple>


template<typename Event>
class EventManager 
{
	struct EventArgs {
		using Args = Event::FuncArgs;
		std::tuple<Args...> args;
	};



};