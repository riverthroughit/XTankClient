#pragma once

#include <memory>

struct ISingleCompContainer {

	virtual ~ISingleCompContainer() = default;
	//��ȡһ�ݿ��� ���ڻع�ϵͳ
	virtual std::unique_ptr<ISingleCompContainer> GetACopy() = 0;
};


template<typename SingletonComponent>
struct SingleCompContainer :public ISingleCompContainer {

	SingletonComponent singletonComp{};

	//��ȡһ�ݿ��� ���ڻع�ϵͳ
	virtual std::unique_ptr<ISingleCompContainer> GetACopy() override {
		return std::make_unique<SingleCompContainer>(*this);
	}
};