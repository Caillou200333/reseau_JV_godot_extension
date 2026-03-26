#include "messages/helo_message.hpp"
#include "serializer.hpp"

HELOMessage::HELOMessage():BaseMessage(MessageType::HELO) {}


void HELOMessage::SerializePayload() const {}

void HELOMessage::DeserializePayload() {}