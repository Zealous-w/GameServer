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
    gs::S2G_RegisterServer msg;

    msg.set_sid(1);


}

void gateSession::SendPacket(uint32 cmd, std::string& msg) {
    struct PACKET pkt;
    pkt.len = PACKET_HEAD_LEN + msg.size();
    pkt.cmd = cmd;
    pkt.uid = 0;
    pkt.sid = 0;
    pkt.msg = msg;
    SendPacket(pkt);
}

void gateSession::SendPacket(struct PACKET& pkt) {
    std::string msg = Encode(pkt);
    conn_.send(msg.c_str(), msg.size());
}

bool gateSession::HandlerRegisterSid(struct PACKET& str) {
    return false;
}

bool gateSession::HandlerDirtyPacket(struct PACKET& str) {
    return false;
}