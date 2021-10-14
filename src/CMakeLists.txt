cmake_minimum_required(VERSION 3.0)

project(lota VERSION 0.0.1)
aux_source_directory(. DIR_SRCS)

include_directories("${PROJECT_SOURCE_DIR}/inc")
include_directories("${PROJECT_SOURCE_DIR}/src")

set(CMAKE_C_FLAGS "-Wall -g -lpthread -lcurl -lpaho-mqtt3c -lpaho-mqtt3a")
set(CMAKE_EXE_LINKER_FLAGS " -lpthread -lcurl -lpaho-mqtt3c -lpaho-mqtt3a")

set_target_properties(${TARGET} PROPERTIES
    COMPILE_FLAGS "-lpthread -lcurl -lpaho-mqtt3c -lpaho-mqtt3a"
    LINK_FLAGS "-lpthread -lcurl -lpaho-mqtt3c -lpaho-mqtt3a"
)

link_directories("/usr/local/lib")
add_executable(${PROJECT_NAME} ${DIR_SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE curl pthread paho-mqtt3c paho-mqtt3a)