#!/bin/bash

set -o xtrace
export runConfigure=0;
export runBuild=0;
export runClean=0;
export runFull=0;

if [ -n "$BUILDER_FORCE_CONFIGURE" ]; then
  export runConfigure=1;
fi
if [ -n "$BUILDER_FORCE_BUILD" ]; then
  export runBuild=1;
fi
if [ -n "$BUILDER_FORCE_CLEAN" ]; then
  export runClean=1;
fi
if [ -n "$BUILDER_FORCE_FULL_BUILD" ]; then
  export runFull=1;
fi

if [ ! -f /rathena/login-server ]; then
  export runFull=1;
elif [ ! -f /rathena/char-server ]; then
  export runFull=1;
elif [ ! -f /rathena/map-server ]; then
  export runFull=1;
fi

if (( $runFull == 1 )); then
  export runConfigure=1;
  export runBuild=1;
  export runClean=1;
fi

if [[ "${runBuild}" -eq "1" ]]; then
  ### checking that ./configure has ran by looking for make file
  if [ ! -f /rathena/make ] && [ "${runConfigure}" -eq "1" ]; then
    echo "Warning: ./configure will be executed with provided values";
    echo "Make sure you have set the variables you want in the docker-compose.yml file";
    echo $BUILDER_CONFIGURE
    ./configure $BUILDER_CONFIGURE
  fi

  if [ "$runClean" -eq "1" ]; then
    make clean
  fi
  make -j8 server;
fi