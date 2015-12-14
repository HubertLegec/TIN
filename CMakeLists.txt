cmake_minimum_required(VERSION 3.3)
project(RING)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Download and unpack gtest at configure time
configure_file(gtest-CMakeLists.txt gtest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gtest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gtest-download )

# Add gtest directly to our build
add_subdirectory(${CMAKE_BINARY_DIR}/gtest-src
        ${CMAKE_BINARY_DIR}/gtest-build
        EXCLUDE_FROM_ALL )

# Add the gtest include directory, since gtest
# doesn't add that dependency to its gtest target
target_include_directories(gtest INTERFACE
        "${CMAKE_BINARY_DIR}/gtest-src/include" )


set(CLIENT_SOURCE_FILES client/main.cpp client/model/Model.cpp)
set(SERVER_SOURCE_FILES server/main.cpp server/model/sources/Category.cpp server/model/sources/Model.cpp server/model/sources/User.cpp server/controller/sources/Controller.cpp)
add_executable(RING_CLIENT ${CLIENT_SOURCE_FILES})
add_executable(RING_SERVER ${SERVER_SOURCE_FILES})


# Tests configuration
add_executable(CLIENT_BASIC_TESTS client_tests/clientBasicTest.cpp client/model/Model.cpp)
target_link_libraries(CLIENT_BASIC_TESTS gtest_main)
add_test(NAME clientBasicTest COMMAND clientBasicTest)

add_executable(NETWORK_MESSAGES_TESTS networkMessage_tests/messagesTests.cpp
        networkMessage/sources/SimpleMessage.cpp
        networkMessage/sources/CategoryListMessage.cpp
        networkMessage/sources/GetMessage.cpp
        networkMessage/sources/ServerInfoMessage.cpp
        networkMessage/sources/CategoryManagementMessage.cpp
        networkMessage/sources/NeighboursInfoMessage.cpp
        networkMessage/sources/RingMessage.cpp
        cereal/archives/binary.hpp)
target_link_libraries(NETWORK_MESSAGES_TESTS gtest_main)
add_test(NAME networkMessageBasicTest COMMAND networkMessageBasicTest)

add_executable(MESSAGES_SERIALIZATION_TESTS networkMessage_tests/messagesSerializationTests.cpp
        networkMessage/sources/SimpleMessage.cpp
        networkMessage/sources/CategoryListMessage.cpp
        networkMessage/sources/GetMessage.cpp
        networkMessage/sources/ServerInfoMessage.cpp
        networkMessage/sources/CategoryManagementMessage.cpp
        networkMessage/sources/NeighboursInfoMessage.cpp
        networkMessage/sources/RingMessage.cpp
        cereal/archives/binary.hpp)
target_link_libraries(MESSAGES_SERIALIZATION_TESTS  gtest_main)
add_test(NAME networkMessageSerializationTest COMMAND networkMessageSerializationTest)

add_executable(NETWORK_BASIC_TESTS networkModule_tests/networkBasicTest.cpp)
target_link_libraries(NETWORK_BASIC_TESTS  gtest_main)
add_test(NAME networkBasicTest COMMAND networkBasicTest)
