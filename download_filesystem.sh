#!/bin/bash

FILENAME=alpine-minirootfs-3.7.0-x86_64.tar.gz
URL=http://nl.alpinelinux.org/alpine/v3.7/releases/x86_64/$FILENAME
ROOT_PATH=./root

TRY() {
  if [[ $1 ]]
  then
    # Run the command
    $1

    if [[ $? -ne 0 ]]
    then
      echo Error running: $1
    fi
  fi
}

if ! [[ -d $ROOT_PATH ]]
then
  if ! [[ -f $FILENAME ]]
  then
    echo Downloading alpine linux basic filesystem
    TRY "wget $URL"
  fi

  echo Creating the root dir
  TRY "mkdir $ROOT_PATH"

  echo Decompressing the filesystem into $ROOT_PATH
  TRY "tar -xf $FILENAME --directory=$ROOT_PATH"

  echo Removing the compressed file
  TRY "rm $FILENAME"

  echo Done!
fi
