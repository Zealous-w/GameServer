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
    log4cppDebug(khaki::logger, "client fd : %d add sessionlists", con->getFd());
}
void gameServer::OnConnClose(const khaki::TcpClientPtr& con) {
    std::unique_lock<std::mutex> lck(mtx_);
    assert(sessionLists_.find(con->getFd()) != sessionLists_.end());
    sessionLists_.erase(con->getFd());
}

void gameServer::DispatcherCmd(struct PACKET& msg) {
    // if ( command_.find(msg.cmd) != command_.end() ) {
    //     command_[msg.cmd](player, msg.msg);
    // } else {
    //     log4cpp("error proto : %d", msg.cmd);
    // }
}

bool gameServer::HandlerRegisterSid(gameSessionPtr& session, std::string& str) {
    return false;
}