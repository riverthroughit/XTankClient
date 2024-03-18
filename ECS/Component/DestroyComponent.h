#pragma once

//用于标记entity是否需销毁
struct DestroyComponent {

	int latency{};	//延迟一定帧数再销毁
};