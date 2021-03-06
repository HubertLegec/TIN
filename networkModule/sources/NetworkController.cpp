//
// Created by hubert on 03.12.15.
//

#include <unistd.h>
#include "../headers/NetworkController.h"
#include "../../logger/easylogging++.h"
#include "../../networkMessage/headers/GetMessage.h"
#include "../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../networkMessage/headers/RingMessage.h"
#include "../../networkMessage/headers/NeighboursInfoMessage.h"
#include "../../networkMessage/headers/ServerInfoMessage.h"
#include "../../networkMessage/headers/UserManagementMessage.h"
#include "../../networkMessage/headers/CategoryListMessage.h"


void NetworkController::prepareSendThread() {
    LOG(INFO) << "[SND] Start sending thread";
    createThread(sendSystemThread, startSendThread);
}

void NetworkController::createThread(pthread_t *thread, void *(*function)(void *)) {
    pthread_create(thread, NULL, function, ((void *) pointer));
}

void *NetworkController::startSendThread(void *param) {
    NetworkController *obj = (NetworkController *) param;
    obj->createSendThread();
}


void NetworkController::createSendThread() {
    LOG(INFO) << "[SND] Starting process of sending messages";
    while (true) {
        std::shared_ptr<MessageWrapper> msg = sendQueue->pop();
        LOG(INFO) << "[SND] I get msg to send: " << msg->getMessage()->toString();
        if (msg->getMessage()->getMessageType() == MessageType::EXIT) {
            LOG(INFO) << "[SND] I get exit message. Closing sending thread";
            SimpleMessage *exitMessage = new SimpleMessage(MessageType::EXIT, -1);
            std::shared_ptr<SimpleMessage> exitMsg = std::shared_ptr<SimpleMessage>(exitMessage);
            receiveQueue->push(exitMsg);
            break;
        }
        if (!prepareConncetionWithReceiver(msg))
            continue;
        if (!sendMsg(msg->getMessage())) {
            LOG(ERROR) << "[SND] Couldnt send msg";
            continue;
        }
    }
    pthread_exit(NULL);
}

bool NetworkController::prepareConncetionWithReceiver(std::shared_ptr<MessageWrapper> msg) {
    LOG(INFO) << "[SND] Creating connection with receiver. Hostname: " << msg->getIP() << " Port: " << msg->getPort();

    hostent *hp;
    struct in_addr ipv4addr;
    struct in6_addr ipv6addr;
    sockaddr_in receiver, client;

    inet_pton(AF_INET, msg->getIP().c_str(), &ipv4addr);
    hp = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
    LOG(INFO) << "[SND] Receiver running at host NAME: " << hp->h_name;
    bcopy(hp->h_addr, &(receiver.sin_addr), hp->h_length);
    LOG(INFO) << "[SND] Receiver INET ADDRESS is: " << inet_ntoa(receiver.sin_addr);

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(msg->getPort());

    sendSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sendSockfd == -1) {
        LOG(ERROR) << "[SND] Unable to create connection with receiver";
        receiveQueue->push(prepareErrorMsg(NetworkControllerErrorMessage::ErrorCode::UNABLE_TO_SEND_MSG,
                                           "I couldn't create connection with receiver msg."));
        return false;
    }
    if (bind(sendSockfd, (struct sockaddr *) &client, sizeof(client)) == -1) {
//        LOG(ERROR) << "[SND] Unable to bind connection with receiver";
//        receiveQueue->push(prepareErrorMsg(NetworkControllerErrorMessage::ErrorCode::UNABLE_TO_SEND_MSG,
//                                           "I couldn't create connection with receiver msg."));
//        return false;
    }

    if (connect(sendSockfd, (struct sockaddr *) &receiver, sizeof(receiver)) == -1) {
        LOG(ERROR) << "[SND] Unable to create connection with receiver";
        receiveQueue->push(prepareErrorMsg(NetworkControllerErrorMessage::ErrorCode::UNABLE_TO_SEND_MSG,
                                           "I couldn't create connection with receiver msg."));
        return false;
    }
    sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    getpeername(sendSockfd, (struct sockaddr *) &from, &fromlen);
    LOG(INFO) << "[SND] Connected to TCPServer1: " << inet_ntoa(from.sin_addr) << ":" << ntohs(from.sin_port);
    hp = gethostbyaddr((char *) &from.sin_addr.s_addr, sizeof(from.sin_addr.s_addr), AF_INET);
    LOG(INFO) << "[SND] Name is : " << hp->h_name;
    return true;

}

