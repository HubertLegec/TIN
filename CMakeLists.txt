cmake_minimum_required(VERSION 3.2.2)
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
        networkMessage/sources/CategoryManagementMessage.cpp
        networkMessage/sources/NeighboursInfoMessage.cpp
        networkMessage/sources/RingMessage.cpp
        networkMessage/sources/GetMessage.cpp
        networkMessage/sources/ServerInfoMessage.cpp
        networkMessage/sources/NetworkControllerErrorMessage.cpp
        networkMessage/sources/UserManagementMessage.cpp)
set(CLIENT_EVENT_SOURCES clientEvents/sources/CategoryAccessEvent.cpp
        clientEvents/sources/ChooseMenuOptionEvent.cpp
        clientEvents/sources/ConfirmMessageEvent.cpp
        clientEvents/sources/NetworkEvent.cpp
        clientEvents/sources/UserAccountEvent.cpp
        clientEvents/sources/NewMessageEvent.cpp
        clientEvents/sources/PendingUserEvent.cpp)
set(CLIENT_STRATEGY_SOURCES client/strategy/sources/BasicEventStrategy.cpp
        client/strategy/sources/ChooseMenuOptionEventStrategy.cpp
        client/strategy/sources/ConfirmMessageEventStrategy.cpp
        client/strategy/sources/NetworkEventStrategy.cpp
        client/strategy/sources/CategoryAccessEventStrategy.cpp
        client/strategy/sources/UserAccountEventStrategy.cpp
        client/strategy/sources/NewMessageEventStrategy.cpp
        client/strategy/sources/PendingUserEventStrategy.cpp)
set(CLIENT_MODEL_SOURCES client/model/sources/Model.cpp
        client/model/sources/CategoryInfo.cpp
        client/model/sources/ConnectionInfo.cpp
        client/model/sources/PendingUserInfo.cpp)
set(CLIENT_VIEW_SOURCES client/view/sources/View.cpp)
set(CLIENT_CONTROLLER_SOURCES client/controller/sources/Controller.cpp)
set(CLIENT_NETWORK_MODULE_SOURCES networkModule/sources/NetworkController.cpp
        networkModule/sources/MessageWrapper.cpp)

set(CLIENT_SOURCE_FILES client/main.cpp
        ${CLIENT_NETWORK_MESSAGE_SOURCES}
        ${CLIENT_MODEL_SOURCES}
        ${CLIENT_VIEW_SOURCES}
        ${CLIENT_EVENT_SOURCES}
        ${CLIENT_STRATEGY_SOURCES}
        ${CLIENT_CONTROLLER_SOURCES}
        ${CLIENT_NETWORK_MODULE_SOURCES})
add_executable(RING_CLIENT ${CLIENT_SOURCE_FILES})
target_link_libraries(RING_CLIENT ${CMAKE_THREAD_LIBS_INIT})

#----------------------------------------- server ----------------------------------------------------------------------

set(SERVER_SOURCE_FILES server/main.cpp
        server/model/sources/Category.cpp
        server/model/headers/Category.h
        server/model/sources/Model.cpp
        server/model/headers/Model.h
        server/model/sources/User.cpp
        server/model/headers/User.h
        server/controller/sources/Controller.cpp
        server/controller/headers/Controller.h
        server/strategy/sources/BasicEventStrategy.cpp
        server/strategy/headers/BasicEventStrategy.h
        server/strategy/sources/GetMessageStrategy.cpp
        server/strategy/headers/GetMessageStrategy.h
        server/strategy/sources/CategoryManagementStrategy.cpp
        server/strategy/headers/CategoryManagementStrategy.h
        server/model/sources/CategoryMember.cpp
        server/model/headers/CategoryMember.h
        server/strategy/sources/UserManagementStrategy.cpp
        server/strategy/headers/UserManagementStrategy.h
        server/strategy/sources/ErrorMessageStrategy.cpp
        server/strategy/headers/ErrorMessageStrategy.h
        server/Server.cpp
        server/Server.h)

set(SERVER_NETWORK_MESSAGE_SOURCES networkMessage/sources/SimpleMessage.cpp
        networkMessage/sources/CategoryListMessage.cpp
        networkMessage/sources/CategoryManagementMessage.cpp
        networkMessage/sources/NeighboursInfoMessage.cpp
        networkMessage/sources/GetMessage.cpp
        networkMessage/sources/ServerInfoMessage.cpp
        networkMessage/sources/RingMessage.cpp
        networkMessage/sources/UserManagementMessage.cpp
        networkMessage/sources/NetworkControllerErrorMessage.cpp
        networkMessage/headers/UserManagementMessage.h
        networkModule/sources/MessageWrapper.cpp
        networkModule/sources/NetworkController.cpp)

add_executable(RING_SERVER ${SERVER_SOURCE_FILES} ${SERVER_NETWORK_MESSAGE_SOURCES})
target_link_libraries(RING_SERVER ${CMAKE_THREAD_LIBS_INIT})

#------------------------------------ Tests configuration --------------------------------------------------------------

#*************************** client tests **********************************************
add_executable(CLIENT_BASIC_TESTS client_tests/clientBasicTest.cpp
        client/model/sources/Model.cpp
        client/model/sources/CategoryInfo.cpp
        client/model/sources/ConnectionInfo.cpp
        networkMessage/sources/RingMessage.cpp
        networkMessage/sources/SimpleMessage.cpp
        )
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
        networkModule/sources/MessageWrapper.cpp
        networkMessage/sources/SimpleMessage.cpp
        networkMessage/sources/UserManagementMessage.cpp
        networkMessage/sources/GetMessage.cpp
        networkMessage/sources/CategoryManagementMessage.cpp
        networkMessage/sources/RingMessage.cpp
        networkMessage/sources/NeighboursInfoMessage.cpp
        networkMessage/sources/CategoryListMessage.cpp
        networkMessage/sources/ServerInfoMessage.cpp
        networkMessage/sources/NetworkControllerErrorMessage.cpp
        networkModule/sources/NetworkController.cpp)
target_link_libraries(NETWORK_BASIC_TESTS gtest_main)
add_test(NAME networkBasicTest COMMAND networkBasicTest)
