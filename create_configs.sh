#!/bin/bash
rm -rf Debug
rm -rf Release
mkdir Debug
mkdir Release
( cd Debug && cmake -DCMAKE_BUILD_TYPE=Debug -G"CodeBlocks - Unix Makefiles" ../ )
( cd Release && cmake -DCMAKE_BUILD_TYPE=Release -G"CodeBlocks - Unix Makefiles" ../ )
