#pragma once

#include "gameplay/common_tools.hpp"

#include <vector>
#include <unordered_map>

// ID specific to a server frame
using FrameID = uint32_t;

struct EntityShot {    
    NetworkID network_id;
    ClassID class_id;

    uint32_t x;
    uint32_t y;

    const struct EntityShot Lerp(double alpha, const struct EntityShot& rhs) const;
};

class SnapShot {
public:
    FrameID frame_id = 0;
    std::vector<EntityShot> entities;

    SnapShot() = default;
    SnapShot(FrameID id);

    void AddEntityShot(const struct EntityShot& entity_shot);

    // Apply a lerp between the first and the second snapshot
    const struct SnapShot Lerp(double alpha, const struct SnapShot& rhs) const;
};

class SnapShotManager {
public:
    double current_frame_time_spent = 0.;
    double frame_length = (1. / 60.);
    FrameID max_frame = 5;

    bool IsBufferReady() const;

    FrameID GetCurrentFrameID() const;
    const struct SnapShot GetCurrentSnapShot(double delta);

    void SaveSnapShot(const struct SnapShot& s);

private:
    inline static constexpr FrameID INVALID_FRAME_ID = -1;
    
    bool is_buffer_ready = false;
    FrameID current_frame_id = INVALID_FRAME_ID;
    std::unordered_map<FrameID, SnapShot> stored_snapshots;

    bool HasSnapShot(FrameID id) const;
    void DeleteSnapShot(FrameID id);
    void SkipSnapShot(FrameID nb_frame_skipt);

    // Return the last/next valid FrameID starting from given "id"
    // Return current - 1 / current + max_frame if doesn't exist
    FrameID GetLastID(FrameID id, bool included_self = true);
    FrameID GetNextID(FrameID id, bool included_self = true);
};