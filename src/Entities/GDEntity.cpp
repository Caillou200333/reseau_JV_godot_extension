#include "Entities/GDEntity.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

GDEntity::GDEntity() {}

GDEntity::~GDEntity() {}

void GDEntity::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init", "x", "y"), &GDEntity::Init);
    ClassDB::bind_method(D_METHOD("move", "x", "y"), &GDEntity::Move);
}

void GDEntity::Init(uint32_t x, uint32_t y) {
    set_position(Vector2(x, y));
    
    Ref<Texture2D> texture = ResourceLoader::get_singleton()->load("res://icon.svg");
    if (texture.is_valid()) {
        set_texture(texture);
    } else {
        UtilityFunctions::printerr("Failed to load texture icon.svg");
    }
}

void GDEntity::Move(uint32_t x, uint32_t y) {
    set_position(Vector2(x, y));
}