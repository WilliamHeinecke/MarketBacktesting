cmake_minimum_required(VERSION 3.31)
project(MarketBacktesting)

set(CMAKE_CXX_STANDARD 20)
find_package(CURL REQUIRED)
add_executable(MarketBacktesting main.cpp
        Stock.cpp
        Stock.h)

target_link_libraries(MarketBacktesting CURL::libcurl)