#pragma once

#include "Network/Messages/GDBaseMessage.h"

class GDTextMessage : public GDBaseMessage {
    GDCLASS(GDTextMessage, GDBaseMessage);

protected:
    static void _bind_methods();

public:
    GDTextMessage();

    void Init(const String& text);
    String GetText() const;
};
