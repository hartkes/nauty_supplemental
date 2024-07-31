#!/bin/bash

# set the following to the path of your local nauty installation

if [ "$(hostname)" == "zendo" ]
then
  export NAUTY_DIR=../../apps/zendo/nauty
else
  echo "I don't know where nauty is installed on this machine."
  exit 99
fi

make
