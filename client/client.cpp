#include <iostream>
#include <Log.h>
#include <base/basic.h>
#include <base/common.h>
#include <protocol/out/cs.pb.h>
#include <khaki.h>


class Client {
public:
    typedef std::function<bool(struct PACKET&)> ServiceFunc;
    Client(khaki::EventLoop* loop, std::string host, uint16_t port) : 
        loop_(loop), conn_(new khaki::Connector(loop_, host, port)){
        conn_->setConnectCallback(std::bind(&Client::OnConnected, this, 
                std::placeholders::_1));
        conn_->setCloseCallback(std::bind(&Client::OnConnectClose, this, 
                std::placeholders::_1));
        conn_->setReadCallback(std::bind(&Client::OnMessage, this, 
                std::placeholders::_1));
        RegisterCmd();
    }
    ~Client() {}

    bool ConnectGateway() {
        return conn_->connectServer();
    }

    void Loop() {
        loop_->loop();
    }
    void OnConnected(const khaki::TcpConnectorPtr& con) {
        cs::C2S_Login msg;
        msg.set_uid(uid_);

        std::string str = msg.SerializeAsString();
        SendPacket(uint32(cs::ProtoID::ID_C2S_Login), str);

        ///start tick
        loop_->getTimer()->AddTimer(std::bind(&Client::Heartbeat, this), khaki::util::getTime(), 20);/*10s tick*/
    }
	void OnMessage(const khaki::TcpConnectorPtr& con) {
        khaki::Buffer& buf = con->getReadBuf();
        while( buf.size() > 0 ) {
            if (!buf.checkInt32()) break;
            struct PACKET pkt = Decode(buf);
            DispatcherCmd(pkt);
        }
    }
    void OnConnectClose(const khaki::TcpConnectorPtr& con) {

    }
    void Heartbeat() {
        cs::C2S_Ping msg;
        msg.set_now_time(khaki::util::getTime());
        std::string str = msg.SerializeAsString();
        
        SendPacket(uint32(cs::ProtoID::ID_C2S_Ping), str);
    }
    void RegisterCmd() {
        command_[cs::ProtoID::ID_S2C_Login] = std::bind(&Client::HandlerLogin, this, std::placeholders::_1);
    }
    void DispatcherCmd(struct PACKET& msg) {
        if ( command_.find(msg.cmd) != command_.end() ) {
            command_[msg.cmd](msg);
        } else {
            log4cppDebug(khaki::logger, "error proto : %d", msg.cmd);
        }
    }
    void SendPacket(struct PACKET& pkt) {
        std::string msg = Encode(pkt);
        conn_->send(msg.c_str(), msg.size());
    }
    void SendPacket(uint32 cmd, std::string& msg) {
        struct PACKET pkt;
        pkt.len = PACKET_HEAD_LEN + msg.size();
        pkt.cmd = cmd;
        pkt.uid = uid_;
        pkt.sid = sid_;
        pkt.msg = msg;
        SendPacket(pkt);
    }

    void SetUid(uint32 uid) {
        uid_ = uid;
    }
    void SetSid(uint32 sid) {
        sid_ = sid;
    }
private:
    std::string host;
    uint16_t port;
    uint64 uid_;
    uint32 sid_;
    khaki::EventLoop* loop_;
    khaki::TcpConnectorPtr conn_;
    std::map<uint32, ServiceFunc> command_;

public:
    bool HandlerLogin(struct PACKET& msg) {
        cs::S2C_Login recv;
        if ( !recv.ParseFromString(msg.msg) )
        {
            log4cppDebug(khaki::logger, "proto parse error : %d", msg.cmd);
            return false;
        }

        uint32 ret = recv.ret();

        log4cppDebug(khaki::logger, "HandlerLogin ret : %d", ret);
        return true;
    }
};

int main(int argc, char* argv[]) {
    khaki::EventLoop loop;
    khaki::InitLog(khaki::logger, "./client.log", log4cpp::Priority::DEBUG);
    Client* client = new Client(&loop, "127.0.0.1", 9527);
    if ( !client->ConnectGateway() ) {
        log4cppDebug(khaki::logger, "connect gateway failed !!");
        return 0;
    }
    client->SetUid(123456);
    client->SetSid(1);
    client->Loop();

    ///////
    delete client;
    log4cpp::Category::shutdown();
	google::protobuf::ShutdownProtobufLibrary();
    return 0;
}