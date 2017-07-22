#include <gameSession.h>

gameSession::gameSession(gameServer* server, const khaki::TcpClientPtr& conn) :
        server_(server), conn_(conn) {
    conn_->setReadCallback(std::bind(&gameSession::OnMessage, 
                        this, std::placeholders::_1));
}
gameSession::~gameSession(){}

void gameSession::OnConnection(const khaki::TcpClientPtr& con) {
    log4cppDebug(khaki::logger, "id : %d already connect", con->getFd());
}
void gameSession::OnConnClose(const khaki::TcpClientPtr& con) {

}
void gameSession::OnMessage(const khaki::TcpClientPtr& con) {
    
}