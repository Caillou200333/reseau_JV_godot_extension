#include "Network/Structs/GDSnapShotManager.h"

#include <godot_cpp/core/class_db.hpp>

void GDSnapShotManager::_bind_methods() {
    // Core
    ClassDB::bind_method(D_METHOD("get_current_frame_id"), &GDSnapShotManager::GetCurrentFrameID);
    ClassDB::bind_method(D_METHOD("load_current_snapshot", "delta"), &GDSnapShotManager::LoadCurrentSnapshot);

    // Config
    ClassDB::bind_method(D_METHOD("is_buffer_ready"), &GDSnapShotManager::IsBufferReady);    

    ClassDB::bind_method(D_METHOD("set_frame_length", "length"), &GDSnapShotManager::SetFrameLength);
    ClassDB::bind_method(D_METHOD("get_frame_length"), &GDSnapShotManager::GetFrameLength);

    ClassDB::bind_method(D_METHOD("set_max_frame", "max"), &GDSnapShotManager::SetMaxFrame);
    ClassDB::bind_method(D_METHOD("get_max_frame"), &GDSnapShotManager::GetMaxFrame);

    // Iteration
    ClassDB::bind_method(D_METHOD("is_empty"), &GDSnapShotManager::IsEmpty);
    ClassDB::bind_method(D_METHOD("next"), &GDSnapShotManager::Next);

    // Entity getters
    ClassDB::bind_method(D_METHOD("get_network_id"), &GDSnapShotManager::GetNetworkID);
    ClassDB::bind_method(D_METHOD("get_class_id"), &GDSnapShotManager::GetClassID);
    ClassDB::bind_method(D_METHOD("get_x"), &GDSnapShotManager::GetX);
    ClassDB::bind_method(D_METHOD("get_y"), &GDSnapShotManager::GetY);

    // Optional: expose as properties in Godot editor
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "frame_length"), "set_frame_length", "get_frame_length");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_frame"), "set_max_frame", "get_max_frame");
}

FrameID GDSnapShotManager::GetCurrentFrameID() const { return manager.GetCurrentFrameID(); }

void GDSnapShotManager::LoadCurrentSnapshot(double delta) {
    current_entityshot_id = 0;
    current_snapshot = manager.GetCurrentSnapShot(delta);
}


bool GDSnapShotManager::IsBufferReady() const { return manager.IsBufferReady(); }

void GDSnapShotManager::SetFrameLength(double length) { manager.frame_length = length; }
double GDSnapShotManager::GetFrameLength() const { return manager.frame_length; }

void GDSnapShotManager::SetMaxFrame(int max) { manager.max_frame = max; }
int GDSnapShotManager::GetMaxFrame() const { return manager.max_frame; }


// Iterate onto the current snapshot
bool GDSnapShotManager::IsEmpty() const {
    return current_entityshot_id >= current_snapshot.entities.size();
}

bool GDSnapShotManager::Next() {
    return ((++ current_entityshot_id) < current_snapshot.entities.size());
}

uint32_t GDSnapShotManager::GetNetworkID() const {
    return current_snapshot.entities[current_entityshot_id].network_id;
}

uint32_t GDSnapShotManager::GetClassID() const {
    return current_snapshot.entities[current_entityshot_id].class_id;
}

uint32_t GDSnapShotManager::GetX() const {
    return current_snapshot.entities[current_entityshot_id].x;
}

uint32_t GDSnapShotManager::GetY() const {
    return current_snapshot.entities[current_entityshot_id].y;
}