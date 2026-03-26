#include "message_factory.hpp"

#include "messages/helo_message.hpp"
#include "messages/gameplay_message.hpp"
#include "messages/input_message.hpp"
#include "messages/text_message.hpp"
#include "messages/rtt_message.hpp"

std::unique_ptr<BaseMessage> MessageFactory::Create(MessageType type) {
    auto it = _registry.find(type);

    if (it != _registry.end()) {
        return it->second();
    }

    return nullptr;
}

void MessageFactory::RegisterAll() {
    MessageFactory::Register<HELOMessage>(MessageType::HELO);
    MessageFactory::Register<GameplayMessage>(MessageType::GAMEPLAY);
    MessageFactory::Register<InputMessage>(MessageType::INPUT);
    MessageFactory::Register<TextMessage>(MessageType::TEXT);
    MessageFactory::Register<RTTMessage>(MessageType::RTT);
}