#!/bin/bash
gcc -o target/main main.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto
./target/main
