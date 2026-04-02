#pragma once

#include "gameplay/common_tools.hpp"

#include <vector>

// ID specific to a server frame
using FrameID = uint32_t;

struct EntityShot {    
    NetworkID network_id;
    ClassID class_id;

    uint32_t x;
    uint32_t y;
};

class SnapShot {
public:
    FrameID frame_id;
    std::vector<EntityShot> entities;

    SnapShot() = default;
    SnapShot(FrameID id);

    void AddEntityShot(const EntityShot& entity_shot);
};