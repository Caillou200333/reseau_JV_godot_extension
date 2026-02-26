#include "Entities/GDEntity.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

GDEntity::GDEntity() {}

GDEntity::~GDEntity() {}

void GDEntity::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init", "x", "y"), &GDEntity::Init);
}

void GDEntity::Init(uint8_t x, uint8_t y) {
    set_position(Vector2(x, y));
    
    Ref<Texture2D> texture = ResourceLoader::get_singleton()->load("res://icon.svg");
    if (texture.is_valid()) {
        set_texture(texture);
    } else {
        UtilityFunctions::printerr("Failed to load texture icon.svg");
    }
}
