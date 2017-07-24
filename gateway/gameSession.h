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
private:
	std::mutex mtx_;
    uint64 sid;
    khaki::TcpClientPtr conn_;
    gameServer* server_;
    std::map<uint32, ServiceFunc> command_;
    khaki::queue<struct PACKET> queue_;

    std::unordered_map<uint32, std::shared_ptr<clientSession>> clientLists;
public:
    bool HandlerRegisterSid(struct PACKET& str);
    bool HandlerDirtyPacket(struct PACKET& str);
};

typedef std::shared_ptr<gameSession> gameSessionPtr;
#endif