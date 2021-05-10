/*!
 * by including the doctest header with the special define,
 * it generates the main() function for the doctest executable.
 * This is connected through the src/CMakeLists.txt that compile this
 * file and the read_write_test.cpp into one binary executable
 * This main.cpp also allows us separate .cpp files for separate files.
 * It makes sense to give the 'main' its own .cpp file
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

