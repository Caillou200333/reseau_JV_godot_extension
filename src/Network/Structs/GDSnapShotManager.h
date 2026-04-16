#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

#include "gameplay/snapshot.hpp"

using namespace godot;

class GDSnapShotManager : public RefCounted {
    GDCLASS(GDSnapShotManager, RefCounted);

protected:
    static void _bind_methods();

public:
    GDSnapShotManager() = default;

    // Update + get interpolated snapshot
    FrameID GetCurrentFrameID() const;
    void LoadCurrentSnapshot(double delta);

    // Config
    bool IsBufferReady() const;

    void SetFrameLength(double length);
    double GetFrameLength() const;

    void SetMaxFrame(int max);
    int GetMaxFrame() const;

    // Iterate onto the current snapshot
    bool IsEmpty() const;
    bool Next();

    uint32_t GetNetworkID() const;
    uint32_t GetClassID() const;
    uint32_t GetX() const;
    uint32_t GetY() const;

    SnapShotManager manager;

private:
    unsigned int current_entityshot_id = 0;
    SnapShot current_snapshot;
};