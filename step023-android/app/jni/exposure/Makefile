
BIN = bin
HL_TARGET ?= arm-64-android
GENERATOR_BIN ?= $(BIN)
ANDROID_ABI ?= $(BIN)/lib
AUTO_SCHEDULER = false

CXX = g++
GXX = g++

HALIDE             =    ${HALIDE_PATH}
HALIDE_TOOLS_DIR   = -I ${HALIDE_PATH}/tools
HALIDE_LIB_PATH    =    ${HALIDE_PATH}/lib/libHalide.a
HALIDE_INC_PATH    = -I ${HALIDE_PATH}/include
HALIDE_SYSTEM_LIBS =    $(HALIDE_INC_PATH) $(HALIDE_TOOLS_DIR) $(HALIDE_LIB_PATH)

GENERATOR_DEPS     =    ${HALIDE_PATH}/tools/GenGen.cpp
GENERATOR_OUTPUTS ?=    static_library,h,registration,stmt,assembly,schedule

INCLUDE            = .
SOURCE             = .


# exposure
$(GENERATOR_BIN)/exposure.generator: exposure.cpp
	@mkdir -p $(@D)
	$(CXX) $(filter-out %.h,$^) $(GENERATOR_DEPS) -g -std=c++17 -fno-rtti $(HALIDE_SYSTEM_LIBS) -o $@ -ldl -lpthread -lz

build.exposure.android: $(GENERATOR_BIN)/exposure.generator
	@mkdir -p $(ANDROID_ABI)
	$^ -g exposure -e static_library,h -o $(ANDROID_ABI) -f exposure target=$(HL_TARGET) auto_schedule=$(AUTO_SCHEDULER)

clean:
	rm -rf $(BIN)
