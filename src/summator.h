#pragma once 

#include <godot_cpp/classes/object.hpp>

using namespace godot;

class Summator : public Object {
    GDCLASS(Summator, Object);

    int count;

protected:
    static void _bind_methods();

public:
    void add(int val);
    void reset();
    int get_total() const;

    Summator();
};