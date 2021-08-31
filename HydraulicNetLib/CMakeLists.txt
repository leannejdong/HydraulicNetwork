project(HydraulicNetLib)
add_library(HydraulicNetLib src/NetworkSolve.cpp )

target_include_directories(HydraulicNetLib PUBLIC include)   # PUBLIC to allow main.cpp to access it

target_link_libraries(HydraulicNetLib PUBLIC    # PUBLIC so my 'users' also link to Eigen
        Eigen3::Eigen
        )

add_executable(basic_tests_hydro src/main.cpp src/inci.cpp src/inci_test.cpp src/NetworkSolve.cpp include/graph.h include/Timer.h)
target_link_libraries(basic_tests_hydro HydraulicNetLib)
target_include_directories(basic_tests_hydro PRIVATE include)

add_test(NAME basic_tests COMMAND basic_tests_hydro WORKING_DIRECTORY src)