cmake_minimum_required(VERSION 3.20)
project(TodoCppBackend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPORTANT:
# Set MYSQL_CONNECTOR_CPP_ROOT to the folder containing include/ and lib64/
# of your MySQL Connector/C++ installation.
#
# Example:
# cmake -S . -B build -DMYSQL_CONNECTOR_CPP_ROOT="C:/mysql-connector-c++"

if(NOT DEFINED MYSQL_CONNECTOR_CPP_ROOT)
    message(FATAL_ERROR "Pass -DMYSQL_CONNECTOR_CPP_ROOT=C:/path/to/mysql-connector-c++")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${MYSQL_CONNECTOR_CPP_ROOT}/include
)

link_directories(
    ${MYSQL_CONNECTOR_CPP_ROOT}/lib64
)

add_executable(todo_backend
    src/main.cpp
    src/Database.cpp
    src/TodoRepository.cpp
    src/TodoService.cpp
    src/HttpServer.cpp
)

target_link_libraries(todo_backend PRIVATE
    mysqlcppconn8
    Ws2_32
)
