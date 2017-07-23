#include <clientServer.h>

clientServer::clientServer(khaki::EventLoop* loop, std::string host, int port, int threadNum) :
        server_(loop, host, port, threadNum) {
    server_.setConnectionCallback(std::bind(&clientServer::OnConnection, 
                        this, std::placeholders::_1));
    server_.setConnCloseCallback(std::bind(&clientServer::OnConnClose,
                        this, std::placeholders::_1));
}

clientServer::~clientServer() {

}

void clientServer::start() {
    server_.start();
}

void clientServer::OnConnection(const khaki::TcpClientPtr& con) {
    std::unique_lock<std::mutex> lck(mtx_);
    //sessionLists_[con->getFd()] = gameSessionPtr(new gameSession(this, con));
    log4cppDebug(khaki::logger, "clientServer fd : %d add sessionlists", con->getFd());
}
void clientServer::OnConnClose(const khaki::TcpClientPtr& con) {
    std::unique_lock<std::mutex> lck(mtx_);
    //assert(sessionLists_.find(con->getFd()) != sessionLists_.end());
    //sessionLists_.erase(con->getFd());
    log4cppDebug(khaki::logger, "clientSession fd : %d closed", con->getFd());
}