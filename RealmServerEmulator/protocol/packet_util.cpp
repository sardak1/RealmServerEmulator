// Copyright 2017 Sardak Software

#include <iostream>
#include <iomanip>
#include "protocol/packet_util.h"
#include "protocol/packet_type.h"

void protocol::log_packet(const void* packet)
{
    auto words = reinterpret_cast<const uint32_t*>(packet);
    auto payload_size = words[0];
    auto packet_type = words[1];

    auto bytes = reinterpret_cast<const uint8_t*>(packet) + 8;

    std::cout << "Packet details: Type=0x" << std::setw(2) << std::setfill('0') << std::hex << words[1] << std::endl;

    for (unsigned int offset = 0; offset < payload_size; offset += 16)
    {
        std::cout << std::setw(4) << std::setfill('0') << std::hex << offset << "  ";

        for (unsigned int index = offset; index < offset + 16; index++)
        {
            if (index < payload_size)
            {
                std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(bytes[index]) << ' ';
            }
            else
            {
                std::cout << "   ";
            }
        }

        std::cout << "    ";

        for (unsigned int index = offset; index < offset + 16; index++)
        {
            if (index < payload_size)
            {
                if ((bytes[index] >= 32) && (bytes[index] < 128))
                {
                    std::cout << static_cast<char>(bytes[index]);
                }
                else
                {
                    std::cout << '.';
                }
            }
        }

        std::cout << std::endl;
    }
}
