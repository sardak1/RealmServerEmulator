// Copyright 2017 Sardak Software

#include "network/buffer.h"

network::Buffer::Buffer(uint32_t initial_capacity)
    : data_(new uint8_t[initial_capacity]), capacity_(initial_capacity)
{
}

void network::Buffer::append(void* data, uint32_t size)
{
    grow(size_ + size);

    std::memcpy(data_.get() + size_, data, size);
    size_ += size;
}

void network::Buffer::advance(uint32_t size)
{
    size_ += size;
}

void network::Buffer::consume(uint32_t size)
{
    if (size < size_)
    {
        std::memmove(data_.get(), data_.get() + size, size_ - size);
        size_ -= size;
    }
    else
    {
        size_ = 0;
    }
}

void network::Buffer::grow(uint32_t desired_size)
{
    if (desired_size > capacity_)
    {
        while (capacity_ < desired_size)
        {
            capacity_ <<= 1;
        }

        std::unique_ptr<uint8_t[]> new_data(new uint8_t[capacity_]);
        std::memcpy(new_data.get(), data_.get(), size_);
        data_ = std::move(new_data);
    }
}
