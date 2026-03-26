#include "messages/rtt_message.hpp"
#include "serializer.hpp"

#include <chrono>

RTTMessage::RTTMessage():RTTMessage(0, 0, 0) {}

void RTTMessage::Init() {
    id = _next_id ++;
    t0 = RTTMessage::NowMs();
}

RTTMessage::RTTMessage(uint32_t _id, uint64_t _t0, uint64_t _t1):BaseMessage(MessageType::RTT),id(_id),t0(_t0),t1(_t1) {}

RTTMessage::RTTMessage(uint32_t _id, uint64_t _t0):RTTMessage(_id, _t0, RTTMessage::NowMs()) {}

RTTMessage::RTTMessage(const RTTMessage& msg):RTTMessage(msg.id, msg.t0, RTTMessage::NowMs()) {}

RTTMessage& RTTMessage::operator=(const RTTMessage& msg) {
    if (this == &msg) return *this;

    id = msg.id;
    t0 = msg.t0;
    t1 = RTTMessage::NowMs();

    return *this;
}

uint64_t RTTMessage::NowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()
    ).count();
}

uint64_t RTTMessage::CalculateRTT() {
    return RTTMessage::NowMs() - t0;
}

void RTTMessage::SerializePayload() const {
    Serializer::Write(id);
    Serializer::Write(t0);
    Serializer::Write(t1);
}

void RTTMessage::DeserializePayload() {
    Serializer::Read(id);
    Serializer::Read(t0);
    Serializer::Read(t1);
}