#ifndef GAME_SERVER_H
#define GAME_SERVER_H
#include <khaki.h>
#include <unordered_map>
#include <gameSession.h>
#include <basic.h>

#include <map>
#include <functional>

class gameServer {
public: 
    typedef std::function<bool(gameSessionPtr&, std::string&)> ServiceFunc;
    gameServer(khaki::EventLoop* loop, std::string host, int port, int threadNum);
    ~gameServer();

    void start();

    void OnConnection(const khaki::TcpClientPtr& con);
	void OnConnClose(const khaki::TcpClientPtr& con);
	void OnMessage(const khaki::TcpClientPtr& con);

    void DispatcherCmd(struct PACKET& msg);
private:
    khaki::TcpThreadServer server_;
	std::mutex mtx_;
	std::unordered_map<int/*fd*/, gameSessionPtr> sessionLists_;
    std::map<uint32, ServiceFunc> command_;

public:
    bool HandlerRegisterSid(gameSessionPtr& session, std::string& str);
};

#endif