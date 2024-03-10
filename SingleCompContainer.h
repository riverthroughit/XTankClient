#pragma once

#include <memory>

struct ISingleCompContainer {

	virtual ~ISingleCompContainer() = default;
	//获取一份拷贝 用于回滚系统
	virtual std::unique_ptr<ISingleCompContainer> GetACopy() = 0;
};


template<typename SingletonComponent>
struct SingleCompContainer :public ISingleCompContainer {

	SingletonComponent singletonComp{};

	//获取一份拷贝 用于回滚系统
	virtual std::unique_ptr<ISingleCompContainer> GetACopy() override {
		return std::make_unique<SingleCompContainer>(*this);
	}
};