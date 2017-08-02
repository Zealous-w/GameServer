#ifndef GAME_CLIENT_SESSION_H
#define GAME_CLIENT_SESSION_H
#include <khaki.h>
#include <unordered_map>
#include <Queue.h>
#include <base/basic.h>
#include <base/common.h>
#include <protocol/out/cs.pb.h>

class gameSession;
class clientServer;
class clientSession {
public:
    enum status {
        E_STATUS_NONE   = 0,
        E_STATUS_LOGIN  = 1,
        E_STATUS_CREATE = 2,
        E_STATUS_VALID  = 3,
        E_STATUS_LOGOUT = 4,
    };
    typedef std::function<bool(struct PACKET&)> ServiceFunc;
    clientSession(clientServer* server, const khaki::TcpClientPtr& conn);
    ~clientSession();

	void OnMessage(const khaki::TcpClientPtr& con);

    void RegisterCmd();
    void DispatcherCmd(struct PACKET& msg);

    void SendToServer(struct PACKET& msg);
    void UnAuthSendToServer(struct PACKET& msg);
private:
    uint8 status_;
    uint64 uid;
    std::map<uint32, ServiceFunc> command_;
    khaki::TcpClientPtr conn_;
    clientServer* server_;
    std::weak_ptr<gameSession> gameSession_;
    khaki::queue<struct PACKET> msgQueue_;
public:
    bool HandlerPing(struct PACKET& str);
    bool HandlerLogin(struct PACKET& str);
    bool HandlerDirtyPacket(struct PACKET& str);
};

typedef std::shared_ptr<clientSession> clientSessionPtr;
#endif