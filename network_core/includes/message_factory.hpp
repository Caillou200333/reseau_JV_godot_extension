#pragma once

#include "messages/base_message.hpp"

#include <unordered_map>
#include <memory>
#include <functional>

class MessageFactory {
public:
    using MSGFactoryFunction = std::function<std::unique_ptr<BaseMessage>()>;

    static std::unique_ptr<BaseMessage> Create(MessageType type);

private:
    inline static std::unordered_map<MessageType, MSGFactoryFunction> _registry;
    
    template<typename T>
    static void Register(MessageType type) {
        _registry[type] = []() { return std::make_unique<T>(); };
    }

public:
    static void RegisterAll();
};