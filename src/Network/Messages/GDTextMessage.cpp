#include "Network/Messages/GDTextMessage.h"
#include "messages/text_message.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDTextMessage::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init", "text"), &GDTextMessage::Init);
    ClassDB::bind_method(D_METHOD("get_text"), &GDTextMessage::GetText);
}

GDTextMessage::GDTextMessage() {
    // Crée un TextMessage vide
    _message = std::make_shared<TextMessage>();
}

void GDTextMessage::Init(const String& text) {
    _message = std::make_shared<TextMessage>(text.utf8().get_data(), text.utf8().length());
}

String GDTextMessage::GetText() const {
    auto txtMsg = std::static_pointer_cast<TextMessage>(_message);
    return String(txtMsg->GetText());
}
