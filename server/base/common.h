#ifndef COMMON_H
#define COMMON_H
#include <base/basic.h>
#include <Buffer.h>

const uint32 PACKET_HEAD_LEN = 20;
struct PACKET {
    PACKET(){}
    uint32 len; //整个包体的长度
    uint32 cmd;
    uint64 uid;
    uint32 sid;
    std::string msg;
};

inline std::string Encode(struct PACKET& pkt) {
    char* msg_ = new char[pkt.len];
    memset(msg_, 0, pkt.len);

    memcpy(msg_, (char*)&(pkt.len), sizeof(uint32));
    memcpy(msg_ + 4, (char*)&pkt.cmd, sizeof(uint32));
    memcpy(msg_ + 8, (char*)&pkt.uid, sizeof(uint64));
    memcpy(msg_ + 16, (char*)&pkt.sid, sizeof(uint32));
    memcpy(msg_ + 20, pkt.msg.c_str(), pkt.msg.size());
    std::string smsg(msg_, pkt.len);
    delete msg_;
    return smsg;
}

inline struct PACKET Decode(std::string& msg) {
    struct PACKET pkt;
    khaki::Buffer buf(msg);
    pkt.len = buf.readInt32();
    pkt.cmd = buf.readInt32();
    pkt.uid = buf.readInt64();
    pkt.sid = buf.readInt32();
    pkt.msg = buf.readStringByLen(pkt.len - PACKET_HEAD_LEN);
    return pkt;
}

#endif