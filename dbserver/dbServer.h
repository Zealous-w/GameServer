#ifndef GAME_CLIENT_SERVER_H
#define GAME_CLIENT_SERVER_H
#include <khaki.h>
#include <Queue.h>
#include <unordered_map>
#include <gameSession.h>
#include <base/common.h>

#include <map>
#include <functional>

class dbServer {
public: 
    dbServer(khaki::EventLoop* loop, std::string host, int port, int threadNum);
    ~dbServer();

    void start();

    void OnConnection(const khaki::TcpClientPtr& con);
	void OnConnClose(const khaki::TcpClientPtr& con);
private:
    khaki::TcpThreadServer server_;
	std::mutex mtx_;
	std::unordered_map<uint32/*sid*/, gameSessionPtr> sessionLists_;
    khaki::queue<struct PACKET> queue_;
};

#endif