#pragma once

#include "Network/Messages/GDBaseMessage.h"

class GDHELOMessage : public GDBaseMessage {
    GDCLASS(GDHELOMessage, GDBaseMessage);

protected:
    static void _bind_methods();

public:
    GDHELOMessage();

    double GetFrameLength() const;
    double GetEntityControlledSpeed() const;
    uint32_t GetEntityControlledID() const;
};