bool NetworkController::sendMsg(std::shared_ptr<SimpleMessage> msg) {
    unsigned long length = 0;
    const char *serializedMsg = serializeMsg(msg, length);
    if (serializedMsg == NULL)
        return false;
    int sentBytes = 0;
    LOG(INFO) << "[SND] Sending msg: " << serializedMsg << " with length: " << length << std::endl;
    LOG(INFO) << "[SND] msg: " << msg->toString();
    sentBytes = send(sendSockfd, serializedMsg, length, 0);
    LOG(INFO) << "[SND] Sent: " << sentBytes << " bytes";
    int trialCounter = 1;
    if (sentBytes == length) {
        while (trialCounter < 4 && close(sendSockfd) != 0) {
            LOG(INFO) << "[SND] Something went wrong during closing connection. Retrying..." << trialCounter++;
        }
    }
    sendSockfd = -1;
    if (trialCounter > 4) {
        LOG(ERROR) << "[SND] Something went wrong during closing connection.";
        return false;
    }
    return true;
}

const char *NetworkController::serializeMsg(std::shared_ptr<SimpleMessage> msg, unsigned long &length) {
    std::stringstream ss;
    cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
    switch (msg->getMessageType()) {
        case MessageType::GET : {
            GetMessage *getMessage = dynamic_cast<GetMessage *>(&*msg);
            oarchive(*getMessage); // Write the data to the archive
        }
            break;
        case MessageType::CATEGORY_LIST : {
            CategoryListMessage *categoryListMessage = dynamic_cast<CategoryListMessage *>(&*msg);
            oarchive(*categoryListMessage); // Write the data to the archive
        }
            break;
        case MessageType::CREATE_CATEGORY :
        case MessageType::DESTROY_CATEGORY :
        case MessageType::JOIN_CATEGORY :
        case MessageType::LEFT_CATEGORY :
        case MessageType::ACTIVATE_CATEGORY :
        case MessageType::DEACTIVATE_CATEGORY :
        case MessageType::NEW_MEMBER_CONFIRM:
        case MessageType::NEW_MEMBER_REJECT: {
            CategoryManagementMessage *categoryMessage = dynamic_cast<CategoryManagementMessage *>(&*msg);
            oarchive(*categoryMessage);
        }
            break;
        case MessageType::RING_MESSAGE: {
            RingMessage *ringMessage = dynamic_cast<RingMessage *>( &*msg);
            oarchive(*ringMessage);
        }
            break;
        case MessageType::NEIGHBOURS_SET: {
            NeighboursInfoMessage *nghbrsMessage = dynamic_cast<NeighboursInfoMessage *>(&*msg);
            oarchive(*nghbrsMessage);
        }
            break;
        case MessageType::SERVER_INFO: {
            ServerInfoMessage *serverInfoMessage = dynamic_cast<ServerInfoMessage *>(&*msg);
            oarchive(*serverInfoMessage);
        }
            break;
        case MessageType::CREATE_USER_ACCOUNT:
        case MessageType::DELETE_USER_ACCOUNT:
        case MessageType::CLIENT_CLOSE_APP: {
            UserManagementMessage *userMenagMessage = dynamic_cast<UserManagementMessage *>(&*msg);
            oarchive(*userMenagMessage);
        }
            break;
        default: {
            LOG(ERROR) << "[SND] Wrong type of MessageType: " << msg->getMessageType();
            receiveQueue->push(prepareErrorMsg(NetworkControllerErrorMessage::ErrorCode::UNABLE_TO_SERIALIZE_MSG,
                                               "I get msg with unknown type to send"));
            return NULL;
        }
    }
    length = (ss.str().length());
    return getcharFromString(ss.str());

}

const char *NetworkController::getcharFromString(std::string string) {
    char *result = new char[string.length()];
    for (int i = 0; i < string.length(); ++i) {
        result[i] = '\0';
    }
    for (int i = 0; i < string.length(); ++i) {
        if (((int) string[i]) != 0) {
            result[i] = string[i];
        }
    }
    return result;
}

void NetworkController::prepareReceiveThread() {
    LOG(INFO) << "[REC] Starting preparing receive thread";
    if (!prepareListeningSocket()) {
        stop();
        pthread_exit(NULL);
    }
    LOG(INFO) << "[REC] Listen  with sockfd: " << receiveSockfd;
    listen(receiveSockfd, 100);
    pthread_t thread;
    receiveSystemThread = &thread;
    createThread(receiveSystemThread, startReceiveThread);
}

