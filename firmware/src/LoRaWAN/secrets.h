#ifndef SECRETS
#define SECRETS

// The following constants should be copied to secrets.h and configured appropriately
// according to the settings from TTN Console

// deveui, little-endian (lsb first)
static const std::uint8_t deveui[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// appeui, little-endian (lsb first)
static const std::uint8_t appeui[] = {0xDA, 0x82, 0xE6, 0x3E, 0x79, 0x74, 0x46, 0xFC};

// appkey: just a string of bytes, sometimes referred to as "big endian".
static const std::uint8_t appkey[] = {
    0x93, 0x14, 0xD0, 0xA3, 0xFC, 0xDE, 0x71,
    0x5B, 0xAF, 0x80, 0x73, 0x45, 0x2A, 0x09,
    0x1B, 0x52};
#endif