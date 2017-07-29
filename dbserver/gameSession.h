#ifndef GAME_SESSION_H
#define GAME_SESSION_H
#include <khaki.h>
#include <unordered_map>
#include <Queue.h>
#include <base/basic.h>
#include <base/common.h>
#include <protocol/out/cs.pb.h>

class dbServer;
class gameSession {
public:
    typedef std::function<bool(struct PACKET&)> ServiceFunc;
    gameSession(dbServer* server, const khaki::TcpClientPtr& conn);
    ~gameSession();

	void OnMessage(const khaki::TcpClientPtr& con);

    void RegisterCmd();
    void DispatcherCmd(struct PACKET& msg);

private:
    uint8 status_;
    uint64 uid;
    std::map<uint32, ServiceFunc> command_;
    khaki::TcpClientPtr conn_;
    dbServer* server_;
    std::weak_ptr<dbServer> dbSession_;
    khaki::queue<struct PACKET> msgQueue_;
public:
    bool HandlerPing(struct PACKET& str);
    bool HandlerLogin(struct PACKET& str);
    bool HandlerDirtyPacket(struct PACKET& str);
};

typedef std::shared_ptr<gameSession> gameSessionPtr;
#endif