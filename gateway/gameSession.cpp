#include <gameSession.h>
#include <clientSession.h>
#include <global.h>

gameSession::gameSession(gameServer* server, const khaki::TcpClientPtr& conn) :
        server_(server), conn_(conn) {
    conn_->setReadCallback(std::bind(&gameSession::OnMessage, 
                        this, std::placeholders::_1));
    RegisterCmd();
}
gameSession::~gameSession(){}

void gameSession::OnMessage(const khaki::TcpClientPtr& con) {
    khaki::Buffer& buf = con->getReadBuf();
    while( buf.size() > 0 ) {
        if (!buf.checkInt32()) break;
        struct PACKET pkt = Decode(buf);
        DispatcherCmd(pkt);
    }
}

void gameSession::SendToGameServer(std::string& msg) {
    conn_->send(msg.c_str(), msg.size());
}

void gameSession::SendToClient(std::string& msg) {

}

void gameSession::RegisterCmd() {
    command_[gs::ProtoID::ID_S2G_Ping] = std::bind(&gameSession::HandlerPing, this, std::placeholders::_1);
    command_[gs::ProtoID::ID_S2G_RegisterServer] = std::bind(&gameSession::HandlerRegisterSid, this, std::placeholders::_1);
}

void gameSession::DispatcherCmd(struct PACKET& msg) {
    if ( command_.find(msg.cmd) != command_.end() ) {
        command_[msg.cmd](msg);
    } else {
        log4cppDebug(khaki::logger, "error proto : %d", msg.cmd);
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

bool gameSession::HandlerPing(struct PACKET& str) {
    gs::S2G_Ping recv;
    if ( !recv.ParseFromString(str.msg) )
    {
        log4cppDebug(khaki::logger, "proto parse error : %d", str.cmd);
        return false;
    }
    uint32 now_time = recv.now_time();
    log4cppDebug(khaki::logger, "gameSession::HandlerPing : %d, time:%d", str.cmd, now_time);
}

bool gameSession::HandlerRegisterSid(struct PACKET& str) {
    gs::S2G_RegisterServer recv;
    if ( !recv.ParseFromString(str.msg) )
    {
        log4cppDebug(khaki::logger, "proto parse error : %d", str.cmd);
        return false;
    }

    sid_ = recv.sid();
    g_gServer->AddAuthGameSession(sid_, conn_->getFd());
    gs::G2S_RegisterServer msg;
    uint32 msgId = gs::ProtoID::ID_G2S_RegisterServer;
    msg.set_ret(1);
    msg.set_sid(sid_);
    std::string msgStr = msg.SerializeAsString();
    SendPacket(msgId, msgStr);
    log4cppDebug(khaki::logger, "gateway HandlerRegisterSid : %d", str.cmd);
    return false;
}

bool gameSession::HandlerDirtyPacket(struct PACKET& str) {
    //
    return false;
}