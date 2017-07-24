#ifndef GAME_CLIENT_SESSION_H
#define GAME_CLIENT_SESSION_H
#include <khaki.h>
#include <unordered_map>
#include <base/basic.h>
#include <base/common.h>
#include <protocol/out/cs.pb.h>

class gameSession;
class clientServer;
class clientSession {
public:
    enum {
        E_CLIENT_NONE = 0,
        E_CLIENT_RUNNING = 1,
    };
    typedef std::function<bool(struct PACKET&)> ServiceFunc;
    clientSession(clientServer* server, const khaki::TcpClientPtr& conn);
    ~clientSession();

	void OnMessage(const khaki::TcpClientPtr& con);

    void RegisterCmd();
    void DispatcherCmd(struct PACKET& msg);

    void SendToServer(struct PACKET& msg);
private:
    uint8 status_;
    uint64 uid;
    std::map<uint32, ServiceFunc> command_;
    khaki::TcpClientPtr conn_;
    clientServer* server_;
    std::weak_ptr<gameSession> gameSession_;
public:
    bool HandlerLogin(struct PACKET& str);
    bool HandlerDirtyPacket(struct PACKET& str);
};

typedef std::shared_ptr<clientSession> clientSessionPtr;
#endif