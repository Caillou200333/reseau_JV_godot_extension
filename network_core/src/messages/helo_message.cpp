#include "messages/helo_message.hpp"
#include "serializer.hpp"

HELOMessage::HELOMessage():BaseMessage(HELO) {}


void HELOMessage::SerializePayload() const {}

void HELOMessage::DeserializePayload() {}