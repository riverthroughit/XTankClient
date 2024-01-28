#pragma once
#include<utility>
#include<cassert>
#include<unordered_set>
#include<unordered_map>

template<typename... Args>
class Event
{

    using InternalFunc = void(*)(void*, Args...);

public:
    using FuncArgs = Args;
    //typedef std::pair<void*, InternalFunc> CallbackStub;

private:

    //key:listener value:callbacks of the listener
    std::unordered_map<void*, std::unordered_set<InternalFunc>> listenerToCallbacks;

    // turns a free function into our internal function stub
    template <void (*Func)(Args...)>
    static void FuncCallback(void*, Args... args)
    {
        // we don't need the listener pointer because we're dealing with free functions
        (Func)(args...);
    }

    // turns a member function into our internal function stub
    template <class C, void (C::* Func)(Args...)>
    static void ClassFuncCallback(void* listener, Args... args)
    {
        // cast the listener pointer back into the original class listener
        (static_cast<C*>(listener)->*Func)(args...);
    }

    Event() {}

public:

    static Event& Instance() {
        static Event event;
        return event;
    }

    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    /// Binds a free function
    template <void (*Func)(Args...)>
    void AddCallback()
    {
        listenerToCallbacks[nullptr].insert(&FuncCallback<Func>);
    }

    /// Binds a class method
    template <class C, void (C::* Func)(Args...)>
    void AddCallback(C* listener)
    {
        listenerToCallbacks[listener].insert(&ClassFuncCallback<C, Func>);
    }

    template <void (*Func)(Args...)>
    void RemoveCallback()
    {
        listenerToCallbacks[nullptr].erase(&FuncCallback<Func>);
    }

    template <class C, void (C::* Func)(Args...)>
    void RemoveCallback(C* listener)
    {
        listenerToCallbacks[listener].erase(&ClassFuncCallback<C, Func>);
    }

    void RemoveListener(void* listener) 
    {
        listenerToCallbacks.erase(listener);
    }

    // Invokes the delegate
    void Invoke(Args... args)
    {
        for (auto& [listener, callbacks] : listenerToCallbacks) {
            for (auto callback : callbacks) {
                callback(listener, args...);
            }
        }
    }

};