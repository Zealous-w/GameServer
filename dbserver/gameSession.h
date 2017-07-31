#ifndef GAME_SESSION_H
#define GAME_SESSION_H
#include <khaki.h>
#include <unordered_map>
#include <Queue.h>
#include <base/basic.h>
#include <base/common.h>
#include <protocol/in/sr.pb.h>

class dbServer;
class gameSession {
public:
    typedef std::function<bool(struct PACKET&)> ServiceFunc;
    gameSession(dbServer* server, const khaki::TcpClientPtr& conn);
    ~gameSession();

	void OnMessage(const khaki::TcpClientPtr& con);

    void RegisterCmd();
    void DispatcherCmd(struct PACKET& msg);

    void SendPacket(struct PACKET& pkt);
    void SendPacket(uint32 cmd, std::string& msg);
    uint32 GetSid() { return sid_; }
private:
    uint8 status_;
    uint32 sid_;
    std::map<uint32, ServiceFunc> command_;
    khaki::TcpClientPtr conn_;
    dbServer* server_;
    std::weak_ptr<dbServer> dbSession_;
public:
    bool HandlerPing(struct PACKET& str);
    bool HandlerRegisterSid(struct PACKET& str);
};

typedef std::shared_ptr<gameSession> gameSessionPtr;
#endif