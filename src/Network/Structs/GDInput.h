#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

#include "messages/input_message.hpp"

class GDInput : public RefCounted {
    GDCLASS(GDInput, RefCounted);

protected:
    static void _bind_methods();

public:
    GDInput() = default;

    void AddKey(uint32_t key);
    bool HasKey(uint32_t key) const;

    void SetAim(float x, float y);
    Vector2 GetAim() const;

    struct Input input = {};
};
