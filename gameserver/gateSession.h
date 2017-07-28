#ifndef GATE_SESSION_H
#define GATE_SESSION_H
#include <khaki.h>
#include <Queue.h>
#include <unordered_map>

#include <base/basic.h>
#include <base/common.h>
#include <protocol/out/cs.pb.h>
#include <protocol/in/gs.pb.h>
#include <player.h>

class gateSession {
public:
    typedef std::function<bool(struct PACKET&)> ServiceFunc;
    gateSession(khaki::EventLoop* loop, std::string& host, uint16_t port);
    ~gateSession();

    bool ConnectGateway();
    void Loop();

    void OnConnected(const khaki::TcpConnectorPtr& con);
	void OnMessage(const khaki::TcpConnectorPtr& con);
    void OnConnectClose(const khaki::TcpConnectorPtr& con);

    void RegisterCmd();
    void DispatcherCmd(struct PACKET& msg);

    void SendPacket(struct PACKET& pkt);
    void SendPacket(uint32 cmd, std::string& msg);
private:
	std::mutex mtx_;
    khaki::EventLoop* loop_;
    khaki::TcpConnectorPtr conn_;
    std::map<uint32, ServiceFunc> command_;
    khaki::queue<struct PACKET> msgQueue_;

    std::unordered_map<uint64, player*> clientLists;
public:
    bool HandlerRegisterSid(struct PACKET& str);
    bool HandlerDirtyPacket(struct PACKET& str);
};

#endif