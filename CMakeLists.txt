cmake_minimum_required(VERSION 3.12)
project(HydraulicNetwork)

set(CMAKE_CXX_STANDARD 20)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_compile_options(-Wall -Wextra -pedantic -Wmissing-include-dirs -Wformat=2 -Wunused -Wcast-align)

# this can only be applied to C++ sources
set(CMAKE_CXX_FLAGS "-Wnon-virtual-dtor -Woverloaded-virtual")
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message("No build type specified, defaulting to Debug (no optimizations, include debug symbols), use -DCMAKE_BUILD_TYPE=RelWithDebInfo or =Release to build a release)")
endif()
enable_testing()
add_subdirectory(HydraulicNetLib)
add_executable(HydraulicNetwork main.cpp)
target_link_libraries(HydraulicNetwork HydraulicNetLib Eigen3::Eigen)
if (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
    set(TARGETBITS 32)
else()
    set(TARGETBITS 64)
endif()

message("")
message("Hydraulic configured to run on:  ${CMAKE_SYSTEM_NAME} ${TARGETBITS} bit, ${CMAKE_BUILD_TYPE} Mode")
message("Hydraulic will be built in:      ${CMAKE_BINARY_DIR}")
message("\n")