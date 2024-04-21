
export GAMEBOY=$PWD

RAYLIB=''
Platform='unknown'
CurrentOS=$(uname)

if [[ "$CurrentOS" == 'Linux' ]]; then
    Platform='linux'
    RAYLIB=$GAMEBOY/packages/linux
elif [[ "$CurrentOS" == 'Darwin' ]]; then
    Platform='macos'
    RAYLIB=$GAMEBOY/packages/macos
fi

export LD_LIBRARY_PATH=$GAMEBOY/packages/$Platform/lib:$LD_LIBRARY_PATH
export RAYLIB=$RAYLIB

echo "Environment set succesfully"