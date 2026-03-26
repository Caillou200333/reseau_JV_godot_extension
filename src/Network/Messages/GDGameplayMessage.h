#pragma once

#include "Network/Messages/GDBaseMessage.h"

class GDGameplayMessage : public GDBaseMessage {
    GDCLASS(GDGameplayMessage, GDBaseMessage);

protected:
    static void _bind_methods();

public:
    GDGameplayMessage();

    // Getters
    uint32_t GetNetworkID() const;
    uint32_t GetClassID() const;
    uint32_t GetX() const;
    uint32_t GetY() const;
};