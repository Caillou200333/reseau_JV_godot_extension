#include "Network/Structs/GDInput.h"

void GDInput::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_key", "key"), &GDInput::AddKey);
    ClassDB::bind_method(D_METHOD("has_key", "key"), &GDInput::HasKey);

    ClassDB::bind_method(D_METHOD("set_aim", "x", "y"), &GDInput::SetAim);
    ClassDB::bind_method(D_METHOD("get_aim"), &GDInput::GetAim);
}

void GDInput::AddKey(uint32_t key) {
    input.AddKey((enum InputID) key);
}

bool GDInput::HasKey(uint32_t key) const {
    return input.HasKey((enum InputID) key);
}

void GDInput::SetAim(float x, float y) {
    input.aim_x = x;
    input.aim_y = y;
}

Vector2 GDInput::GetAim() const {
    return Vector2(input.aim_x, input.aim_y);
}