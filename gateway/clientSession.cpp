#include <clientSession.h>

clientSession::clientSession(clientServer* server, const khaki::TcpClientPtr& conn):
    server_(server), conn_(conn) {
    conn_->setReadCallback(std::bind(&clientSession::OnMessage, 
                        this, std::placeholders::_1));
}

clientSession::~clientSession() {

}

void clientSession::OnMessage(const khaki::TcpClientPtr& con) {

}

void clientSession::SendToServer(std::string& str) {

}