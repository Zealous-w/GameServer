#include <gateSession.h>

gateSession::gateSession(khaki::EventLoop* loop, std::string& host, uint16_t port) :
        loop_(loop), conn_(loop_, host, port) {
    RegisterCmd();
}
gateSession::~gateSession(){}

bool gateSession::ConnectGateway() {
    return conn_.connectServer();
}

void gateSession::Loop() {
    loop_->loop();
}

void gateSession::OnMessage(const khaki::TcpClientPtr& con) {
}

void gateSession::RegisterCmd() {
    command_[gs::ProtoID::ID_G2S_RegisterServer] = std::bind(&gateSession::HandlerRegisterSid, this, std::placeholders::_1);
}

void gateSession::DispatcherCmd(struct PACKET& msg) {
    if ( command_.find(msg.cmd) != command_.end() ) {
        command_[msg.cmd](msg);
    } else {
        log4cppDebug(khaki::logger, "error proto : %d", msg.cmd);
    }
}

void gateSession::RegisterServer() {

}

bool gateSession::HandlerRegisterSid(struct PACKET& str) {
    return false;
}

bool gateSession::HandlerDirtyPacket(struct PACKET& str) {
    return false;
}