// Copyright 2017 Sardak Software

#ifndef REALMSERVEREMULATOR_PROTOCOL_ENCRYPTION_ENCRYPTION_ALGORITHM_H_
#define REALMSERVEREMULATOR_PROTOCOL_ENCRYPTION_ENCRYPTION_ALGORITHM_H_

#include "network/buffer.h"
#include "protocol/encryption/encryption_state.h"

namespace protocol
{

    void write_decryption_algorithm(const EncryptionState* state, network::Buffer* buffer);
    void write_encryption_algorithm(const EncryptionState* state, network::Buffer* buffer);

}  // namespace protocol

#endif  // REALMSERVEREMULATOR_PROTOCOL_ENCRYPTION_ENCRYPTION_ALGORITHM_H_
