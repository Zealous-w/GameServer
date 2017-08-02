#ifndef WORLD_H
#define WORLD_H
#include <map>
#include <thread>
#include <base/basic.h>
#include <base/common.h>
#include <player.h>
#include <Queue.h>

#include <gateSession.h>
#include <dbSession.h>

#define REGISTER_CMD_CALLBACK(cmdId, func) \
    command_[uint32(cmdId)]  = std::bind(&World::func, this, std::placeholders::_1)


class World {
public:
    typedef std::function<bool(struct PACKET&)> ServiceFunc;
    typedef std::map<uint64, player*>           MapUsers;
    static World& getInstance() {
        static World w;
        return w;
    }
private:
    World();
    ~World();

private:
    bool running_;
    std::thread thread_;
    MapUsers users_;
    gateSession* gSession_;
    dbSession* dSession_;
    std::map<uint32, ServiceFunc> command_;
    khaki::queue<struct PACKET> msgQueue_;
    khaki::queue<struct PACKET> dbMsgQueue_;
public:
    void Start() { running_ = true; }
    void Stop() { running_ = false; }
    void SetSession(gateSession* gSession, dbSession* dSession) { gSession_ = gSession; dSession_ = dSession; }
    void Run();
    void PushGateMsg(struct PACKET& t) { msgQueue_.push(t); }
    void PushDbMsg(struct PACKET& t) { dbMsgQueue_.push(t); }
    void MsgProcess(khaki::queue<struct PACKET>& msg);
    void RegisterCmd();
    void DispatcherCmd(struct PACKET& msg);

public:
    bool HandlerLogin(struct PACKET& pkt);

public:
    ///////dbs -> gs
    bool HandlerRSLogin(struct PACKET& pkt);
};

#define gWorld World::getInstance()

#endif