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
    uint8_t GetX() const;
    uint8_t GetY() const;
};