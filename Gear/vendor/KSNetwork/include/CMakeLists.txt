cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

project(Engine LANGUAGES CXX)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(nework_header 
    HeaderShared.h
    MemoryStream.h
    Packet.h
    SocketAddress.h
    SocketAddressFactory.h
    SocketUtil.h
    StringUtils.h
    TCPSocket.h
    UDPSocket.h
)
set(network_source 
    HeaderShared.h
    MemoryStream.cpp
    Packet.cpp
    SocketAddress.cpp
    SocketAddressFactory.cpp
    SocketUtil.cpp
    StringUtils.cpp
    TCPSocket.cpp
    UDPSocket.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}../vendor/lib)

add_library(${PROJECT_NAME}  ${nework_header} ${network_source})
