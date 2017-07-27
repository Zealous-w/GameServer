#include <gameSession.h>
#include <clientSession.h>

gameSession::gameSession(gameServer* server, const khaki::TcpClientPtr& conn) :
        server_(server), conn_(conn) {
    conn_->setReadCallback(std::bind(&gameSession::OnMessage, 
                        this, std::placeholders::_1));
    RegisterCmd();
}
gameSession::~gameSession(){}

void gameSession::OnMessage(const khaki::TcpClientPtr& con) {
    
}

void gameSession::SendToGameServer(std::string& msg) {
    conn_->send(msg.c_str(), msg.size());
}

void gameSession::SendToClient(std::string& msg) {

}

void gameSession::RegisterCmd() {
    command_[gs::ProtoID::ID_S2G_RegisterServer] = std::bind(&gameSession::HandlerRegisterSid, this, std::placeholders::_1);
}

void gameSession::DispatcherCmd(struct PACKET& msg) {
    if ( command_.find(msg.cmd) != command_.end() ) {
        command_[msg.cmd](msg);
    } else {
        log4cppDebug(khaki::logger, "error proto : %d", msg.cmd);
    }
}

bool gameSession::HandlerRegisterSid(struct PACKET& str) {
    gs::S2G_RegisterServer recv;
    if ( !recv.ParseFromString(str.msg) )
    {
        log4cppDebug(khaki::logger, "proto parse error : %d", str.cmd);
        return false;
    }

    uint32 sid = recv.sid();

    return false;
}

bool gameSession::HandlerDirtyPacket(struct PACKET& str) {
    return false;
}