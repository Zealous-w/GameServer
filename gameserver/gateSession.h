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

	void OnMessage(const khaki::TcpClientPtr& con);

    void RegisterCmd();
    void DispatcherCmd(struct PACKET& msg);
private:
	std::mutex mtx_;
    khaki::EventLoop* loop_;
    khaki::Connector conn_;
    std::map<uint32, ServiceFunc> command_;
    khaki::queue<struct PACKET> queue_;

    std::unordered_map<uint64, player*> clientLists;
public:
    bool HandlerRegisterSid(struct PACKET& str);
    bool HandlerDirtyPacket(struct PACKET& str);
};

#endif