// Copyright 2017 Sardak Software, All Rights Reserved

#ifndef REALMSERVEREMULATOR_PROTOCOL_MESSAGE_SET_H_
#define REALMSERVEREMULATOR_PROTOCOL_MESSAGE_SET_H_

#include <string>
#include <memory>
#include <unordered_map>

namespace protocol
{

    template<typename T>
    class MessageSet final
    {
    public:
        MessageSet() = default;
        MessageSet(const MessageSet<T>&) = delete;
        ~MessageSet() = default;

        MessageSet<T>& operator=(const MessageSet<T>&) = delete;

        void add_packet_type(const std::string& name, uint32_t value)
        {
            packet_types_.insert(std::make_pair(name, value));
            packet_names_.insert(std::make_pair(value, name));
        }

        void add_message(const std::string& name, std::shared_ptr<T> message)
        {
            messages_.insert(std::make_pair(name, message));
        }

        uint32_t get_packet_type(const std::string& name) const
        {
            return packet_types_.find(name)->second;
        }

        const std::string& get_packet_name(uint32_t packet_type) const
        {
            return packet_names_.find(packet_type)->second;
        }

        T* get_message(const std::string& name) const
        {
            auto iter = messages_.find(name);

            return (iter != messages_.end()) ? iter->second.get() : nullptr;
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<T>> messages_;
        std::unordered_map<std::string, uint32_t> packet_types_;
        std::unordered_map<uint32_t, std::string> packet_names_;
    };

}  // namespace protocol

#endif  // REALMSERVEREMULATOR_PROTOCOL_MESSAGE_SET_H_
