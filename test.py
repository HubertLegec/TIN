#!/usr/bin/env python
import os
import signal
import time
import subprocess


def scrollOutPut(clientProc):
    time.sleep(1)
    while True:
        line = clientProc.stdout.readline()
        if line == '>\n':
            break


def run(clientProc, commands):
    licznik = 0
    for command in commands:
        # if flag and licznik < 1:
        scrollOutPut(clientProc)
        clientProc.stdin.write(str(command) + "\n")
        licznik += 1


def run1(clientProc, commands):
    licznik = 0
    for command in commands:
        if licznik != 0:
            scrollOutPut(clientProc)
        clientProc.stdin.write(str(command) + "\n")
        licznik += 1


def assertContains(clientProc, string):
    time.sleep(1)
    result = False
    while True:
        line = clientProc.stdout.readline()
        if line.find(string) != -1:
            result = True
        if line == '>\n':
            break
    return result


def assertContainsElements(clientProc, elements):
    time.sleep(1)
    list = []
    while True:
        line = clientProc.stdout.readline()
        list.append(line)
        if line == '>\n':
            break
    for element in elements:
        result = False
        for elLine in list:
            if element in elLine:
                result = True
                break
        if result == False:
            return False
    return True


def displayCategoryTest(clientProc, categoryNames):
    commands = ["s"]
    run1(clientProc, commands)
    if assertContainsElements(clientProc, categoryNames):
        print "V user saw correct categories. Categories name: " + str(categoryNames)
    else:
        print "X test failed"


def creationCategoryTest(clientProc, categoryName):
    commands = ["c", categoryName, "y"]
    run1(clientProc, commands)
    if assertContains(clientProc, "created"):
        print "V category created correct. Category name: " + categoryName
    else:
        print "X test failed"


def creationUserTest(clientProc, userName):
    commands = ["a", userName, "y"]
    run(clientProc, commands)
    if assertContains(clientProc, "created"):
        print "V user created correct. Username: " + userName
    else:
        print "X test failed"


def registrationInRingTest(clientProc, userName, ringId, ringName):
    commands = ["u", ringId, "y"]
    run1(clientProc, commands)
    if assertContains(clientProc, "waits for owner"):
        print "V user: " + userName + " try to register in ring: " + ringName
    else:
        print "X test failed"


def confirmRegistrationTest(clientProc, userName, user1, user2):
    clientProc.stdin.write("p" + "\n")
    if assertContains(clientProc, "name: " + user1):
        print "V user: " + user1 + " was registered by: " + userName
    else:
        print "X test failed. User: " + user1 + " didnt join to category"
    time.sleep(1)
    clientProc.stdin.write("y" + "\n")
    time.sleep(1)
    if assertContains(clientProc, "name: " + user2):
        print "V user: " + user2 + " was registered by: " + userName
    else:
        print "X test failed. User: " + user2 + " didnt join to category"
    clientProc.stdin.write("y" + "\n")
    scrollOutPut(clientProc)
    clientProc.stdin.write("p" + "\n")


def confirmRegistration1Test(clientProc, userName, user1):
    clientProc.stdin.write("p" + "\n")
    time.sleep(1)
    if assertContains(clientProc, "name: " + user1):
        print "V user: " + user1 + " was registered by: " + userName
    else:
        print "X test failed. User: " + user1 + " didnt join to category"
    time.sleep(1)
    clientProc.stdin.write("y" + "\n")


def checkAcceptInboxTest(clientProc, categoryName, userName, leftNeigh, rightNei):
    commands = ["i"]
    run1(clientProc, commands)
    expectResults = [categoryName, "Left neighbour: " + leftNeigh, "Right neighbour: " + rightNei]
    if assertContainsElements(clientProc, expectResults):
        print "V user: " + userName + " was added to category and has correct neighbours: " + str(expectResults)
    else:
        print "X user: " + userName + " wasnt added to category or has wrong neighbours. Expected: " + str(
                expectResults)


