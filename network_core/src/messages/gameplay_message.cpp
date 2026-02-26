#include "messages/gameplay_message.hpp"
#include "serializer.hpp"

GameplayMessage::GameplayMessage():GameplayMessage(0,ClassID::PLAYER_CLASS,0,0) {}

GameplayMessage::GameplayMessage(NetworkID network_id, ClassID class_id, uint8_t x, uint8_t y)
    :BaseMessage(GAMEPLAY),
    _network_id(network_id),_class_id(class_id),_x(x),_y(y) {}


void GameplayMessage::SerializePayload() const {
    Serializer::Write(_network_id);
    Serializer::Write(_class_id);
    Serializer::Write(_x);
    Serializer::Write(_y);
}

void GameplayMessage::DeserializePayload() {
    Serializer::Read(_network_id);
    Serializer::Read(_class_id);
    Serializer::Read(_x);
    Serializer::Read(_y);
}