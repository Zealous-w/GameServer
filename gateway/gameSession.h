#ifndef GAME_SESSION_H
#define GAME_SESSION_H
#include <khaki.h>
#include <Queue.h>
#include <unordered_map>

#include <base/basic.h>
#include <base/common.h>
#include <protocol/out/cs.pb.h>
#include <protocol/in/gs.pb.h>

class clientSession;
class gameServer;
class gameSession {
public:
    typedef std::function<bool(struct PACKET&)> ServiceFunc;
    gameSession(gameServer* server, const khaki::TcpClientPtr& conn);
    ~gameSession();

	void OnMessage(const khaki::TcpClientPtr& con);

    void RegisterCmd();
    void DispatcherCmd(struct PACKET& msg);

    void SendToGameServer(std::string& msg);

    void SendToClient(std::string& msg);
    void SendPacket(struct PACKET& pkt);
    void SendPacket(uint32 cmd, std::string& msg);
    uint32 GetSid() { return sid_; }
private:
	std::mutex mtx_;
    uint32 sid_;
    khaki::TcpClientPtr conn_;
    gameServer* server_;
    std::map<uint32, ServiceFunc> command_;
    khaki::queue<struct PACKET> msgQueue_;

    std::unordered_map<uint32, std::shared_ptr<clientSession>> clientLists;
public:
    bool HandlerPing(struct PACKET& str);
    bool HandlerRegisterSid(struct PACKET& str);
    bool HandlerDirtyPacket(struct PACKET& str);
};

typedef std::shared_ptr<gameSession> gameSessionPtr;
#endif