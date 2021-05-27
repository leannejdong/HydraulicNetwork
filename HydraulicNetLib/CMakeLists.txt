project(HydraulicNetLib)
add_library(HydraulicNetLib src/inci.cpp src/NetworkSolve.cpp src/newton.cpp)

target_include_directories(HydraulicNetLib PUBLIC include)   # PUBLIC to allow main.cpp to access it

target_link_libraries(HydraulicNetLib PUBLIC    # PUBLIC so my 'users' also link to Eigen
        Eigen3::Eigen
        )

add_executable(basic_tests src/inci_test.cpp src/main.cpp src/NetworkSolve.cpp src/inci.cpp src/newton.cpp)
target_link_libraries(basic_tests HydraulicNetLib)
target_include_directories(basic_tests PRIVATE include)

add_test(NAME basic_tests COMMAND basic_tests WORKING_DIRECTORY src)