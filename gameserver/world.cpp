#include <unistd.h>
#include <world.h>
#include <Log.h>
#include <protocol/out/cs.pb.h>
#include <protocol/in/base.pb.h>
#include <error.h>

World::World():thread_(&World::Run, this) {
    running_ = false;
    RegisterCmd();
}

World::~World() {
    thread_.join();
}

void World::Run() {
    while ( running_ ) {
        MsgProcess(msgQueue_);
        MsgProcess(dbMsgQueue_);
        usleep(10000);
    }
}

void World::MsgProcess(khaki::queue<struct PACKET>& msg) {
    if ( msg.size() > 0 ) {
        std::queue<struct PACKET> tmpQueue = msg.popAll();
        while ( !tmpQueue.empty() ) {
            struct PACKET pkt = tmpQueue.front();
            DispatcherCmd(pkt);
            tmpQueue.pop();
        }
    }
}

void World::RegisterCmd() {
    ///////////GS///////////
    REGISTER_CMD_CALLBACK(gs::ProtoID::ID_G2S_Login, HandlerLogin);
    REGISTER_CMD_CALLBACK(gs::ProtoID::ID_G2S_Create, HandlerCreate);
    ///////////RS///////////
    REGISTER_CMD_CALLBACK(sr::ProtoID::ID_R2S_Login, HandlerRSLogin);
    REGISTER_CMD_CALLBACK(sr::ProtoID::ID_R2S_Create, HandlerRSCreate);
}

void World::DispatcherCmd(struct PACKET& msg) {
    if ( command_.find(msg.cmd) != command_.end() ) {
        command_[msg.cmd](msg);
    } else {
        log4cppDebug(khaki::logger, "error proto : %d", msg.cmd);
    }
}

///////////GS///////////
bool World::HandlerLogin(struct PACKET& pkt) {
    gs::G2S_Login recv;
    if ( !recv.ParseFromString(pkt.msg) )
    {
        log4cppDebug(khaki::logger, "proto parse error : %d", pkt.cmd);
        return false;
    }

    uint64 uid = recv.uid();

    sr::S2R_Login msg;
    uint32 msgId = uint32(sr::ProtoID::ID_S2R_Login);
    msg.set_tokenid(recv.tokenid());
    msg.set_uid(uid);
    std::string msgStr = msg.SerializeAsString();
    dSession_->SendPacket(msgId, pkt.uid, pkt.sid, msgStr);
    log4cppDebug(khaki::logger, "HandlerLogin proto : %d %d", pkt.cmd, uid);
}

bool World::HandlerCreate(struct PACKET& pkt) {
    gs::G2S_Create recv;
    if ( !recv.ParseFromString(pkt.msg) )
    {
        log4cppDebug(khaki::logger, "proto parse error : %d", pkt.cmd);
        return false;
    }

    uint64 uid = recv.uid();

    sr::S2R_Create msg;
    uint32 msgId = uint32(sr::ProtoID::ID_S2R_Create);
    msg.set_tokenid(recv.tokenid());
    base::User* user = msg.mutable_user();
    ///////////////// create new role
    user->set_uid(123456);
    user->set_sid(pkt.sid);
    user->set_name("wkw");
    user->set_level(1);
    user->set_money(100);
    ////////////////
    std::string msgStr = msg.SerializeAsString();
    dSession_->SendPacket(msgId, user->uid(), pkt.sid, msgStr);
    log4cppDebug(khaki::logger, "HandlerCreate proto : %d %d", pkt.cmd, uid);
}

///////////RS///////////
bool World::HandlerRSLogin(struct PACKET& pkt) {
    sr::R2S_Login recv;
    if ( !recv.ParseFromString(pkt.msg) )
    {
        log4cppDebug(khaki::logger, "proto parse error : %d", pkt.cmd);
        return false;
    }
    uint32 ret = recv.ret();

    gs::S2G_Login msg;
    uint32 msgId = uint32(gs::ProtoID::ID_S2G_Login);
    msg.set_tokenid(recv.tokenid());
    msg.set_ret(ret);
    std::string msgStr = msg.SerializeAsString();
    gSession_->SendPacket(msgId, pkt.uid, pkt.sid, msgStr);
    log4cppDebug(khaki::logger, "HandlerRSLogin proto : %d %d", pkt.cmd, pkt.uid);
    return true;
}

bool World::HandlerRSCreate(struct PACKET& pkt) {
    sr::R2S_Create recv;
    if ( !recv.ParseFromString(pkt.msg) )
    {
        log4cppDebug(khaki::logger, "proto parse error : %d", pkt.cmd);
        return false;
    }
    uint32 ret = recv.ret();

    gs::S2G_Create msg;
    uint32 msgId = uint32(gs::ProtoID::ID_S2G_Create);
    msg.set_tokenid(recv.tokenid());
    msg.set_ret(ret);
    std::string msgStr = msg.SerializeAsString();
    gSession_->SendPacket(msgId, pkt.uid, pkt.sid, msgStr);
    log4cppDebug(khaki::logger, "HandlerRSCreate proto : %d %d ret=%d", pkt.cmd, pkt.uid, ret);
    return true;
}