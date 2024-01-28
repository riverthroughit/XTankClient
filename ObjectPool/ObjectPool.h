#pragma once
#include<cassert>
#include<memory>
#include<numeric>
#include<vector>

template <typename T, typename Allocator = std::allocator<T>>
class ObjectPool {

private:

    //ÿ�η���������������ָ��
    std::vector<T*> pool;

    //��ǰ���õĶ���
    std::vector<T*> freeObjects;

    //��ʼ�����chunk��С
    static const size_t InitChunkSize{ 5 };

    //��ǰ������chunk��С
    size_t newChunkSize{ InitChunkSize };

    Allocator allocator;

    void addChunk() {

        auto* firstNewObject{ allocator.allocate(newChunkSize) };
        pool.push_back(firstNewObject);

        auto oldFreeObjectSize{ freeObjects.size() };
        freeObjects.resize(oldFreeObjectSize + newChunkSize);
        std::iota(std::begin(freeObjects) + oldFreeObjectSize, std::end(freeObjects), firstNewObject);

        newChunkSize *= 2;
    }


public:
    ObjectPool() = default;
    ~ObjectPool() {
        assert(freeObjects.size() == InitChunkSize * (std::pow(2, pool.size()) - 1));

        size_t chunkSize{ InitChunkSize };
        for (auto* chunk : pool) {
            allocator.deallocate(chunk, chunkSize);
            chunkSize *= 2;
        }
        //pool.clear();
    }

    template <typename... Args>
    std::shared_ptr<T> acquireObject(Args&&... args) {
        if (freeObjects.empty()) {
            addChunk();
        }

        T* object{ freeObjects.back() };
        new (object) T{ std::forward<Args>(args)... };
        freeObjects.pop_back();

        return std::shared_ptr<T>(object, [this](T* object) {
            std::destroy(object);
            freeObjects.push_back(object);
        });
    }

};