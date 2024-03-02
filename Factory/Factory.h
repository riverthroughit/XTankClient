#pragma once

#include<unordered_map>
#include<string>
#include<cassert>

template <class AbstractType>
class IProductRegistrar
{
public:
    // ��ȡ��Ʒ�������ӿ�
    virtual AbstractType* CreateProduct() = 0;

protected:
    // ��ֹ�ⲿ������鹹 ��ֹջ�Ϸ���
    IProductRegistrar() {}
    virtual ~IProductRegistrar() {}

    // ��ֹ�����͸�ֵ����
    IProductRegistrar(const IProductRegistrar&) = delete;
    const IProductRegistrar& operator=(const IProductRegistrar&) = delete;
};


template <class AbstractType>
class Factory
{

private:

    // key:product name , value:product type
    std::unordered_map<std::string, IProductRegistrar<AbstractType>*> nameToRegistrarMap;

    // ��ֹ�ⲿ����
    Factory() {}
    //~Factory() {}

public:
    // ��ȡ��������
    static Factory& Instance()
    {
        static Factory instance;
        return instance;
    }

    // ��ֹ�����͸�ֵ����
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    // ��Ʒע��
    void RegisterProduct(IProductRegistrar<AbstractType>* registrar, std::string name)
    {
        nameToRegistrarMap[name] = registrar;
    }

    // ��������name����ȡ��Ӧ����Ĳ�Ʒ����
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
    // ���캯��������ע���Ʒ��������ֻ����ʾ����
    explicit ProductRegistrar(std::string name)
    {
        Factory<AbstractType>::Instance().RegisterProduct(this, name);
    }

    // ���������Ʒ����ָ��
    AbstractType* CreateProduct()
    {
        return new ImplType();
    }
};

