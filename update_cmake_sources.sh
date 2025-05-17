#!/bin/bash

#
# Useful wee script for updating the SOURCES_CPP and SOURCES_H variables declared in CMakeLists.txt by recursively
# listing the contents of "src", so I don't have to do it manually. Not very efficient but what script is?
#

sed -i 's|set(SOURCES_CPP.*|set(SOURCES_CPP)|' CMakeLists.txt
sed -i 's|set(SOURCES_H.*|set(SOURCES_H)|' CMakeLists.txt

find src -type f -name "*.cpp" | sort | xargs -I % sed -i 's|set(SOURCES_CPP|set(SOURCES_CPP %|' CMakeLists.txt
find src -type f -name "*.h" | sort | xargs -I % sed -i 's|set(SOURCES_H|set(SOURCES_H %|' CMakeLists.txt