#include "Network/Messages/GDInputMessage.h"
#include "messages/input_message.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDInputMessage::_bind_methods() {
    ClassDB::bind_static_method("GDInputMessage", D_METHOD("add_input", "input"), &GDInputMessage::AddInput);
}

GDInputMessage::GDInputMessage() {
    // Crée un InputMessage vide
    _message = std::make_shared<InputMessage>();
}

void GDInputMessage::AddInput(const Ref<GDInput>& gd_input) {
    if (gd_input.is_valid()) 
        InputMessage::AddInput(gd_input->input);
}
