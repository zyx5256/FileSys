#!/usr/bin/env python
import sys
import commands
import os
import util
import time


# tweak this value
# port = "10000"
# host = "localhost"
ctimeout = 5
numtest = 46
host = os.environ.get("MFS_SERVER")
port = os.environ.get("MFS_PORT")

def run_test(testcase):
    commands.getoutput("killall -9 server")
    commands.getoutput("killall -9 testclient")
    if int(testcase) == 1:
        if os.path.exists("README") or os.path.exists("README.txt"):
            return 0 # pass
        print "ERROR: no README or README.txt"
        return -1 # failed

    if int(testcase) == 2:
        if os.path.exists("Makefile") or os.path.exists("makefile"):
            return 0
        print "ERROR: no Makefile or makefile"
        return -1

    if int(testcase) == 3:
        commands.getoutput("rm -rf server")
        commands.getoutput("rm -rf libmfs.so")
        commands.getoutput("make")
        if not os.path.exists("server") or not os.path.exists("libmfs.so"):
            print "ERROR: make does not generate server and libmfs.so"
            return -1
        return 0

    if int(testcase) > 3 and int(testcase) <=39:
        rc = util.make()
        if rc == -1:
            return -1
        rc = util.compile_testclient(testcase)
        if rc == -1:
            return -1

        rc = util.start_fresh_server(port)
        if rc == -1:
            return -1

        status, out = commands.getstatusoutput("./testclient %s %s"
                                               % (port, host))
#        print out
        commands.getoutput("killall -9 server")
        return status


    if int(testcase) == 40: # test a
        rc = util.make()
        if rc == -1:
            return -1;

        # start server with a fresh file
        commands.getoutput("rm -rf fsimage")
        commands.getoutput("rm -rf server-out.txt")
        commands.getoutput("killall -9 server")
        os.system("./server %s fsimage > /dev/null &" % port)
        #time.sleep(1)
        time.sleep(3) # added on 5/3 9am
        rc = util.compile_testclient(str(testcase) + "pre")
        if rc == -1:
            return -1
        status, out = commands.getstatusoutput("./testclient %s %s"
                                               % (port, host))
#        print out
        if status == -1:
            print "can not pass PRE test"
            return -1

        commands.getoutput("killall -9 server")
        time.sleep(1) # added on 5/3 9am

        os.system("./server %s fsimage > /dev/null &" % port)
        time.sleep(0.5)
        rc = util.compile_testclient(str(testcase) + "post")
        if rc == -1:
            return -1
        status, out = commands.getstatusoutput("./testclient %s %s"
                                               % (port, host))
#        print out
        commands.getoutput("killall -9 server")
        return status

    if int(testcase) >= 41:
        rc = util.make()
        if rc == -1:
            return -1
        rc = util.compile_testclient(testcase)
        if rc == -1:
            return -1
        clientout = "out.txt"
        commands.getoutput("rm -rf out.txt")
        os.system("./testclient %s %s > %s &" % (port, host, clientout))
        time.sleep(ctimeout/2)
        rc = util.start_fresh_server(port)
        if rc == -1:
            return -1
        time.sleep(ctimeout)
        ps = commands.getoutput("ps -ax")
        if ps.find("./testclient %s %s" % (port, host)) >= 0:
            # the testclient is still running
            print "ERROR: your client did not implement timeout correctly"
            print "ERROR: testclient should have finished"
            return -1
        else: # now testclient finish, let check the return status
            cat = commands.getoutput("cat %s" % clientout)
            if cat.find("PASSED") >= 0:
                return 0
            return -1




# starting of the script

if not os.path.exists("mfs.h"):
    print "ERROR: missing mfs.h"
    sys.exit(0)
commands.getoutput("rm -rf ./testcases/mfs.h")
commands.getoutput("cp mfs.h ./testcases")

print "runing test with MFS_SERVER = %s, MFS_PORT = %s" % (host, str(port))

if len(sys.argv) == 1:
    for i in range(1, numtest + 1):
        if i not in [7,8,13,19,20,27,33,35,39]:
            rc = run_test(i)
            if rc == 0:
                print "Test " + str(i) + " passed"
            else:
                print "Test " + str(i) + " failed"

    commands.getoutput("killall -9 server")
    commands.getoutput("killall -9 testclient")

elif len(sys.argv) == 2:
    i = int(sys.argv[1])
    rc = run_test(i)
    if rc == 0:
        print "Test " + str(i) + " passed"
    else:
        print "Test " + str(i) + " failed"
    commands.getoutput("killall -9 server")
    commands.getoutput("killall -9 testclient")
else:
    print "Usage run-test.py [test number (1-46) | all]"
    sys.exit(0)
