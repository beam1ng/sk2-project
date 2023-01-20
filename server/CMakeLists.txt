cmake_minimum_required(VERSION 3.12)

project(server)

set(SRCS
    src/main.cpp
    src/serverlogic.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
