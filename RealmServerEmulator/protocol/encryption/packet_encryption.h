// Copyright 2017 Sardak Software, All Rights Reserved

#ifndef REALMSERVEREMULATOR_PROTOCOL_ENCRYPTION_PACKET_ENCRYPTION_H_
#define REALMSERVEREMULATOR_PROTOCOL_ENCRYPTION_PACKET_ENCRYPTION_H_

#include "protocol/encryption/encryption_state.h"

namespace protocol
{

    bool decrypt_packet(EncryptionState* state, void* packet);
    void encrypt_packet(EncryptionState* state, void* packet);

}

#endif  // REALMSERVEREMULATOR_PROTOCOL_ENCRYPTION_PACKET_ENCRYPTION_H_
