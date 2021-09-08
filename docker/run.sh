#!/bin/bash

WS_DIR_PATH=$(realpath "$PWD/..")

docker run -ti \
--gpus=all \
--privileged=true \
--cap-add=CAP_SYS_ADMIN \
--ipc=host \
-v /tmp/.X11-unix:/tmp/.X11-unix \
-v /dev/bus/usb:/dev/bus/usb \
-v /dev/input:/dev/input \
-v /media/pedro/thor/.datasets:/root/.datasets \
-e DISPLAY=$DISPLAY \
-v $WS_DIR_PATH:/workspace/course \
-p 8080:8080 \
--name halide-run halide:latest \
/bin/bash

# Grant docker access to host X server
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
xhost +
fi
