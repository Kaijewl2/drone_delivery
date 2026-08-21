cmake_minimum_required(VERSION 3.15)

project(go_to_location)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Strict warning handling
add_definitions()

find_package(MAVSDK REQUIRED)

# App's executable and source files
add_executable(go_to_location
  go_to_location.cpp
)

# App's executable and linked libraries
target_link_libraries(go_to_location
    MAVSDK::mavsdk
)

