#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector2.hpp>

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
    float GetX() const;
    float GetY() const;

    // Return the position of the entity corresponding to the network_id from the last received snapshot (current known server frame)
    Vector2 GetLastFramePosition(uint32_t network_id) const;

    SnapShotManager manager;

private:
    unsigned int current_entityshot_id = 0;
    SnapShot current_snapshot;
};