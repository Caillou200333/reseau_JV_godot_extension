#include "Network/Messages/GDHELOMessage.h"
#include "messages/helo_message.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDHELOMessage::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_frame_length"), &GDHELOMessage::GetFrameLength);
    ClassDB::bind_method(D_METHOD("get_entity_controlled_speed"), &GDHELOMessage::GetEntityControlledSpeed);
    ClassDB::bind_method(D_METHOD("get_entity_controlled_id"), &GDHELOMessage::GetEntityControlledID);
}

GDHELOMessage::GDHELOMessage() {
    _message = std::make_shared<HELOMessage>();
}

double GDHELOMessage::GetFrameLength() const {
    auto heloMsg = std::static_pointer_cast<HELOMessage>(_message);
    return heloMsg->frame_length;
}

double GDHELOMessage::GetEntityControlledSpeed() const {
    auto heloMsg = std::static_pointer_cast<HELOMessage>(_message);
    return heloMsg->character_speed;
}

uint32_t GDHELOMessage::GetEntityControlledID() const {
    auto heloMsg = std::static_pointer_cast<HELOMessage>(_message);
    return heloMsg->entity_controlled;
}