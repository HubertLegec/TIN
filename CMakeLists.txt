cmake_minimum_required(VERSION 3.3)
project(RING)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(Threads)


#------------------------------------------- gtest ---------------------------------------------------------------------

# Download and unpack gtest at configure time
configure_file(gtest-CMakeLists.txt gtest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gtest-download)
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gtest-download)

# Add gtest directly to our build
add_subdirectory(${CMAKE_BINARY_DIR}/gtest-src
        ${CMAKE_BINARY_DIR}/gtest-build
        EXCLUDE_FROM_ALL)

# Add the gtest include directory, since gtest
# doesn't add that dependency to its gtest target
target_include_directories(gtest INTERFACE
        "${CMAKE_BINARY_DIR}/gtest-src/include")


#----------------------------------------- client ----------------------------------------------------------------------
set(CLIENT_NETWORK_MESSAGE_SOURCES networkMessage/sources/SimpleMessage.cpp
        networkMessage/sources/CategoryListMessage.cpp
        networkMessage/sources/NeighboursInfoMessage.cpp
        networkMessage/sources/RingMessage.cpp
        networkMessage/sources/ServerInfoMessage.cpp
        networkMessage/sources/UserManagementMessage.cpp
        networkMessage/headers/UserManagementMessage.h)
set(CLIENT_EVENT_SOURCES clientEvents/sources/BasicEvent.cpp
        clientEvents/sources/CategoryAccessEvent.cpp
        clientEvents/sources/ChooseMenuOptionEvent.cpp
        clientEvents/sources/ConfirmMessageEvent.cpp
        clientEvents/sources/NetworkEvent.cpp)
set(CLIENT_STRATEGY_SOURCES client/strategy/sources/BasicEventStrategy.cpp
        client/strategy/sources/ChooseMenuOptionEventStrategy.cpp
        client/strategy/sources/ConfirmMessageEventStrategy.cpp
        client/strategy/sources/NetworkEventStrategy.cpp)
set(CLIENT_MODEL_SOURCES client/model/sources/Model.cpp
        client/model/sources/CategoryInfo.cpp
        client/model/sources/ConnectionInfo.cpp)
set(CLIENT_VIEW_SOURCES client/view/sources/View.cpp)
set(CLIENT_CONTROLLER_SOURCES client/controller/sources/Controller.cpp)

set(CLIENT_SOURCE_FILES client/main.cpp
        ${CLIENT_NETWORK_MESSAGE_SOURCES}
        ${CLIENT_MODEL_SOURCES}
        ${CLIENT_VIEW_SOURCES}
        ${CLIENT_EVENT_SOURCES}
        ${CLIENT_STRATEGY_SOURCES}
        ${CLIENT_CONTROLLER_SOURCES})
add_executable(RING_CLIENT ${CLIENT_SOURCE_FILES})
target_link_libraries(RING_CLIENT ${CMAKE_THREAD_LIBS_INIT})

#----------------------------------------- server ----------------------------------------------------------------------

set(SERVER_SOURCE_FILES server/main.cpp
        server/model/sources/Category.cpp
        server/model/sources/Model.cpp
        server/model/sources/User.cpp
        server/controller/sources/Controller.cpp server/strategy/sources/BasicEventStrategy.cpp server/strategy/headers/BasicEventStrategy.h server/strategy/sources/GetMessageStrategy.cpp server/strategy/headers/GetMessageStrategy.h server/strategy/sources/CategoryManagementStrategy.cpp server/strategy/headers/CategoryManagementStrategy.h server/model/sources/CategoryMember.cpp server/model/headers/CategoryMember.h server/strategy/sources/UserManagementStrategy.cpp server/strategy/headers/UserManagementStrategy.h)
set(SERVER_NETWORK_MESSAGE_SOURCES networkMessage/sources/SimpleMessage.cpp
        networkMessage/sources/CategoryListMessage.cpp
        networkMessage/sources/CategoryManagementMessage.cpp
        networkMessage/sources/NeighboursInfoMessage.cpp
        networkMessage/sources/GetMessage.cpp
        networkMessage/sources/ServerInfoMessage.cpp networkMessage/sources/UserManagementMessage.cpp networkMessage/headers/UserManagementMessage.h)

add_executable(RING_SERVER ${SERVER_SOURCE_FILES} ${SERVER_NETWORK_MESSAGE_SOURCES})


#------------------------------------ Tests configuration --------------------------------------------------------------

#*************************** client tests **********************************************
add_executable(CLIENT_BASIC_TESTS client_tests/clientBasicTest.cpp client/model/sources/Model.cpp)
target_link_libraries(CLIENT_BASIC_TESTS gtest_main)
add_test(NAME clientBasicTest COMMAND clientBasicTest)


#************************** network messages tests *************************************
add_executable(NETWORK_MESSAGES_TESTS networkMessage_tests/messagesTests.cpp
        networkMessage/sources/SimpleMessage.cpp
        networkMessage/sources/CategoryListMessage.cpp
        networkMessage/sources/GetMessage.cpp
        networkMessage/sources/ServerInfoMessage.cpp
        networkMessage/sources/CategoryManagementMessage.cpp
        networkMessage/sources/NeighboursInfoMessage.cpp
        networkMessage/sources/RingMessage.cpp
        networkMessage/sources/UserManagementMessage.cpp
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
        networkMessage/sources/UserManagementMessage.cpp
        cereal/archives/binary.hpp)
target_link_libraries(MESSAGES_SERIALIZATION_TESTS gtest_main)
add_test(NAME networkMessageSerializationTest COMMAND networkMessageSerializationTest)


#*************************** newtwork module tests ******************************************
add_executable(NETWORK_BASIC_TESTS networkModule_tests/networkBasicTest.cpp
        networkModule/sources/NetworkController.cpp)
target_link_libraries(NETWORK_BASIC_TESTS gtest_main)
add_test(NAME networkBasicTest COMMAND networkBasicTest)
