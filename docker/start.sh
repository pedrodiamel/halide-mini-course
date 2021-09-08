#!/bin/bash

# Grant docker access to host X server
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
xhost +
fi

docker start halide-run
docker attach halide-run
