#!/usr/bin/env python

import commands
import os
import time

testdir = "./testcases"

# --- make everything
def make():
    """make sure a make file exists, and make generate server."""
    if not (os.path.exists("Makefile") or os.path.exists("makefile")):
        print "ERROR: missing Makefile or makefile"
        return -1

    commands.getoutput("make clean")
    commands.getoutput("rm -rf server")
    commands.getoutput("rm -rf libmfs.so")

    commands.getoutput("make")
    if not os.path.exists("server"):
        print "ERROR: make does not generate an excutable server file"
        return -1
    if not os.path.exists("libmfs.so"):
        print "ERROR: make does not generate libmfs.so"
        return -1
    return 0

# --- make the testclient 
def compile_testclient(testcase):
    commands.getoutput("rm -rf testclient")

    commands.getoutput("gcc -Wall -c udp.c -o udp.o")
    commands.getoutput("gcc -Wall -c %s/test%s.c -o testclient.o"
                       %(testdir,testcase))
    commands.getoutput("gcc -o testclient testclient.o udp.o -L. -lmfs")
#    commands.getoutput(" gcc %s/test%s.c -L. -lmfs -o testclient"
#                       % (testdir, testcase))
    if not os.path.exists("testclient"):
        print "ERROR: testclient cannot be compiled"
        return -1
    return 0

# --- start a fress server
def start_fresh_server(port):
    """ start fresh server with /tmp/fsimage."""
    commands.getoutput("killall -9 server")
    fsimage = commands.getoutput("mktemp /tmp/p6.fsimage.XXX")
    commands.getoutput("rm -rf " + fsimage)
    commands.getoutput("rm -rf server-out.txt")
    cmd = "./server " + port + " " + fsimage
    os.system(cmd + " > server-out.txt &")
    time.sleep(1)
    ps = commands.getoutput("ps -ax")
    if ps.find(cmd) == -1:
        print "ERROR: Server can not start when runing the command: " + cmd
        print "see server-out.txt to check your server output"
        return -1
    return 0

