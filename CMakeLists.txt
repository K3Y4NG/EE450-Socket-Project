cmake_minimum_required(VERSION 3.3)
project(EE450_Socket_Project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES server.c server.h universal.h)
#set(SOURCE_FILES client.c client.h universal.h)

configure_file(serverA.txt serverA.txt COPYONLY)
configure_file(serverB.txt serverB.txt COPYONLY)
configure_file(serverC.txt serverC.txt COPYONLY)
configure_file(serverD.txt serverD.txt COPYONLY)

add_executable(EE450_Socket_Project ${SOURCE_FILES})