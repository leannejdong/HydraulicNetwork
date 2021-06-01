project(HydraulicNetLib)
add_library(HydraulicNetLib src/NetworkSolve.cpp )

target_include_directories(HydraulicNetLib PUBLIC include)   # PUBLIC to allow main.cpp to access it

target_link_libraries(HydraulicNetLib PUBLIC    # PUBLIC so my 'users' also link to Eigen
        Eigen3::Eigen
        )

add_executable(basic_tests src/main.cpp src/inci.cpp src/inci_test.cpp src/NetworkSolve.cpp)
target_link_libraries(basic_tests HydraulicNetLib)
target_include_directories(basic_tests PRIVATE include)

add_test(NAME basic_tests COMMAND basic_tests WORKING_DIRECTORY src)