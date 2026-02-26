#pragma once

#include <cstdint>
#include <cstring>
#include <stdexcept>
/**
 * @class Static class allowing to write / read a given buffer with serialization (used to encode/decode the serialization)
 */
class Serializer {
public:
    /**
     * @brief Set a new buffer to read/write into with a given capacity/size.
     * Reset the offset.
     * Must be called before each new read/write use.
     */
    static void SetWritingBuffer(uint8_t* buffer, unsigned int capacity);
    static void SetReadingBuffer(const uint8_t* buffer, unsigned int size);

    /**
     * @brief Various writing methods to write inside the given buffer (serialize the data).
     */
    template<typename T>
    static void Write(const T& value) {
        unsigned int sizeof_value = sizeof(T);
        if (_writing_offset + sizeof_value > _writing_capacity)
            throw std::runtime_error("Buffer overflow in Write");

        std::memcpy(_writing_buffer + _writing_offset, &value, sizeof_value);
        _writing_offset += sizeof_value;
    }

    static void WriteBytes(const char* data, unsigned int size_bytes) {
        if (_writing_offset + size_bytes > _writing_capacity)
            throw std::runtime_error("Buffer overflow in WriteBytes");
        
        std::memcpy(_writing_buffer + _writing_offset, data, size_bytes);
        _writing_offset += size_bytes;
    }
    
    /**
     * @brief Return the size of the writen buffer.
     */
    static unsigned int GetWritingSize();

    /**
     * @brief Various reading methods to read from the given buffer (deserialize the data).
     * @return override the parameter 'out_value'.
     */
    template<typename T>
    static void Read(T& out_value) {
        unsigned int sizeof_value = sizeof(T);
        if (_reading_offset + sizeof_value > _reading_size)
            throw std::runtime_error("Buffer overflow in Read");

        std::memcpy(&out_value, _reading_buffer + _reading_offset, sizeof_value);
        _reading_offset += sizeof_value;
    }

    static void ReadBytes(char* out_data, unsigned int size_bytes) {
        if (_reading_offset + size_bytes > _reading_size)
            throw std::runtime_error("Buffer overflow in ReadBytes");

        std::memcpy(out_data, _reading_buffer + _reading_offset, size_bytes);
        _reading_offset += size_bytes;
    }

private:
    inline static uint8_t* _writing_buffer;
    inline static unsigned int _writing_offset;
    inline static unsigned int _writing_capacity;

    inline static const uint8_t* _reading_buffer;
    inline static unsigned int _reading_offset;
    inline static unsigned int _reading_size;
};