#ifndef GAME_SERVER_H
#define GAME_SERVER_H
#include <khaki.h>
#include <Queue.h>
#include <unordered_map>
#include <gameSession.h>
#include <base/common.h>

#include <map>
#include <functional>

class gameServer {
public: 
    //typedef std::function<bool(gameSessionPtr&, std::string&)> ServiceFunc;
    gameServer(khaki::EventLoop* loop, std::string host, int port, int threadNum);
    ~gameServer();

    void start();

    void OnConnection(const khaki::TcpClientPtr& con);
	void OnConnClose(const khaki::TcpClientPtr& con);

    gameSessionPtr GetGameSessionBySid(uint32 sid);
    void AddGameSession(uint32 sid, gameSessionPtr& gsp);
private:
    khaki::TcpThreadServer server_;
	std::mutex mtx_;
	std::unordered_map<uint32, gameSessionPtr> sessionLists_;
    std::map<uint32/*sid*/, uint32/*fd*/> unauthList;
};

#endif