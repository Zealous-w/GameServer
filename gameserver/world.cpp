#include <world.h>
#include <Log.h>
#include <protocol/out/cs.pb.h>

World::World():thread_(&World::Run, this) {
    running_ = false;
}

World::~World() {
    thread_.join();
}

void World::Run() {
    while ( running_ ) {
        //log4cppDebug(khaki::logger, "World::Run");
        usleep(50000);
    }
}

void World::RegisterCmd() {
    REGISTER_CMD_CALLBACK(cs::ProtoID::ID_C2S_Login, HandlerLogin);
}

void World::DispatcherCmd(struct PACKET& msg) {
    if ( command_.find(msg.cmd) != command_.end() ) {
        command_[msg.cmd](msg);
    } else {
        log4cppDebug(khaki::logger, "error proto : %d", msg.cmd);
    }
}

bool World::HandlerLogin(struct PACKET& str) {

}