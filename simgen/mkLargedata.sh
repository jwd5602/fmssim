#!/bin/bash

# BS, rudimentary way to make a large number of events, since I'm RAM limited. Looking into more sophisticated cuts within pythia

# NOTE THAT THIS WILL DELETE events.root
# Note also that the random number generator for pythia is based on time, so you need to set NEVENTS largeenough that it takes a coupel of seconds, 10^4 is plenty
rm outgen/events.root
NFILES=10
NEVENTS=100000
SQRTS=200
COUNTER=0
MACROSIZE=10

while [ $COUNTER -lt $NFILES ]; do
  root -q -b "GenerateEvents.C(${NEVENTS},${SQRTS},${COUNTER})"
  let COUNTER+=1
done

hadd outgen/events.root outgen/temp*.root
rm outgen/temp*.root

root -q -b "RebuildArgfile.C"

