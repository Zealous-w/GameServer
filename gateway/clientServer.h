#ifndef GAME_CLIENT_SERVER_H
#define GAME_CLIENT_SERVER_H
#include <khaki.h>
#include <Queue.h>
#include <unordered_map>
#include <clientSession.h>
#include <base/common.h>

#include <map>
#include <functional>

class clientServer {
public: 
    clientServer(khaki::EventLoop* loop, std::string host, int port, int threadNum);
    ~clientServer();

    void start();

    void OnConnection(const khaki::TcpClientPtr& con);
	void OnConnClose(const khaki::TcpClientPtr& con);
    void SendPacketByTokenId(uint64 tokenId, struct PACKET& pkt);
    void SendPacketByUid(uint64 uid, struct PACKET& pkt);

    void AddAuthSession(uint64 uid, uint32 sockFd);
    void RemoveAuthSession(uint32 uid);

    void AddTokenSession(uint64 tokenId, uint32 sockFd);
    void RemoveTokenSession(uint64 tokenId);
private:
    khaki::TcpThreadServer server_;
	std::mutex mtx_;
	std::unordered_map<uint32/*fd*/, clientSessionPtr> sessionLists_;
    std::map<uint64/*uid*/, uint32/*fd*/> authList_;
    std::map<uint64/*tokenId*/, uint32/*fd*/> tokenList_;
    khaki::queue<struct PACKET> queue_;
};

#endif