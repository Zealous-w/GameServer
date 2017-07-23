#include <gameSession.h>

gameSession::gameSession(gameServer* server, const khaki::TcpClientPtr& conn) :
        server_(server), conn_(conn) {
    conn_->setReadCallback(std::bind(&gameSession::OnMessage, 
                        this, std::placeholders::_1));
    RegisterCmd();
}
gameSession::~gameSession(){}

void gameSession::OnMessage(const khaki::TcpClientPtr& con) {
    
}

void gameSession::RegisterCmd() {
    //command_[] =  
}

void gameSession::SendToClient(std::string& str) {

}

void gameSession::DispatcherCmd(struct PACKET& msg) {
    if ( command_.find(msg.cmd) != command_.end() ) {
        // if (sessionLists_.find(msg.sid) == sessionLists_.end() ) {
        //     log4cppDebug(khaki::logger, "Not Found This Server %d", msg.sid);
        //     return;
        // }
        command_[msg.cmd](msg);
    } else {
        log4cppDebug(khaki::logger, "error proto : %d", msg.cmd);
    }
}

bool gameSession::HandlerRegisterSid(struct PACKET& str) {
    return false;
}

bool gameSession::HandlerDirtyPacket(struct PACKET& str) {
    return false;
}