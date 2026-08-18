cmake_minimum_required(VERSION 3.15)

# Specify your project's name
project(TOL_test)

# Match the library/examples (C++20). C++17 may work for apps that only need older language surface.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable strict handling of warnings
add_definitions("-Wall -Wextra -Werror")

# Finds MAVSDK when installed system wide.
find_package(MAVSDK REQUIRED)

# Specify your app's executable name, and list of source files used to create it.
add_executable(TOL_test
  takeoff_and_land.cpp
    # ... any other source files
)

# Specify your app's executable name and a list of linked libraries
target_link_libraries(TOL_test
    MAVSDK::mavsdk             #All apps link against mavsdk library
    #MAVSDK::mavsdk_action      # If action plugin used by app ...
    #MAVSDK::mavsdk_telemetry   #If telemetry plugin used by app ...
    # ... Any other linked libraries
)
