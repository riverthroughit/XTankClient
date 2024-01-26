#pragma once
#include<utility>
#include<cassert>
#include<vector>

template<typename... Args>
class Event
{
    typedef void (*InternalFunc)(void*, Args...);
    typedef std::pair<void*, InternalFunc> CallbackStub;

private:
    //CallbackStub callbackStub;
    std::vector<CallbackStub> callbackStubs;

    // turns a free function into our internal function stub
    template <void (*Func)(Args...)>
    static void FuncCallback(void*, Args... args)
    {
        // we don't need the instance pointer because we're dealing with free functions
        (Func)(args...);
    }

    // turns a member function into our internal function stub
    template <class C, void (C::* Func)(Args...)>
    static void ClassFuncCallback(void* instance, Args... args)
    {
        // cast the instance pointer back into the original class instance
        (static_cast<C*>(instance)->*Func)(args...);
    }

public:

    /// Binds a free function
    template <void (*Func)(Args...)>
    void AddListener()
    {
        callbackStubs.emplace_back(nullptr, &FuncCallback<Func>);
    }

    /// Binds a class method
    template <class C, void (C::* Func)(Args...)>
    void AddListener(C* instance)
    {
        callbackStubs.emplace_back(instance, &ClassFuncCallback<C, Func>);
    }

    /// Invokes the delegate
    void Invoke(Args... args) const
    {
        assert(callbackStub.second && "Cannot invoke unbound delegate. Call Bind() first.");
        return callbackStub.second(callbackStub.first, args...);
    }

};