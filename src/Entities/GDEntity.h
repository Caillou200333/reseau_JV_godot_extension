#pragma once

#include <godot_cpp/classes/sprite2d.hpp>

using namespace godot;

class GDEntity : public Sprite2D {
	GDCLASS(GDEntity, Sprite2D)

protected:
	static void _bind_methods();

public:
	GDEntity();
	~GDEntity();

	void Init(uint8_t x, uint8_t y);
};