#include "messages/helo_message.hpp"
#include "serializer.hpp"

HELOMessage::HELOMessage():BaseMessage(MessageType::HELO) {}

HELOMessage::HELOMessage(double f, double s, NetworkID e)
    :BaseMessage(MessageType::HELO),
    frame_length(f),
    character_speed(s),
    entity_controlled(e) {}

void HELOMessage::SerializePayload() const {
    Serializer::Write(frame_length);
    Serializer::Write(character_speed);
    Serializer::Write(entity_controlled);
}

void HELOMessage::DeserializePayload() {
    Serializer::Read(frame_length);
    Serializer::Read(character_speed);
    Serializer::Read(entity_controlled);
}