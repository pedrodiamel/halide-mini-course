
BIN = bin

CXX ?= g++
GXX ?= g++

HALIDE=${HALIDE_PATH}
HALIDE_TOOLS_DIR = -I ${HALIDE_PATH}/tools
HALIDE_LIB_PATH  = -L ${HALIDE_PATH}/lib
HALIDE_INC_PATH  = -I ${HALIDE_PATH}/include
IMAGE_IO_FLAGS   = `libpng-config --cflags --ldflags` -ljpeg

$(BIN)/test: main.cpp
	@mkdir -p $(@D)
	$(CXX) $(filter-out %.h,$^) -g $(HALIDE_INC_PATH) $(HALIDE_TOOLS_DIR) $(HALIDE_LIB_PATH) -lHalide -o $@ -std=c++17 -DHL_DEBUG_PROCESS

$(BIN)/main: $(BIN)/test
	@mkdir -p $(@D)
	$(BIN)/test 3.0

test: $(BIN)/main

clean:
	rm -rf $(BIN)
