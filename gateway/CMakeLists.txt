PROJECT(gateway)
cmake_minimum_required(VERSION 2.8)
INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(../server/)
INCLUDE_DIRECTORIES(../server/khaki)
INCLUDE_DIRECTORIES(../server/deps/include)
INCLUDE_DIRECTORIES(../server/khaki/deps/include)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread")
MESSAGE(${CMAKE_CURRENT_SOURCE_DIR})
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../server/khaki/build)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../server/khaki/deps/lib)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../server/deps/lib)

AUX_SOURCE_DIRECTORY(. SRC_CPP)
AUX_SOURCE_DIRECTORY(../server/protocol/out SRC_CPP_PROTO_OUT)
AUX_SOURCE_DIRECTORY(../server/protocol/in SRC_CPP_PROTO_IN)
ADD_EXECUTABLE(../bin/gateway ${SRC_CPP} ${SRC_CPP_PROTO_OUT} ${SRC_CPP_PROTO_IN})

TARGET_LINK_LIBRARIES(../bin/gateway libkhaki.a)
TARGET_LINK_LIBRARIES(../bin/gateway liblog4cpp.a)
target_link_libraries(../bin/gateway libprotobuf.a)