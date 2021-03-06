cmake_minimum_required(VERSION 3.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/.. ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/External ${PROJECT_SOURCE_DIR}/Shared)
set(CMAKE_CXX_STANDARD 17)

if(INCLUDEOS)

  add_definitions(-D_INCLUDEOS)

  set(GENERIC_SRC_StormNetCustomBindings
              ./NetClientBackendWebsocket.cpp
              ./NetServerBackendWebsocket.cpp
              ./NetTransmitterServerWebsocket.cpp
              ./NetTransmitterClientWebsocket.cpp
              )
elseif(WEB)
  set(GENERIC_SRC_StormNetCustomBindings
              ./NetClientBackendWebrtc.cpp
              ./NetClientBackendWebsocket.cpp
              ./NetTransmitterClientWebrtc.cpp
              ./NetTransmitterClientWebsocket.cpp
              )
else()
  set(GENERIC_SRC_StormNetCustomBindings
              ./NetClientBackendWebrtc.cpp
              ./NetClientBackendWebsocket.cpp
              ./NetServerBackendWebrtc.cpp
              ./NetServerBackendWebsocket.cpp
              ./NetTransmitterClientWebrtc.cpp
              ./NetTransmitterClientWebsocket.cpp
              ./NetTransmitterServerWebrtc.cpp
              ./NetTransmitterServerWebsocket.cpp
              )
endif()            
            
add_library(StormNetCustomBindings STATIC ${GENERIC_SRC_StormNetCustomBindings} )