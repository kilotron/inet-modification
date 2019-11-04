#!/bin/sh
screen -d -m -S massive opp_run -r 0,7 -m -u Cmdenv -c Move -n ../../src:..:../../tutorials:../../showcases --image-path=../../images -l ../../src/INET --record-eventlog=false --scalar-recording=false --vector-recording=false omnetpp.ini
screen -d -m -S massive2 opp_run -r 1,6 -m -u Cmdenv -c Move -n ../../src:..:../../tutorials:../../showcases --image-path=../../images -l ../../src/INET --record-eventlog=false --scalar-recording=false --vector-recording=false omnetpp.ini
screen -d -m -S massive3 opp_run -r 2,5 -m -u Cmdenv -c Move -n ../../src:..:../../tutorials:../../showcases --image-path=../../images -l ../../src/INET --record-eventlog=false --scalar-recording=false --vector-recording=false omnetpp.ini
screen -d -m -S massive4 opp_run -r 3,4 -m -u Cmdenv -c Move -n ../../src:..:../../tutorials:../../showcases --image-path=../../images -l ../../src/INET --record-eventlog=false --scalar-recording=false --vector-recording=false omnetpp.ini
