#include <iostream>
#include "gameSession.h"
#include "gameServer.h"

int main(int argc, char* argv[]) {
    khaki::EventLoop loop;
	khaki::InitKhakiLog(khaki::logger, "./gateway.log", log4cpp::Priority::DEBUG);

	gameSession echo(&loop, "127.0.0.1", 9527, 4);
	echo.start();

	loop.loop();
    return 0;
}