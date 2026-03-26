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

	void Init(uint32_t x, uint32_t y);

	void Move(uint32_t x, uint32_t y);
};