#include <iostream>
#include "gameSession.h"
#include "gameServer.h"

int main(int argc, char* argv[]) {
    khaki::EventLoop loop;
	khaki::Log::getLog().setLogLevel(khaki::Log::LogLevel::E_LOG_INFO);

	gameSession echo(&loop, "127.0.0.1", 9527, 4);
	echo.start();

	loop.loop();
    return 0;
}