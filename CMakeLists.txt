project(BoxWorld)
set(APP BoxWorld)

cmake_minimum_required(VERSION 2.8)

include(../of/openFrameworks.cmake)

if(CMAKE_SYSTEM MATCHES Linux)
set(FILE_WATCHER_IMPLEMENTTION_SOURCE "src/FileWatcher/src/FileWatcherLinux.cpp")
set(SENSOR_SOURCES "src/Sensors/rsf200mgr.cpp src/Sensors/rsF200DepthSensorImp.cpp")
set(SENSOR_LINK_LIB "libivcam.so")
elseif(CMAKE_SYSTEM MATCHES Darwin)
set(FILE_WATCHER_IMPLEMENTTION_SOURCE "src/FileWatcher/src/FileWatcherOSX.cpp")
set(CONN_IMPLEMENTTION_SOURCE "src/Connection/ConnBleOSX.mm")
find_library(MAC_COCOA_FRAMEWORK Cocoa)
find_library(MAC_IOBLUETOOTH_FRAMEWORK IOBluetooth)
find_library(MAC_FOUNDATION_FRAMEWORK Foundation)
set(MAC_TARGET_LINK_LIBRARIES ${MAC_COCOA_FRAMEWORK} ${MAC_IOBLUETOOTH_FRAMEWORK} ${MAC_FOUNDATION_FRAMEWORK})
elseif(CMAKE_SYSTEM MATCHES Windows)
endif()

#ofxaddon()

include_directories("src/")
include_directories("src/App/")
include_directories("src/Audio/")
include_directories("src/Cmd/")
include_directories("src/Connection/")
include_directories("src/FileWatcher/inc/")
include_directories("src/Message/")
include_directories("src/Shader/")
include_directories("src/Shader/Uniform")
include_directories("src/Shader/Uniform/ShadertoyCustom/")
include_directories("src/Thread/")
include_directories("src/jsoncpp/json/")
include_directories("src/ofxCubeMap/src")
include_directories("src/Sensors")
include_directories("src/Sensors/ivcam")

set(SOURCES
    src/App/BoxWorldApp.cpp
    src/App/ResourceMgrInst.cpp
    src/App/main.cpp
    src/Connection/ConnMgrInst.cpp
    src/Connection/ConnSimuImp.cpp
    ${CONN_IMPLEMENTTION_SOURCE}
    src/FileWatcher/src/FileWatcher.cpp
    ${FILE_WATCHER_IMPLEMENTTION_SOURCE}
    src/Shader/GLSLProgram.cpp
    src/Shader/Parser.cpp
    src/Shader/ShaderExecutor.cpp
    src/jsoncpp/jsoncpp.cpp
    src/ofxCubeMap/src/ofxCubeMap.cpp
    src/Message/Message.cpp
    src/Message/MsgBuilder.cpp
    src/Protocol/slip.c
    src/Sensors/SimuDepthSensorImp.cpp
    ${SENSOR_SOURCES}
)

add_executable(
    ${APP}
    MACOSX_BUNDLE
    ${SOURCES}
    ${OFXADDONS_SOURCES}
)

target_link_libraries(
    ${APP}
    ${OPENFRAMEWORKS_LIBRARIES}
    ${MAC_TARGET_LINK_LIBRARIES}
    ${SENSOR_LINK_LIB}
)

set_target_properties(
    ${APP} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST
    "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
)

cotire(${APP})
