#include "../server/khaki/log.h"
#include "../server/khaki/khaki.h"
#include <unordered_map>

class gameSession {
public: 
    gameSession(khaki::EventLoop* loop, std::string host, int port, int threadNum);
    ~gameSession();

    void start();

    void OnConnection(const khaki::TcpClientPtr& con);
	void OnConnClose(const khaki::TcpClientPtr& con);
	void OnMessage(const khaki::TcpClientPtr& con);
private:
    khaki::TcpThreadServer server_;
	std::mutex mtx_;
	std::unordered_map<int, khaki::TcpWeakPtr> sessionLists_;
};