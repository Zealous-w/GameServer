#include "gameSession.h"

gameSession::gameSession(khaki::EventLoop* loop, std::string host, int port, int threadNum) :
        server_(loop, host, port, threadNum) {
    server_.setReadCallback(std::bind(&gameSession::OnMessage, 
                        this, std::placeholders::_1));
    server_.setConnectionCallback(std::bind(&gameSession::OnConnection, 
                        this, std::placeholders::_1));
    server_.setConnCloseCallback(std::bind(&gameSession::OnConnClose, 
                        this, std::placeholders::_1));
}
gameSession::~gameSession(){}

void gameSession::start() {
    server_.start();
}

void gameSession::OnConnection(const khaki::TcpClientPtr& con) {

}
void gameSession::OnConnClose(const khaki::TcpClientPtr& con) {

}
void gameSession::OnMessage(const khaki::TcpClientPtr& con) {
    
}