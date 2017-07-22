#ifndef GAME_SESSION_H
#define GAME_SESSION_H
#include <khaki.h>
#include <unordered_map>
#include <basic.h>

class gameServer;
class gameSession {
public:
    
    gameSession(gameServer* server, const khaki::TcpClientPtr& conn);
    ~gameSession();

    void OnConnection(const khaki::TcpClientPtr& con);
	void OnConnClose(const khaki::TcpClientPtr& con);
	void OnMessage(const khaki::TcpClientPtr& con);
private:
	std::mutex mtx_;
    uint64 sid;
    khaki::TcpClientPtr conn_;
    gameServer* server_;

public:

};

typedef std::shared_ptr<gameSession> gameSessionPtr;
#endif