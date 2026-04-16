#pragma once

#include "Network/Messages/GDBaseMessage.h"

#include "Network/Structs/GDSnapShotManager.h"

class GDGameplayMessage : public GDBaseMessage {
    GDCLASS(GDGameplayMessage, GDBaseMessage);

private:
    unsigned int current_index = 0;

protected:
    static void _bind_methods();

public:
    GDGameplayMessage();
    
    void SaveSnapShot(Ref<GDSnapShotManager> manager) const;
};