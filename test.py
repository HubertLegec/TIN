#!/usr/bin/env python
import os
import signal
import subprocess
import time


def scrollOutPut(clientProc):
    time.sleep(1)
    while True:
        line = clientProc.stdout.readline()
        # print line.rstrip()
        if line == '>\n':
            break


def run(clientProc, commands):
    licznik = 0
    for command in commands:
        # if flag and licznik < 1:
        scrollOutPut(clientProc)
        # print str(command)
        clientProc.stdin.write(str(command) + "\n")
        licznik += 1


def run1(clientProc, commands):
    licznik = 0
    for command in commands:
        if licznik != 0:
            scrollOutPut(clientProc)
        # print str(command)
        clientProc.stdin.write(str(command) + "\n")
        licznik += 1


def assertContains(clientProc, string):
    time.sleep(1)
    result = False
    while True:
        line = clientProc.stdout.readline()
        # print line.rstrip()
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
    # scrollOutPut(clientProc)
    # clientProc.stdin.write("s" + "\n")
    if assertContainsElements(clientProc, categoryNames):
        print "V user saw correct categories. Categories name: " + str(categoryNames)
        # print categoryNames
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
    # scrollOutPut(clientProc)
    # print str(command)
    if assertContains(clientProc, "name: " + user1):
        print "V user: " + user1 + " was registered by: " + userName
    else:
        print "X test failed. User: " + user1 + " didnt join to category"
    time.sleep(1)
    clientProc.stdin.write("y" + "\n")
    time.sleep(1)
    if assertContains(clientProc, "name: " + user2):
        print "V user: " + user2 + " was not registered by: " + userName
    else:
        print "X test failed. User: " + user2 + " didnt join to category"
    clientProc.stdin.write("n" + "\n")


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


def sendMsgTest(clientProc, categoryId, msg):
    commands = ["m", categoryId, msg]
    run1(clientProc, commands)


def userSignOutCategoryTest(clientProc, userName, categoryID):
    commands = ["o", categoryID, "y"]
    run1(clientProc, commands)
    time.sleep(1)
    expectResult = "successfully"
    if assertContains(clientProc, expectResult):
        print "V User " + userName + " successfully left category " + str(categoryID)
    else:
        print "X User " + userName + " was unable to leave category " + str(categoryID)


def deleteCategoryTest(clientProc, categoryID):
    commands = ["d", categoryID, "y"]
    run1(clientProc, commands)
    time.sleep(1)
    expectResult = "removed!"
    if assertContains(clientProc, expectResult):
        print "V category " + str(categoryID) + " was successfully deleted"
    else:
        print "X category " + str(categoryID) + " wasn't deleted"


def main():
    # subprocess.Popen("cmake CMakeLists.txt")
    # subprocess.Popen("make RING_SERVER")
    # subprocess.Popen("make RING_CLIENT")
    # subprocess.Popen("./RING_SERVER", shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    serverProc = subprocess.Popen("./RING_SERVER", shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                  preexec_fn=os.setsid)

    clientProc1 = subprocess.Popen("./RING_CLIENT -port 5557", shell=True,
                                   stdin=subprocess.PIPE,
                                   stdout=subprocess.PIPE, preexec_fn=os.setsid)

    clientProc2 = subprocess.Popen("./RING_CLIENT -port 6667", shell=True,
                                   stdin=subprocess.PIPE,
                                   stdout=subprocess.PIPE, preexec_fn=os.setsid)
    clientProc3 = subprocess.Popen("./RING_CLIENT -port 4447", shell=True,
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
    checkAcceptInboxTest(clientProc2, "pierscien", "Hubert", "Wojtek", "Wojtek")
    registrationInRingTest(clientProc3, "Damian", 0, "pierscien")
    # confirmRegistration1Test(clientProc1, "Wojtek", "Damian") jeszcze nie przechodi work in progress
    time.sleep(1)
    print "========SENDING MSG IN RING TEST======"
    time.sleep(1)
    # sendMsgTest(clientProc1, 0, ) nad tym pracuje

    print "========USER SIGN OUT FROM CATEGORY TEST======"
    userSignOutCategoryTest(clientProc2, "Hubert", 0);

    print "========DELETING CATEGORY TEST======="
    deleteCategoryTest(clientProc1, 0)
    categories = ["pierscien1"]
    displayCategoryTest(clientProc1, categories)
    displayCategoryTest(clientProc2, categories)

    os.killpg(os.getpgid(serverProc.pid), signal.SIGTERM)
    os.killpg(os.getpgid(clientProc1.pid), signal.SIGTERM)
    os.killpg(os.getpgid(clientProc2.pid), signal.SIGTERM)
    os.killpg(os.getpgid(clientProc3.pid), signal.SIGTERM)


main()
