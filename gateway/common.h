#ifndef COMMON_H
#define COMMON_H
#include <basic.h>

const uint32 PACKET_HEAD_LEN = 12;
struct PACKET {
    PACKET(const std::string name):connName(name){}
    uint32 len;
    uint32 cmd;
    uint32 uid;
    std::string msg;
    
    std::string connName;
};

#endif