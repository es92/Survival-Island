#!/bin/bash

function cleanup {
  pkill -f "./my_game"
}
trap cleanup EXIT

cd build

while true; do

  echo '=========================================='
  cmake ..
  echo '=========================================='
  make
  MRES=$?
  echo '=========================================='
  if [[ $MRES -ne 0 ]]; then
    read -rsp $'An error occurred while building. Press any key to try again...\n' -n1 key
    clear
    continue
  fi

  ./my_game &

  read -rsp $'Running. Press any key to stop...\n' -n1 key
  pkill -f "./my_game"
  read -rsp $'Press any key to build and run...\n' -n1 key
  clear
done

