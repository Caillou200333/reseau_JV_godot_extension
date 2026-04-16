#include "gameplay/snapshot.hpp"

#include <stdexcept>

// EntityShot
const struct EntityShot EntityShot::Lerp(double alpha, const struct EntityShot& rhs) const {
    return {network_id, class_id, (uint32_t)((1-alpha)*x + alpha*rhs.x), (uint32_t)((1-alpha)*y + alpha*rhs.y)};
}


// SnapShot
SnapShot::SnapShot(FrameID id):frame_id(id) {}

void SnapShot::AddEntityShot(const EntityShot& entity_shot) {
    entities.push_back(entity_shot);
}

const struct SnapShot SnapShot::Lerp(double alpha, const struct SnapShot& rhs) const {
    struct SnapShot interp;

    for (auto& entity_lhs : entities) {
        struct EntityShot entity_rhs;
    
        unsigned int i;
        for (i = 0; i < rhs.entities.size(); ++ i) {
            if (rhs.entities[i].network_id == entity_lhs.network_id) {
                entity_rhs = rhs.entities[i];
                break;
            }
        }
        if (i >= rhs.entities.size())
            continue;

        interp.AddEntityShot(entity_lhs.Lerp(alpha, entity_rhs));
    }

    return interp;
}


// SnapShotManager
bool SnapShotManager::HasSnapShot(FrameID id) const {
    return stored_snapshots.find(id) != stored_snapshots.end();
}

void SnapShotManager::DeleteSnapShot(FrameID id) { stored_snapshots.erase(id); }

FrameID SnapShotManager::GetLastID(FrameID id, bool included_self) {
    FrameID last_id = (included_self) ? id : (id - 1);
    
    while ((last_id >= current_frame_id) && !HasSnapShot(last_id)) { -- last_id; }

    if (last_id < current_frame_id) throw std::runtime_error("SnapShotManager error : no last frame available");

    return last_id;
}

FrameID SnapShotManager::GetNextID(FrameID id, bool included_self) {
    FrameID next_id = (included_self) ? id : (id + 1);
    
    while ((next_id < (current_frame_id + max_frame)) && !HasSnapShot(next_id)) { ++ next_id; }
    
    if (next_id >= (current_frame_id + max_frame)) throw std::runtime_error("SnapShotManager error : no next frame available");
    
    return next_id;
}
#include <iostream>
void SnapShotManager::SkipSnapShot(FrameID nb_frame_skipt) {
    FrameID tmp_current_frame_id = current_frame_id;
    FrameID new_current_frame_id = current_frame_id + nb_frame_skipt;

    FrameID last_available_frame_id = GetLastID(new_current_frame_id);
    current_frame_id = new_current_frame_id;
    FrameID next_available_frame_id = GetNextID(new_current_frame_id);
    FrameID delta_frame_skipt = next_available_frame_id - last_available_frame_id;

    if (delta_frame_skipt > 0) {
        double alpha_skipt = (double)(new_current_frame_id - last_available_frame_id) / (double) delta_frame_skipt;
        std::cout << alpha_skipt;
        SnapShot tmp_snapshot = stored_snapshots[last_available_frame_id].Lerp(alpha_skipt, stored_snapshots[next_available_frame_id]);
        tmp_snapshot.frame_id = new_current_frame_id;
        SaveSnapShot(tmp_snapshot);
    }
    

    for (FrameID id = tmp_current_frame_id; id < new_current_frame_id; ++ id) {
        DeleteSnapShot(id);
    }
}


bool SnapShotManager::IsBufferReady() const { return is_buffer_ready; }

FrameID SnapShotManager::GetCurrentFrameID() const { return current_frame_id; }

const struct SnapShot SnapShotManager::GetCurrentSnapShot(double delta) {
    current_frame_time_spent += delta;
    unsigned int nb_frame_skipt = (unsigned int) (current_frame_time_spent / frame_length);
    if (nb_frame_skipt > 0) {
        SkipSnapShot(nb_frame_skipt);
        /*
        FrameID new_current_frame_id = current_frame_id + nb_frame_skipt;
        
        FrameID last_available_frame_id = GetLastID(new_current_frame_id);
        FrameID next_available_frame_id = GetNextID(new_current_frame_id);
        FrameID delta_frame_skipt = next_available_frame_id - last_available_frame_id;

        if (delta_frame_skipt > 0) {
            double alpha_skipt = (double)(new_current_frame_id - current_frame_id) / (double) delta_frame_skipt;
            SaveSnapShot(stored_snapshots[last_available_frame_id].Lerp(alpha_skipt, stored_snapshots[next_available_frame_id]));
        }

        for (FrameID id = current_frame_id; id < new_current_frame_id; ++ id) {
            DeleteSnapShot(id);
        }
        current_frame_id = new_current_frame_id;*/
        current_frame_time_spent -= nb_frame_skipt * frame_length;
    }

    const struct SnapShot& current_snapshot = stored_snapshots[current_frame_id];
    
    FrameID next_frame_id = GetNextID(current_frame_id, false);
    const struct SnapShot& next_snapshot = stored_snapshots[next_frame_id];

    FrameID delta_frame = next_frame_id - current_frame_id;
    double alpha = delta / (delta_frame * frame_length);

    return current_snapshot.Lerp(alpha, next_snapshot);
}

void SnapShotManager::SaveSnapShot(const struct SnapShot& s) {
    if (current_frame_id == INVALID_FRAME_ID)
        current_frame_id = s.frame_id;

    if ((s.frame_id >= current_frame_id) && !HasSnapShot(s.frame_id)) {
        stored_snapshots[s.frame_id] = s;

        if (s.frame_id >= (current_frame_id + max_frame)) {
            is_buffer_ready = true;
            FrameID nb_frame_skipt = s.frame_id - (current_frame_id + max_frame);
            SkipSnapShot(nb_frame_skipt);
        }
    }
}