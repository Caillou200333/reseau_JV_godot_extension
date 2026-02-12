#include "summator.h"

#include <godot_cpp/core/class_db.hpp>

void Summator::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add", "val"), &Summator::add);
    ClassDB::bind_method(D_METHOD("reset"), &Summator::reset);
    ClassDB::bind_method(D_METHOD("get"), &Summator::get_total);
}

void Summator::add(int val) { count += val; }
void Summator::reset() { count = 0; }
int Summator::get_total() const { return count; }

Summator::Summator():count(0) {}