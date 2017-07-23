#ifndef GAME_CLIENT_SESSION_H
#define GAME_CLIENT_SESSION_H
#include <khaki.h>
#include <unordered_map>
#include <base/basic.h>

class clientServer;
class clientSession {
public:
    clientSession(clientServer* server, const khaki::TcpClientPtr& conn);
    ~clientSession();

	void OnMessage(const khaki::TcpClientPtr& con);

    void SendToServer(std::string& str);
private:
	std::mutex mtx_;
    uint64 uid;
    khaki::TcpClientPtr conn_;
    clientServer* server_;

public:

};

typedef std::shared_ptr<clientSession> clientSessionPtr;
#endif