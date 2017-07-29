#include <dbServer.h>

dbServer::dbServer(khaki::EventLoop* loop, std::string host, int port, int threadNum) :
        server_(loop, host, port, threadNum) {
    server_.setConnectionCallback(std::bind(&dbServer::OnConnection, 
                        this, std::placeholders::_1));
    server_.setConnCloseCallback(std::bind(&dbServer::OnConnClose,
                        this, std::placeholders::_1));
}

dbServer::~dbServer() {

}

void dbServer::start() {
    server_.start();
}

void dbServer::OnConnection(const khaki::TcpClientPtr& con) {
    std::unique_lock<std::mutex> lck(mtx_);
    sessionLists_[con->getFd()] = gameSessionPtr(new gameSession(this, con));
    log4cppDebug(khaki::logger, "clientServer fd : %d add sessionlists", con->getFd());
}
void dbServer::OnConnClose(const khaki::TcpClientPtr& con) {
    std::unique_lock<std::mutex> lck(mtx_);
    assert(sessionLists_.find(con->getFd()) != sessionLists_.end());
    sessionLists_.erase(con->getFd());
    log4cppDebug(khaki::logger, "clientSession fd : %d closed", con->getFd());
}