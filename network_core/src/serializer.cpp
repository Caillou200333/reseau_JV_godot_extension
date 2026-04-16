#include "serializer.hpp"

void Serializer::SetWritingBuffer(uint8_t* buffer, unsigned int capacity) {
    Serializer::_writing_buffer = buffer;
    Serializer::_writing_offset = 0;
    Serializer::_writing_capacity = capacity;
}

void Serializer::SetReadingBuffer(const uint8_t* buffer, unsigned int size) {
    Serializer::_reading_buffer = buffer;
    Serializer::_reading_offset = 0;
    Serializer::_reading_size = size;
}

unsigned int Serializer::GetWritingSize() { return Serializer::_writing_offset;}