bool NetworkController::prepareListeningSocket() {
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    LOG(INFO) << "[REC] Getting my own address";
    struct sockaddr_in server;
    hostent *hp;
    struct in_addr ipv4addr;
    inet_pton(AF_INET, myIP, &ipv4addr);
    hp = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
    if (hp == NULL) {
        LOG(ERROR) << "[REC] Problem with creating listening socket. Closing application...";
        receiveQueue->push(prepareErrorMsg(NetworkControllerErrorMessage::ErrorCode::UNABLE_TO_CREATE_LISTENING_SOCKET,
                                           "I couldn't create listening socket for receive thread. Probably your port is already used. Closing application..."));
        return false;
    }
    bcopy(hp->h_addr, &(server.sin_addr), hp->h_length);
    LOG(INFO) << "[REC] TCP/Server INET ADDRESS is: " << inet_ntoa(server.sin_addr);

    LOG(INFO) << "[REC] Trying to get listening socket";
    if ((receiveSockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        LOG(ERROR) << "[REC] Problem with creating listening socket. Closing application...";
        receiveQueue->push(prepareErrorMsg(NetworkControllerErrorMessage::ErrorCode::UNABLE_TO_CREATE_LISTENING_SOCKET,
                                           "I couldn't create listening socket for receive thread. Probably your port is already used. Closing application..."));
        return false;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(myPort));
    int length = sizeof(server);
    if (bind(receiveSockfd, (struct sockaddr *) &server, length) == -1) {
        close(receiveSockfd);
        receiveSockfd = -1;
        LOG(ERROR) << "[REC] Not bind. Probably your port is already used.";
        receiveQueue->push(prepareErrorMsg(NetworkControllerErrorMessage::ErrorCode::UNABLE_TO_CREATE_LISTENING_SOCKET,
                                           "I couldn't create listening socket for receive thread. Probably your port is already used. Closing application..."));
        return false;
    }
    socklen_t length1 = sizeof(server);
    getsockname(receiveSockfd, (struct sockaddr *) &server, &length1);
    LOG(INFO) << "[REC] Server Port is: " << ntohs(server.sin_port);
    return true;

}

void *NetworkController::startReceiveThread(void *param) {
    LOG(INFO) << "[REC] Start receive thread";
    NetworkController *obj = (NetworkController *) param;
    obj->createReceiveThread();

}

void NetworkController::createReceiveThread() {
    LOG(INFO) << "[REC] Receiving connection with sockfd: " << receiveSockfd;
    while (true) {

        struct sockaddr_in peer_name;
        socklen_t socklen = sizeof(peer_name);
        std::unique_lock<std::mutex> mlock(mutex_);
        if (exitFlag) {
            LOG(INFO) << "[REC] Closing receiving thread";
            mlock.unlock();
            break;
        }
        mlock.unlock();
        int senderSockfd = accept(receiveSockfd, (struct sockaddr *) &peer_name, &socklen);
        if (senderSockfd == -1) {
            LOG(INFO) << "[REC] Error during accept connection. I skip this connection... ";
            continue;
        }
        std::unique_lock<std::mutex> mlock1(mutex_);
        if (exitFlag) {
            LOG(INFO) << "[REC] Closing receiving thread";
            mlock1.unlock();
            break;
        }
        mlock1.unlock();
        LOG(INFO) << "[REC] Aceepted connection from sockfd: " << senderSockfd;
        LOG(INFO) << "[REC] I get correct msg. Processing...";
        receiveMsg(senderSockfd, peer_name);
    }
    pthread_exit(NULL);
}


void NetworkController::receiveMsg(int senderSockfd, struct sockaddr_in from) {
    hostent *hp;
    hp = gethostbyaddr((char *) &from.sin_addr.s_addr, sizeof(from.sin_addr.s_addr), AF_INET);
    LOG(INFO) << "[REC] Name is : " << hp->h_name;
    char *buffer = new char[1000];
    int msgLength = 0;
    unsigned long i = 0;
    while ((msgLength = recv(senderSockfd, buffer + i, sizeof(buffer), 0)) > 0) {
        i += msgLength;
    }
    LOG(INFO) << "[REC] I get: " << i << " bytes";
    LOG(INFO) << "[REC] Msg before transform: " << buffer;
    close(senderSockfd);
    senderSockfd = -1;
    SimpleMessage tempMsg;
    LOG(INFO) << "Start reserialize...";
    std::stringstream tempString(getStringFromChar(i, buffer));
    std::stringstream ss(getStringFromChar(i, buffer));
    cereal::BinaryInputArchive iTempArchive(tempString); // Create an input archive
    iTempArchive(tempMsg);
    cereal::BinaryInputArchive iarchive(ss); // Create an input archive

    switch (tempMsg.getMessageType()) {
        case MessageType::GET : {
            GetMessage *getMessage = new GetMessage();
            iarchive(*getMessage); // Write the data to the archive
            std::shared_ptr<GetMessage> msg(getMessage);
            LOG(INFO) << "[REC] MSG: " << getMessage->toString();
            receiveQueue->push(msg);
        }
            break;
        case MessageType::CATEGORY_LIST : {
            CategoryListMessage *categoryListMessage = new CategoryListMessage();
            iarchive(*categoryListMessage); // Write the data to the archive
            std::shared_ptr<CategoryListMessage> msg(categoryListMessage);
            LOG(INFO) << "[REC] MSG: " << categoryListMessage->toString();
            receiveQueue->push(msg);
        }
            break;
        case MessageType::CREATE_CATEGORY :
        case MessageType::DESTROY_CATEGORY :
        case MessageType::JOIN_CATEGORY :
        case MessageType::LEFT_CATEGORY :
        case MessageType::ACTIVATE_CATEGORY :
        case MessageType::DEACTIVATE_CATEGORY :
        case MessageType::NEW_MEMBER_CONFIRM:
        case MessageType::NEW_MEMBER_REJECT: {
            CategoryManagementMessage *categoryMessage = new CategoryManagementMessage();
            iarchive(*categoryMessage);
            std::shared_ptr<CategoryManagementMessage> msg(categoryMessage);
            LOG(INFO) << "[REC] MSG: " << categoryMessage->toString();
            receiveQueue->push(msg);
        }
            break;
        case MessageType::RING_MESSAGE: {
            RingMessage *ringMessage = new RingMessage();
            iarchive(*ringMessage);
            std::shared_ptr<RingMessage> msg(ringMessage);
            LOG(INFO) << "[REC] MSG: " << ringMessage->toString();
            receiveQueue->push(msg);
        }
            break;
        case MessageType::NEIGHBOURS_SET: {
            NeighboursInfoMessage *nghbrsMessage = new NeighboursInfoMessage();
            iarchive(*nghbrsMessage);
            std::shared_ptr<NeighboursInfoMessage> msg(nghbrsMessage);
            LOG(INFO) << "[REC] MSG: " << nghbrsMessage->toString();
            receiveQueue->push(msg);
        }
            break;
        case MessageType::SERVER_INFO: {
            ServerInfoMessage *serverInfoMessage = new ServerInfoMessage();
            iarchive(*serverInfoMessage);
            std::shared_ptr<ServerInfoMessage> msg(serverInfoMessage);
            LOG(INFO) << "[REC] MSG: " << serverInfoMessage->toString();
            receiveQueue->push(msg);
        }
            break;
        case MessageType::CREATE_USER_ACCOUNT:
        case MessageType::CLIENT_CLOSE_APP:
        case MessageType::DELETE_USER_ACCOUNT: {
            UserManagementMessage *userMenagMessage = new UserManagementMessage();
            iarchive(*userMenagMessage);
            std::shared_ptr<UserManagementMessage> msg(userMenagMessage);
            LOG(INFO) << "[REC] MSG: " << userMenagMessage->toString();
            receiveQueue->push(msg);
        }
            break;
        default: {
            LOG(ERROR) << "[REC] Wrong type of MessageType: " << tempMsg.getMessageType();
            receiveQueue->push(prepareErrorMsg(NetworkControllerErrorMessage::ErrorCode::UNABLE_TO_RESERIALIZE_MSG,
                                               "I get msg with unknown type"));
        }
            break;
    }
}

std::string NetworkController::getStringFromChar(unsigned long length1, const char *tab) {
    if (length1 < 16)
        length1 = ((length1 / 16) + 1) * 16;
    std::string result;
    result.
            resize(length1);
    for (
            int i = 0;
            i < length1;
            ++i) {
        result[i] = tab[i];
    }
    return
            result;

}


void NetworkController::stop() {
    std::unique_lock<std::mutex> mlock(mutex_);
    exitFlag = true;
    mlock.unlock();
    SimpleMessage *exitMessage = new SimpleMessage(MessageType::EXIT, -1);
    std::shared_ptr<SimpleMessage> exitMsg = std::shared_ptr<SimpleMessage>(exitMessage);
    MessageWrapper *wrapper = new MessageWrapper(exitMsg, "", -1);
    std::shared_ptr<MessageWrapper> p1(wrapper);
    sendQueue->push(p1);
}

std::shared_ptr<SimpleMessage> NetworkController::prepareErrorMsg(NetworkControllerErrorMessage::ErrorCode errorCode,
                                                                  std::string info) {
    NetworkControllerErrorMessage *errorMsg = new NetworkControllerErrorMessage();
    errorMsg->setErrorCode(errorCode);
    errorMsg->setInfo(info);
    return std::shared_ptr<NetworkControllerErrorMessage>(errorMsg);
}
