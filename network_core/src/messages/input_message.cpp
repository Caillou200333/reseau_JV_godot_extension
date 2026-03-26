#include "messages/input_message.hpp"
#include "serializer.hpp"

#include <stack>

// INPUT
void Input::AddKey(InputID key) { keys |= key; }

bool Input::HasKey(InputID key) const { return (keys & key); }

// INPUT BUFFER
void InputBuffer::AddInput(const struct Input& input) {
    ++ next_sequence;
    inputs.push_back(input);

    unsigned int inputs_size = inputs.size();
    if (inputs_size > MAX_INPUTS) {
        RemoveInput(inputs_size - MAX_INPUTS);

        unsigned int first_sequence = next_sequence - inputs.size();
        if (next_sequence_to_get < first_sequence)
            next_sequence_to_get = first_sequence;
    }
}

const Input* InputBuffer::GetNextInput() {
    if (next_sequence_to_get >= next_sequence)
        return nullptr;

    unsigned int input_index = (next_sequence_to_get ++) + inputs.size() - next_sequence;
    return &inputs[input_index];
}

void InputBuffer::RemoveInput(unsigned int nb_to_remove) {
    for (unsigned int i = 0; !inputs.empty() && (i < nb_to_remove); ++ i) {
        inputs.pop_front();
    }
}

//INPUT MESSAGE
InputMessage::InputMessage():BaseMessage(MessageType::INPUT) {}

struct InputBuffer* InputMessage::GetInputBuffer(PlayerID id) {
    auto it = _inputs_received.find(id);
    if (it != _inputs_received.end()) return &it->second;
    return nullptr;
}

void InputMessage::SetPlayerID(PlayerID new_player_id) {
    _current_player_id = new_player_id;
}

void InputMessage::AddInput(const struct Input& input) {
    _inputs_to_send.AddInput(input);
}

const Input* InputMessage::GetNextInput(PlayerID player_id) {    
    struct InputBuffer* input_buffer = InputMessage::GetInputBuffer(player_id);
    
    if (!input_buffer)
        throw std::runtime_error("Missing player's input for corresponding PlayerID in GetInput");

    return input_buffer->GetNextInput();
}

void InputMessage::SerializePayload() const {
    Serializer::Write(_inputs_to_send.next_sequence);

    for (unsigned int i = _inputs_to_send.inputs.size(); i > 0; -- i) {
        const struct Input& input_to_write = _inputs_to_send.inputs[i - 1];
        Serializer::Write(input_to_write.keys);
        Serializer::Write(input_to_write.aim_x);
        Serializer::Write(input_to_write.aim_y);
    }
}

void InputMessage::DeserializePayload() {
    if (_current_player_id == INVALID_ID) return;
        //throw std::runtime_error("INVALID_ID used as PlayerID to store received inputs in InputMessage::DeserializePayload (must specify a correct PlayerID before deserialization)");

    struct InputBuffer* input_buffer = InputMessage::GetInputBuffer(_current_player_id);
    if (!input_buffer) { // No input previously received from current_player_id
        _inputs_received[_current_player_id] = InputBuffer();
        input_buffer = &_inputs_received[_current_player_id];
    }

    uint32_t received_sequence;
    Serializer::Read(received_sequence);

    uint32_t current_sequence = input_buffer->next_sequence;

    if (received_sequence <= current_sequence)
        return; // a paquet has been lost/delayed -> received an obsolete paquet with inputs already handled

    static struct Input input_to_read;
    static std::stack<struct Input> tmp_stack; // to return the order in which we read the inputs
    unsigned int nb_inputs_to_read = received_sequence - current_sequence; // nb of inputs to read/recover

    for (unsigned int i = 0; i < nb_inputs_to_read; ++ i) {
        Serializer::Read(input_to_read.keys);
        Serializer::Read(input_to_read.aim_x);
        Serializer::Read(input_to_read.aim_y);
        tmp_stack.push(input_to_read);
    }

    while (!tmp_stack.empty()) {
        input_buffer->AddInput(tmp_stack.top());
        tmp_stack.pop();
    }
}