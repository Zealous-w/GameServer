#include <iostream>
#include <gameSession.h>
#include <gameServer.h>
#include <global.h>

int main(int argc, char* argv[]) {
    khaki::EventLoop loop;
	khaki::InitLog(khaki::logger, "./gateway.log", log4cpp::Priority::DEBUG);

	gameServer echo(&loop, "127.0.0.1", 9527, 4);
	echo.start();

	loop.loop();

	//////
    //delete g_cServer;
	delete g_gServer;
	log4cpp::Category::shutdown();    
    return 0;
}