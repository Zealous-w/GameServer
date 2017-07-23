#!/bin/bash

protoc -I=./server/proto --cpp_out=./server/protocol/out ./server/proto/cs.proto
protoc -I=./server/proto --cpp_out=./server/protocol/in ./server/proto/gs.proto