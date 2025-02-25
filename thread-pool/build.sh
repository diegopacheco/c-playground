#!/bin/bash

rm -rf target/
mkdir target/
gcc src/*.c -pthread -o target/main
