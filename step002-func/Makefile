BIN = bin
IMAGES ?= ../rec

CXX ?= g++
GXX ?= g++

HALIDE=${HALIDE_PATH}
HALIDE_TOOLS_DIR = -I ${HALIDE_PATH}/tools
HALIDE_LIB_PATH  = -L ${HALIDE_PATH}/lib
HALIDE_INC_PATH  = -I ${HALIDE_PATH}/include
IMAGE_IO_FLAGS   = `libpng-config --cflags --ldflags` -ljpeg

# Example 1

$(BIN)/test_brighter: brighter.cpp
	@mkdir -p $(@D)
	$(CXX) $(filter-out %.h,$^) -g $(HALIDE_INC_PATH) $(HALIDE_TOOLS_DIR) $(HALIDE_LIB_PATH) -lHalide $(IMAGE_IO_FLAGS) -o $@ -std=c++17 -DHL_DEBUG_PROCESS

$(BIN)/brighter: $(BIN)/test_brighter
	@mkdir -p $(@D)
	$(BIN)/test_brighter $(IMAGES)/rgb.png 1.8 $(BIN)/output.png

test_brighter: $(BIN)/brighter

# Example 2

$(BIN)/test_blur: blur.cpp
	@mkdir -p $(@D)
	$(CXX) $(filter-out %.h,$^) -g $(HALIDE_INC_PATH) $(HALIDE_TOOLS_DIR) $(HALIDE_LIB_PATH) -lHalide $(IMAGE_IO_FLAGS) -o $@ -std=c++17 -DHL_DEBUG_PROCESS

$(BIN)/blur: $(BIN)/test_blur
	@mkdir -p $(@D)
	$(BIN)/test_blur $(IMAGES)/rgb.png $(BIN)/output.png

test_blur: $(BIN)/blur

# Example 3

$(BIN)/test_eq: equalization.cpp
	@mkdir -p $(@D)
	$(CXX) $(filter-out %.h,$^) -g $(HALIDE_INC_PATH) $(HALIDE_TOOLS_DIR) $(HALIDE_LIB_PATH) -lHalide $(IMAGE_IO_FLAGS) -o $@ -std=c++17 -DHL_DEBUG_PROCESS

$(BIN)/eq: $(BIN)/test_eq
	@mkdir -p $(@D)
	$(BIN)/test_eq $(IMAGES)/gray.png $(BIN)/output.png

test_eq: $(BIN)/eq

# Example 4

$(BIN)/test_th: thresholding.cpp
	@mkdir -p $(@D)
	$(CXX) $(filter-out %.h,$^) -g $(HALIDE_INC_PATH) $(HALIDE_TOOLS_DIR) $(HALIDE_LIB_PATH) -lHalide $(IMAGE_IO_FLAGS) -o $@ -std=c++17 -DHL_DEBUG_PROCESS

$(BIN)/th: $(BIN)/test_th
	@mkdir -p $(@D)
	$(BIN)/test_th $(IMAGES)/lenna.png $(BIN)/output.png

test_th: $(BIN)/th


clean:
	rm -rf $(BIN)
