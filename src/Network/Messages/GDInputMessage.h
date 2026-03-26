#pragma once

#include "Network/Messages/GDBaseMessage.h"
#include "Network/Structs/GDInput.h"

class GDInputMessage : public GDBaseMessage {
    GDCLASS(GDInputMessage, GDBaseMessage);

protected:
    static void _bind_methods();

public:
    GDInputMessage();

    static void AddInput(const Ref<GDInput>& input);
};
