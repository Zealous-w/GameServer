PROJECT(gateway)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
INCLUDE_DIRECTORIES(../server/khaki)
INCLUDE_DIRECTORIES(../server/khaki/deps/include)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
AUX_SOURCE_DIRECTORY(. SRC_CPP)
ADD_EXECUTABLE(../bin/gateway ${SRC_CPP})

TARGET_LINK_LIBRARIES(../bin/gateway /home/khaki/item/AthenaServer/server/khaki/build/libkhaki.a)
TARGET_LINK_LIBRARIES(../bin/gateway /home/khaki/item/AthenaServer/server/khaki/deps/lib/liblog4cpp.a)