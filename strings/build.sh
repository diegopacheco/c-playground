#!/bin/bash

mkdir target | echo "target already exists, ignoring."
gcc main.c -o target/main