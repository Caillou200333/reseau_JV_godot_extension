#include "Network/Messages/GDHELOMessage.h"
#include "messages/helo_message.hpp"

using namespace godot;

void GDHELOMessage::_bind_methods() {}

GDHELOMessage::GDHELOMessage() {
    _message = std::make_shared<HELOMessage>();
}
