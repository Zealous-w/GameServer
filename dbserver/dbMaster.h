#ifndef DBMASTER_H
#define DBMASTER_H
#include <map>
#include <thread>
#include <base/basic.h>
#include <base/common.h>
#include <Queue.h>
#include <Mysql/dbsql.h>

#define REGISTER_CMD_CALLBACK(cmdId, func) \
    command_[uint32(cmdId)]  = std::bind(&dbMaster::func, this, std::placeholders::_1)


class dbMaster {
public:
    typedef std::function<bool(struct PACKET&)> ServiceFunc;
    static dbMaster& getInstance() {
        static dbMaster w;
        return w;
    }
private:
    dbMaster();
    ~dbMaster();

private:
    bool running_;
    std::thread thread_;
    DbSQL* mysql_;
    std::map<uint32, ServiceFunc> command_;
    khaki::queue<struct PACKET> msgQueue_;
public:
    bool Start() { running_ = true; }
    void Stop() { running_ = false; }
    void Run();
    void SetDbSQL(DbSQL* db) { mysql_ = db; }
    void Push(struct PACKET& t) { msgQueue_.push(t); }

    void RegisterCmd();
    void DispatcherCmd(struct PACKET& msg);

public:
    bool HandlerLogin(struct PACKET& str);
    
};

#define gdbMaster dbMaster::getInstance()
#endif