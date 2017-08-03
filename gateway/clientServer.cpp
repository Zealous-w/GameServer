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
    sessionLists_[con->getFd()] = clientSessionPtr(new clientSession(this, con));
    log4cppDebug(khaki::logger, "clientServer fd : %d add sessionlists", con->getFd());
}
void clientServer::OnConnClose(const khaki::TcpClientPtr& con) {
    std::unique_lock<std::mutex> lck(mtx_);
    assert(sessionLists_.find(con->getFd()) != sessionLists_.end());
    sessionLists_.erase(con->getFd());
    log4cppDebug(khaki::logger, "clientSession fd : %d closed", con->getFd());
}

void clientServer::SendPacketByTokenId(uint64 tokenId, struct PACKET& pkt) {
    auto authClient = authList_.find(tokenId);
    if (authClient == authList_.end()) {
        return;
    }

    SendPacketByUid(authClient->second, pkt);
}

void clientServer::SendPacketByUid(uint64 uid, struct PACKET& pkt) {
    auto authClient = sessionLists_.find(uid);
    if (authClient == sessionLists_.end()) {
        return;
    }

    authClient->second->SendPacket(pkt);
}

void clientServer::AddAuthSession(uint64 uid, uint32 sockFd) {
    if (authList_.find(uid) == authList_.end()) {
        return;
    }

    authList_.insert(std::make_pair(uid, sockFd));
}

void clientServer::RemoveAuthSession(uint32 uid) {
    authList_.erase(uid);
}

void clientServer::AddTokenSession(uint64 tokenId, uint32 sockFd) {
    if (tokenList_.find(tokenId) == tokenList_.end()) {
        return;
    }

    tokenList_.insert(std::make_pair(tokenId, sockFd));
}

void clientServer::RemoveTokenSession(uint64 tokenId) {
    tokenList_.erase(tokenId);
}