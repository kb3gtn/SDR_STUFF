#!/bin/bash

if [ -z ./output ]; then
    rm ./output -Rf
fi

mkdir -p ./output
cd ./output
cmake ../
make
cd ./bin

