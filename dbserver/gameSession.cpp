#include <gameSession.h>
#include <dbMaster.h>
#include <dbServer.h>
gameSession::gameSession(dbServer* server, const khaki::TcpClientPtr& conn):
    server_(server), conn_(conn) {
    conn_->setReadCallback(std::bind(&gameSession::OnMessage, 
                        this, std::placeholders::_1));
    RegisterCmd();
}

gameSession::~gameSession() {

}

void gameSession::RegisterCmd() {
    command_[sr::ProtoID::ID_S2R_Ping] = std::bind(&gameSession::HandlerPing, this, std::placeholders::_1);
    command_[sr::ProtoID::ID_S2R_RegisterServer] = std::bind(&gameSession::HandlerRegisterSid, this, std::placeholders::_1);
}

void gameSession::DispatcherCmd(struct PACKET& msg) {
    if ( command_.find(msg.cmd) != command_.end() ) {
        command_[msg.cmd](msg);
    } else {
        gdbMaster.Push(msg);
    }
}

void gameSession::OnMessage(const khaki::TcpClientPtr& con) {
    khaki::Buffer& buf = con->getReadBuf();
    while( buf.size() > 0 ) {
        if (!buf.checkInt32()) break;
        struct PACKET pkt = Decode(buf);
        DispatcherCmd(pkt);
    }
}

void gameSession::SendPacket(uint32 cmd, std::string& msg) {
    struct PACKET pkt;
    pkt.len = PACKET_HEAD_LEN + msg.size();
    pkt.cmd = cmd;
    pkt.uid = 0;
    pkt.sid = sid_;
    pkt.msg = msg;
    SendPacket(pkt);
}

void gameSession::SendPacket(struct PACKET& pkt) {
    std::string msg = Encode(pkt);
    conn_->send(msg.c_str(), msg.size());
}

bool gameSession::HandlerPing(struct PACKET& pkt) {
    sr::S2R_Ping recv;
    if ( !recv.ParseFromString(pkt.msg) )
    {
        log4cppDebug(khaki::logger, "proto parse error : %d", pkt.cmd);
        return false;
    }
    log4cppDebug(khaki::logger, "dbServer HandlerPing uid : %d, sid : %d, cmd : %d", pkt.uid, pkt.sid, pkt.cmd);
}

bool gameSession::HandlerRegisterSid(struct PACKET& str) {
    sr::S2R_RegisterServer recv;
    if ( !recv.ParseFromString(str.msg) )
    {
        log4cppDebug(khaki::logger, "proto parse error : %d", str.cmd);
        return false;
    }

    sid_ = recv.sid();
    server_->AddAuthGameSession(sid_, conn_->getFd());
    sr::R2S_RegisterServer msg;
    uint32 msgId = sr::ProtoID::ID_R2S_RegisterServer;
    msg.set_ret(1);
    msg.set_sid(sid_);
    std::string msgStr = msg.SerializeAsString();
    SendPacket(msgId, msgStr);
    log4cppDebug(khaki::logger, "dbServer HandlerRegisterSid, sid:%d cmd:%d", sid_, str.cmd);
    return false;
}