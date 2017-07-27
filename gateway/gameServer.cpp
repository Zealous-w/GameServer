#include <gameServer.h>

gameServer::gameServer(khaki::EventLoop* loop, std::string host, int port, int threadNum) :
        server_(loop, host, port, threadNum) {
    server_.setConnectionCallback(std::bind(&gameServer::OnConnection, 
                        this, std::placeholders::_1));
    server_.setConnCloseCallback(std::bind(&gameServer::OnConnClose, 
                        this, std::placeholders::_1));
}

gameServer::~gameServer(){}

void gameServer::start() {
    server_.start();
}

void gameServer::OnConnection(const khaki::TcpClientPtr& con) {
    std::unique_lock<std::mutex> lck(mtx_);
    sessionLists_[con->getFd()] = gameSessionPtr(new gameSession(this, con));
    log4cppDebug(khaki::logger, "gameServer fd : %d add sessionlists", con->getFd());
}
void gameServer::OnConnClose(const khaki::TcpClientPtr& con) {
    std::unique_lock<std::mutex> lck(mtx_);
    assert(sessionLists_.find(con->getFd()) != sessionLists_.end());
    sessionLists_.erase(con->getFd());
    log4cppDebug(khaki::logger, "gametSession fd : %d closed", con->getFd());
}

gameSessionPtr gameServer::GetGameSessionBySid(uint32 sid) {
    std::unique_lock<std::mutex> lck(mtx_);
    gameSessionPtr gs;
    if ( sessionLists_.find(sid) != sessionLists_.end() ) {
        gs = sessionLists_[sid];
    }
    return gs;
}

void gameServer::AddGameSession(uint32 sid, gameSessionPtr& gsp) {
    std::unique_lock<std::mutex> lck(mtx_);
}