#pragma pack(push, 1)
#include <enet/enet.h>

enum class PacketType : uint8_t {
    LOGIN_REQUEST = 1,
    PLAYER_MOVE = 2
};

// Common header to identify packet types on receive
struct PacketHeader {
    PacketType type;
};

struct PacketLogin {
    PacketHeader header{PacketType::LOGIN_REQUEST};
    char username[32];
    char password[32];
};

// Example Movement Packet
struct PacketMove {
    PacketHeader header{PacketType::PLAYER_MOVE};
    uint32_t playerID;
    float x;
    float y;
    float z;
};

#pragma pack(pop)