#pragma once

#include<unordered_map>
#include<string>
#include<cassert>

template <class AbstractType>
class IProductRegistrar
{
public:
    // 获取产品对象抽象接口
    virtual AbstractType* CreateProduct() = 0;

protected:
    // 禁止外部构造和虚构 禁止栈上分配
    IProductRegistrar() {}
    virtual ~IProductRegistrar() {}

    // 禁止拷贝和赋值操作
    IProductRegistrar(const IProductRegistrar&) = delete;
    const IProductRegistrar& operator=(const IProductRegistrar&) = delete;
};


template <class AbstractType>
class Factory
{

private:

    // key:product name , value:product type
    std::unordered_map<std::string, IProductRegistrar<AbstractType>*> nameToRegistrarMap;

    // 禁止外部构造
    Factory() {}
    //~Factory() {}

public:
    // 获取工厂单例
    static Factory& Instance()
    {
        static Factory instance;
        return instance;
    }

    // 禁止拷贝和赋值操作
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    // 产品注册
    void RegisterProduct(IProductRegistrar<AbstractType>* registrar, std::string name)
    {
        nameToRegistrarMap[name] = registrar;
    }

    // 根据名字name，获取对应具体的产品对象
    AbstractType* GetObject(std::string name)
    {
        AbstractType* object = nullptr;
        auto ite = nameToRegistrarMap.find(name);
        if (ite != nameToRegistrarMap.end()){
            object = ite->second->CreateProduct();
        }

        assert(object && "No object found for " + name);

        return object;
    }

};


template <class AbstractType, class ImplType>
class ProductRegistrar : public IProductRegistrar<AbstractType>
{
public:
    // 构造函数，用于注册产品到工厂，只能显示调用
    explicit ProductRegistrar(std::string name)
    {
        Factory<AbstractType>::Instance().RegisterProduct(this, name);
    }

    // 创建具体产品对象指针
    AbstractType* CreateProduct()
    {
        return new ImplType();
    }
};

