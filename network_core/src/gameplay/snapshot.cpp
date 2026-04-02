#include "gameplay/snapshot.hpp"

SnapShot::SnapShot(FrameID id):frame_id(id) {}

void SnapShot::AddEntityShot(const EntityShot& entity_shot) {
    entities.push_back(entity_shot);
}