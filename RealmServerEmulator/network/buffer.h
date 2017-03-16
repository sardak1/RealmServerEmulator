// Copyright 2017 Sardak Software, All Rights Reserved

#ifndef REALMSERVEREMULATOR_NETWORK_BUFFER_H_
#define REALMSERVEREMULATOR_NETWORK_BUFFER_H_

#include <type_traits>
#include <cstdint>
#include <memory>

namespace network
{

    class Buffer final
    {
    public:
        Buffer() = delete;
        Buffer(uint32_t initial_capacity);
        Buffer(const Buffer&) = delete;

        Buffer& operator=(const Buffer&) = delete;

        void append(void* data, uint32_t size);
        void advance(uint32_t size);
        void consume(uint32_t size);

        void grow(uint32_t desired_size);

        template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
        void write(const T& value)
        {
            write_bytes(&value, sizeof(T));
        }

        template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
        T read()
        {
            T value;
            read_bytes(&value, sizeof(T));
            return value;
        }

        uint8_t* data() { return data_.get(); }
        uint32_t capacity() const { return capacity_; }
        uint32_t size() const { return size_; }

    private:
        void write_bytes(const void* data, size_t size);
        void read_bytes(void* data, size_t size);

        std::unique_ptr<uint8_t[]> data_;
        uint32_t capacity_;
        uint32_t size_ = 0;
    };

}

#endif  // REALMSERVEREMULATOR_NETWORK_BUFFER_H_
