cmake_minimum_required(VERSION 3.1.0)
project(HDK-Status) # Change this line.

find_package(osvr)

add_executable(hdk-status hdk-status.cpp)
set_property(TARGET hdk-status PROPERTY CXX_STANDARD 11)
target_link_libraries(hdk-status osvr::osvrClientKitCpp)
