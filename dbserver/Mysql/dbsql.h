#ifndef DBSQL_H
#define DBSQL_H 
#include <mysql++/mysql++.h>
#include <base/basic.h>
#include <protocol/in/base.pb.h>

class DbSQL {
public:
    typedef std::vector<std::vector<std::string>> VQueryResult;
    DbSQL(std::string host, uint16_t port, std::string dbName, std::string user, std::string pwd);
    ~DbSQL();
public:
    bool ConnectionDatabase();
    bool CreateDbDatabase(std::string& dbName);
    bool CreateDbTable(std::string& dbTable);
    mysqlpp::StoreQueryResult GetData(std::string& sql);
    bool SaveData();
    bool ExectueSql(std::string& sql);
    void CloseMysql();
    ////////////////////////
    bool LoadUser(uint64 uid);
    bool SaveUser(uint64 uid);
    ////////////////////////

    ////////////////////////
    bool SaveBaseInfo(base::user& user);
    ////////////////////////

    ////////RESULT/////////
    base::user GetUserBaseInfo(uint64 uid);
private:
    std::string host_; 
    uint16_t port_; 
    std::string dbName_; 
    std::string user_; 
    std::string pwd_;
    mysqlpp::Connection db_;
    mysqlpp::Query query_;
};

#endif