#include "Network/Messages/GDBaseMessage.h"

void GDBaseMessage::_bind_methods() {}

std::shared_ptr<BaseMessage> GDBaseMessage::GetMessage() { return _message; }