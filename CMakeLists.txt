cmake_minimum_required(VERSION 3.0)

project(lota VERSION 0.0.1)
aux_source_directory(src DIR_SRCS)

add_executable(${PROJECT_NAME} ${DIR_SRCS})

