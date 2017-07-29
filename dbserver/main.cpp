#include <dbServer.h>

int main(int argc, char* argv[]) {
    khaki::EventLoop loop;
	khaki::InitLog(khaki::logger, "./dbserver.log", log4cpp::Priority::DEBUG);

	dbServer* g_dbServer = new dbServer(&loop, "127.0.0.1", 9529, 4);
	
	g_dbServer->start();

	loop.loop();
	//////
	delete g_dbServer;
	log4cpp::Category::shutdown();
	google::protobuf::ShutdownProtobufLibrary();
    return 0;
}