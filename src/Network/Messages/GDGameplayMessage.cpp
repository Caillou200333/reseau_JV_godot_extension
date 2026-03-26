#include "Network/Messages/GDGameplayMessage.h"
#include "messages/gameplay_message.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDGameplayMessage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_network_id"), &GDGameplayMessage::GetNetworkID);
	ClassDB::bind_method(D_METHOD("get_class_id"), &GDGameplayMessage::GetClassID);
	ClassDB::bind_method(D_METHOD("get_x"), &GDGameplayMessage::GetX);
	ClassDB::bind_method(D_METHOD("get_y"), &GDGameplayMessage::GetY);
}

GDGameplayMessage::GDGameplayMessage() {
    _message = std::make_shared<GameplayMessage>();
}

uint32_t GDGameplayMessage::GetNetworkID() const {
    auto gmplMsg = std::static_pointer_cast<GameplayMessage>(_message);
    return gmplMsg->_network_id;
}
uint32_t GDGameplayMessage::GetClassID() const {
    auto gmplMsg = std::static_pointer_cast<GameplayMessage>(_message);
    return gmplMsg->_class_id;
}
uint32_t GDGameplayMessage::GetX() const {
    auto gmplMsg = std::static_pointer_cast<GameplayMessage>(_message);
    return gmplMsg->_x;
}
uint32_t GDGameplayMessage::GetY() const {
    auto gmplMsg = std::static_pointer_cast<GameplayMessage>(_message);
    return gmplMsg->_y;
}