cmake_minimum_required(VERSION 3.3)
project(RING)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

##################################
# Download and install GoogleTest

include(ExternalProject)
ExternalProject_Add(gtest
        URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
        # Comment above line, and uncomment line below to use subversion.
        # SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
        # Uncomment line below to freeze a revision (here the one for 1.7.0)
        # SVN_REVISION -r700

        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
        INSTALL_COMMAND ""
        )
ExternalProject_Get_Property(gtest source_dir binary_dir)

################
# Define a test
add_executable(client_basic_test client_tests/clientBasicTest.cpp)

######################################
# Configure the test to use GoogleTest
#
# If used often, could be made a macro.

add_dependencies(client_basic_test gtest)
include_directories(${source_dir}/include)
target_link_libraries(client_basic_test ${binary_dir}/libgtest.a)
target_link_libraries(client_basic_test ${binary_dir}/libgtest_main.a)

##################################
# Just make the test runnable with
#   $ make test

enable_testing()
add_test(NAME    client_basic_test
        COMMAND client_basic_test)


set(CLIENT_SOURCE_FILES client/main.cpp client/model/Model.cpp)
set(SERVER_SOURCE_FILES server/main.cpp)
add_executable(RING_CLIENT ${CLIENT_SOURCE_FILES})
add_executable(RING_SERVER ${SERVER_SOURCE_FILES})