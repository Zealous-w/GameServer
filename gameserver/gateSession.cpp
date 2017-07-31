#include <gateSession.h>
#include <Util.h>
#include <world.h>

gateSession::gateSession(khaki::EventLoop* loop, std::string& host, uint16_t port) :
        loop_(loop), conn_(new khaki::Connector(loop_, host, port)) {
    conn_->setConnectCallback(std::bind(&gateSession::OnConnected, this, std::placeholders::_1));
    conn_->setCloseCallback(std::bind(&gateSession::OnConnectClose, this, std::placeholders::_1));
    conn_->setReadCallback(std::bind(&gateSession::OnMessage, this, std::placeholders::_1));
    RegisterCmd();
}
gateSession::~gateSession(){}

bool gateSession::ConnectGateway() {
    return conn_->connectServer();
}

void gateSession::Loop() {
    loop_->loop();
}

void gateSession::Heartbeat() {
    gs::S2G_Ping msg;
    msg.set_now_time(khaki::util::getTime());
    std::string str = msg.SerializeAsString();
    
    SendPacket(uint32(gs::ProtoID::ID_S2G_Ping), str);
    //log4cppDebug(khaki::logger, "gateSession::Heartbeat()");
}

void gateSession::OnConnected(const khaki::TcpConnectorPtr& con) {
    gs::S2G_RegisterServer msg;
    log4cppDebug(khaki::logger, "OnConnected");
    msg.set_sid(1);

    std::string str = msg.SerializeAsString();
    SendPacket(uint32(gs::ProtoID::ID_S2G_RegisterServer), str);
    loop_->getTimer()->AddTimer(std::bind(&gateSession::Heartbeat, this), khaki::util::getTime(), 10);/*10s tick*/
}

void gateSession::OnMessage(const khaki::TcpConnectorPtr& con) {
    khaki::Buffer& buf = con->getReadBuf();
    while( buf.size() > 0 ) {
        if (!buf.checkInt32()) break;
        struct PACKET pkt = Decode(buf);
        DispatcherCmd(pkt);
    }
}

void gateSession::OnConnectClose(const khaki::TcpConnectorPtr& con) {
    log4cppDebug(khaki::logger, "OnConnectClose");
}

void gateSession::RegisterCmd() {
    command_[gs::ProtoID::ID_G2S_RegisterServer] = std::bind(&gateSession::HandlerRegisterSid, this, std::placeholders::_1);
    ///////////////////////////
}

void gateSession::DispatcherCmd(struct PACKET& msg) {
    if ( command_.find(msg.cmd) != command_.end() ) {
        command_[msg.cmd](msg);
    } else {
        HandlerDirtyPacket(msg);
        //log4cppDebug(khaki::logger, "error proto : %d", msg.cmd);
    }
}

void gateSession::SendPacket(uint32 cmd, std::string& msg) {
    struct PACKET pkt;
    pkt.len = PACKET_HEAD_LEN + msg.size();
    pkt.cmd = cmd;
    pkt.uid = 0;
    pkt.sid = sid_;
    pkt.msg = msg;
    SendPacket(pkt);
}

void gateSession::SendPacket(uint32 cmd, uint64 uid, std::string& msg) {
    struct PACKET pkt;
    pkt.len = PACKET_HEAD_LEN + msg.size();
    pkt.cmd = cmd;
    pkt.uid = uid;
    pkt.sid = sid_;
    pkt.msg = msg;
    SendPacket(pkt);
}

void gateSession::SendPacket(struct PACKET& pkt) {
    std::string msg = Encode(pkt);
    conn_->send(msg.c_str(), msg.size());
}

bool gateSession::HandlerRegisterSid(struct PACKET& str) {
    log4cppDebug(khaki::logger, "gateway, HandlerRegisterSid Success %d", str.cmd);
    return false;
}

bool gateSession::HandlerDirtyPacket(struct PACKET& str) {
    gWorld.Push(str);
    return true;
}