#!/bin/sh

set -xe

clang -g main.c -o vulkan -lglfw -lvulkan -DNDEBUG=1
