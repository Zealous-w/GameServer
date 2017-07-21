#include "gameServer.h"

gameServer::gameServer(khaki::EventLoop* loop, std::string host, int port, int threadNum) :
        server_(loop, host, port, threadNum) {
    server_.setReadCallback(std::bind(&gameServer::OnMessage, 
                        this, std::placeholders::_1));
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

}
void gameServer::OnConnClose(const khaki::TcpClientPtr& con) {

}
void gameServer::OnMessage(const khaki::TcpClientPtr& con) {
    
}