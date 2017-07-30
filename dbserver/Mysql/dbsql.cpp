#include "dbsql.h"
#include <Log.h>

DbSQL::DbSQL(std::string host, uint16_t port, std::string dbName, std::string user, std::string pwd):
        host_(host),port_(port),dbName_(dbName),user_(user),pwd_(pwd), db_(false),query_(db_.query()) {
}

DbSQL::~DbSQL() {

}

bool DbSQL::ConnectionDatabase() {
    bool ret = db_.connect(dbName_.c_str(), host_.c_str(), user_.c_str(), pwd_.c_str(), port_);
    if (ret == false) {
        log4cppDebug(khaki::logger, "mysql connect failed, h:%s, p:%d, db:%s", host_.c_str(), port_, dbName_.c_str());
        return false;
    }
    return true;
}

bool DbSQL::CreateDbDatabase(std::string& dbName) {
    std::string sql = khaki::util::string_format("create dababase %s", dbName.c_str());
    bool ret = query_.exec(sql.c_str());
    if (ret) {
        return true;
    } else {
        log4cppDebug(khaki::logger, "CreateDbDatabase, query failed, %s", sql.c_str());
        return false;
    }
}

bool DbSQL::CreateDbTable(std::string& dbTable) {
    bool ret = query_.exec(dbTable.c_str());
    if (ret) {
        return true;
    } else {
        log4cppDebug(khaki::logger, "CreateDbTable, query failed, %s", dbTable.c_str());
        return false;
    }
}

mysqlpp::StoreQueryResult DbSQL::GetData(std::string& sql) {
    mysqlpp::StoreQueryResult ret;
    ret = query_.store();
    if (ret) {
        
    } else {
        log4cppDebug(khaki::logger, "query failed, %s", sql.c_str());
    }
    return ret;
}

bool DbSQL::SaveData() {

}

bool DbSQL::ExectueSql(std::string& sql) {

}
void DbSQL::CloseMysql() {

}

bool DbSQL::LoadUser(uint64 uid) {
    return false;
}

bool DbSQL::SaveUser(uint64 uid) {
    return false;
}

bool DbSQL::SaveBaseInfo(base::user& user) {
    return false;
}

base::user DbSQL::GetUserBaseInfo(uint64 uid) {
    base::user uInfo;
    std::string sql = khaki::util::string_format("select * from user where uid=%d", uid);
    mysqlpp::StoreQueryResult res;
    mysqlpp::StoreQueryResult  ret = query_.store(sql.c_str());
    if (res) {
        for (auto iter = res.begin(); iter != res.end(); ++iter) {

        }
    } else {
        log4cppDebug(khaki::logger, "GetUserBaseInfo, query failed, %s", sql.c_str());
    }
    return uInfo;
}