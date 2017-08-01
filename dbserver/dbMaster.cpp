#include <unistd.h>
#include <dbMaster.h>
#include <Log.h>
#include <protocol/in/sr.pb.h>

dbMaster::dbMaster():thread_(&dbMaster::Run, this) {
    running_ = false;
    RegisterCmd();
}

dbMaster::~dbMaster() {
    thread_.join();
}

void dbMaster::Run() {
    while ( running_ ) {
        if ( msgQueue_.size() > 0 ) {
            std::queue<struct PACKET> tmpQueue = msgQueue_.popAll();
            while ( !tmpQueue.empty() ) {
                struct PACKET pkt = tmpQueue.front();
                DispatcherCmd(pkt);
                tmpQueue.pop();
            }
        }
        usleep(10000);
    }
}

void dbMaster::RegisterCmd() {
    REGISTER_CMD_CALLBACK(sr::ProtoID::ID_S2R_Login, HandlerLogin);
}

void dbMaster::DispatcherCmd(struct PACKET& msg) {
    if ( command_.find(msg.cmd) != command_.end() ) {
        command_[msg.cmd](msg);
    } else {
        log4cppDebug(khaki::logger, "error proto : %d", msg.cmd);
    }
}

bool dbMaster::HandlerLogin(struct PACKET& pkt) {
    sr::S2R_Login recv;
    if ( !recv.ParseFromString(pkt.msg) )
    {
        log4cppDebug(khaki::logger, "proto parse error : %d", pkt.cmd);
        return false;
    }
    uint64 uid = recv.uid();
    base::User user;
    sr::R2S_Login msg;
    uint32 msgId = uint32(sr::ProtoID::ID_R2S_Login);
    if (mysql_->LoadUser(user, uid)) {
        msg.set_ret(2);

    }
    log4cppDebug(khaki::logger, "dbMaster HandlerLogin uid : %d, sid : %d, cmd : %d", pkt.uid, pkt.sid, pkt.cmd);
    return true;
}

bool dbMaster::HandlerCreate(struct PACKET& pkt) {

    return true;
}