cmake_minimum_required(VERSION 3.15)

project(go_to_location)

# Match MAVSDK library (C++20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable strict handling of warnings
add_definitions("-Wall -Wextra -Werror")

# Find MAVSDK when installed system wide
find_package(MAVSDK REQUIRED)

# Specify app's executable name and list of source files used to create it
# LAST: figure out how architechture of running script will work
add_executable(your_executable_name
    your_source_file.cpp
    # ... any other source files
)

# Specify your app's executable name and a list of linked libraries
target_link_libraries(your_executable_name
    MAVSDK::mavsdk             #All apps link against mavsdk library
    MAVSDK::mavsdk_action      # If action plugin used by app ...
    MAVSDK::mavsdk_telemetry   #If telemetry plugin used by app ...
    # ... Any other linked libraries
)