def sendMsg(clientProc, categoryId, msg):
    commands = ["m", categoryId]
    run(clientProc, commands)
    time.sleep(1)
    # scrollOutPut(clientProc)
    time.sleep(1)
    clientProc.stdin.write(str(msg) + "\n")
    scrollOutPut(clientProc)


def receiveMsgTest(clientProc, userName, msg):
    commands = ["i"]
    run1(clientProc, commands)
    if assertContains(clientProc, msg):
        print "V user: " + userName + " received msg: " + msg
    else:
        print "X test failed. User: " + userName + " didnt received msg: " + msg
    clientProc.stdin.write("y" + "\n")


def checkMsgReceivedToSender(clientProc, username, users):
    commands = ["i", "i"]
    run1(clientProc, commands)
    if assertContainsElements(clientProc, users):
        print "V owner: " + username + " saw correct information that all users read his msg. users name: " + str(users)
        # print categoryNames
    else:
        print "X test failed"


def main():
    compileCmake = subprocess.Popen("cmake CMakeLists.txt", shell=True)
    compileCmake.wait()
    compileServer = subprocess.Popen("make RING_SERVER", shell=True)
    compileServer.wait()
    compileClient = subprocess.Popen("make RING_CLIENT", shell=True)
    compileClient.wait()
    serverProc = subprocess.Popen("./RING_SERVER", shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                  preexec_fn=os.setsid)

    clientProc1 = subprocess.Popen("./RING_CLIENT -port 5555", shell=True,
                                   stdin=subprocess.PIPE,
                                   stdout=subprocess.PIPE, preexec_fn=os.setsid)

    clientProc2 = subprocess.Popen("./RING_CLIENT -port 6666", shell=True,
                                   stdin=subprocess.PIPE,
                                   stdout=subprocess.PIPE, preexec_fn=os.setsid)
    clientProc3 = subprocess.Popen("./RING_CLIENT -port 4444", shell=True,
                                   stdin=subprocess.PIPE,
                                   stdout=subprocess.PIPE, preexec_fn=os.setsid)
    print "========CREATION USER TEST======"
    creationUserTest(clientProc1, "Wojtek")
    creationUserTest(clientProc2, "Hubert")
    creationUserTest(clientProc3, "Damian")
    time.sleep(1)
    print "========CREATION CATEGORY TEST======"
    creationCategoryTest(clientProc1, "pierscien")
    creationCategoryTest(clientProc1, "pierscien1")
    categories = ["pierscien", "pierscien1"]
    displayCategoryTest(clientProc1, categories)
    time.sleep(1)
    print "========REGISTRATION IN RING TEST======"
    registrationInRingTest(clientProc2, "Hubert", 0, "pierscien")
    registrationInRingTest(clientProc3, "Damian", 0, "pierscien")
    confirmRegistrationTest(clientProc1, "Wojtek", "Hubert", "Damian")
    checkAcceptInboxTest(clientProc2, "pierscien", "Hubert", "Wojtek", "Damian")
    checkAcceptInboxTest(clientProc3, "pierscien", "Damian", "Hubert", "Wojtek")
    time.sleep(1)
    print "========SENDING MSG IN RING TEST======"
    sendMsg(clientProc1, 0, "wiadomosc")
    time.sleep(1)
    receiveMsgTest(clientProc3, "Damian", "wiadomosc")
    time.sleep(2)
    receiveMsgTest(clientProc2, "Hubert", "wiadomosc")
    users = ["Hubert", "Damian"]
    checkMsgReceivedToSender(clientProc1, "Wojtek", users)
    time.sleep(1)
    # sendMsgTest(clientProc1, 0, ) nad tym pracuje

    os.killpg(os.getpgid(serverProc.pid), signal.SIGTERM)
    os.killpg(os.getpgid(clientProc1.pid), signal.SIGTERM)
    os.killpg(os.getpgid(clientProc2.pid), signal.SIGTERM)
    os.killpg(os.getpgid(clientProc3.pid), signal.SIGTERM)


main()

