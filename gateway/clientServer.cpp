#include <clientServer.h>

clientServer::clientServer(khaki::EventLoop* loop, std::string host, int port, int threadNum) :
        server_(loop, host, port, threadNum), markId_(0) {
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
    markId_++;
    con->setUniqueId(markId_);
    sessionLists_[markId_] = clientSessionPtr(new clientSession(this, con));
    log4cppDebug(khaki::logger, "clientServer fd : %d add sessionlists", con->getFd());
}
void clientServer::OnConnClose(const khaki::TcpClientPtr& con) {
    std::unique_lock<std::mutex> lck(mtx_);
    assert(sessionLists_.find(con->getUniqueId()) != sessionLists_.end());
    sessionLists_.erase(con->getUniqueId());
    log4cppDebug(khaki::logger, "clientSession fd : %d closed", con->getFd());
}

void clientServer::SendPacketByUniqueId(uint64 uniqueId, struct PACKET& pkt) {
    auto client = sessionLists_.find(uniqueId);
    if (client == sessionLists_.end()) {
        return;
    }

    client->second->SendPacket(pkt);
}