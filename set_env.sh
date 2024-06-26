#!/bin/bash

export GAMEBOY=$PWD

RAYLIB=''
Platform='unknown'
CurrentOS=$(uname)
FRAMEWORK=''

if [[ "$CurrentOS" == 'Linux' ]]; then
    Platform='linux'
    RAYLIB=$GAMEBOY/packages/linux
    FRAMEWORK=''
elif [[ "$CurrentOS" == 'Darwin' ]]; then
    Platform='macos'
    RAYLIB=$GAMEBOY/packages/macos
    FRAMEWORK='-lglfw -framework Cocoa -framework IOKit'
fi

export LD_LIBRARY_PATH=$GAMEBOY/packages/$Platform/lib:$LD_LIBRARY_PATH
export RAYLIB=$RAYLIB

export FRAMEWORK=$FRAMEWORK

echo "Environment set succesfully"