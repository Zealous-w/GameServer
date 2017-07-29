#include <gameSession.h>

gameSession::gameSession(dbServer* server, const khaki::TcpClientPtr& conn):
    server_(server), conn_(conn) {
    conn_->setReadCallback(std::bind(&gameSession::OnMessage, 
                        this, std::placeholders::_1));
    RegisterCmd();
}

gameSession::~gameSession() {

}

void gameSession::RegisterCmd() {
    command_[cs::ProtoID::ID_C2S_Ping] = std::bind(&gameSession::HandlerPing, this, std::placeholders::_1);
}

void gameSession::DispatcherCmd(struct PACKET& msg) {
    if ( command_.find(msg.cmd) != command_.end() ) {
        command_[msg.cmd](msg);
    } else {
        log4cppDebug(khaki::logger, "error proto : %d", msg.cmd);
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

bool gameSession::HandlerPing(struct PACKET& pkt) {
    cs::C2S_Ping recv;
    if ( !recv.ParseFromString(pkt.msg) )
    {
        log4cppDebug(khaki::logger, "proto parse error : %d", pkt.cmd);
        return false;
    }
    log4cppDebug(khaki::logger, "HandlerPing uid : %d, sid : %d, cmd : %d", pkt.uid, pkt.sid, pkt.cmd);
}

bool gameSession::HandlerDirtyPacket(struct PACKET& str) {

}