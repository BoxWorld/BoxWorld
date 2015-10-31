project(BoxWorld)
set(APP BoxWorld)

cmake_minimum_required(VERSION 2.8)

include(../of/openFrameworks.cmake)

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

set(SOURCES
    src/App/BoxWorldApp.cpp
    src/App/ResourceMgrInst.cpp
    src/App/main.cpp
    src/Connection/ConnMgrInst.cpp
    src/Connection/ConnSimuImp.cpp
    src/FileWatcher/src/FileWatcher.cpp
    src/FileWatcher/src/FileWatcherOSX.cpp
    src/Shader/GLSLProgram.cpp
    src/Shader/Parser.cpp
    src/Shader/ShaderExecutor.cpp
    src/jsoncpp/jsoncpp.cpp
    src/ofxCubeMap/src/ofxCubeMap.cpp
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
)

set_target_properties(
    ${APP} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST
    "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
)

cotire(${APP})
