BIN = bin
HL_TARGET ?= host-profile
GENERATOR_BIN ?= $(BIN)
AUTO_SCHEDULER = false
IMAGES ?= ../rec

CXX ?= g++
GXX ?= g++

HALIDE=${HALIDE_PATH}
HALIDE_TOOLS_DIR           = -I ${HALIDE_PATH}/tools
HALIDE_LIB_PATH            = -L ${HALIDE_PATH}/lib
HALIDE_LINK_PATH           =    ${HALIDE_PATH}/lib/libHalide.a
HALIDE_INC_PATH            = -I ${HALIDE_PATH}/include
HALIDE_SYSTEM_LIBS         =    $(HALIDE_INC_PATH) $(HALIDE_TOOLS_DIR) $(HALIDE_LINK_PATH)
HALIDE_GENERATOR_DEPS      =    ${HALIDE_PATH}/tools/GenGen.cpp
HALIDE_GENERATOR_OUTPUTS  ?=    static_library,h,registration,stmt,assembly,schedule
IMAGE_IO_FLAGS             = `libpng-config --cflags --ldflags` -ljpeg


$(GENERATOR_BIN)/mfcc.generator: mfcc.cpp
	@mkdir -p $(@D)
	$(CXX) $(filter-out %.h,$^) $(HALIDE_GENERATOR_DEPS) -g -std=c++17 -fno-rtti $(HALIDE_SYSTEM_LIBS) -o $@ -ldl -lpthread -lz

$(BIN)/%/mfcc.a: $(GENERATOR_BIN)/mfcc.generator
	@mkdir -p $(@D)
	$^ -g mfcc -e $(HALIDE_GENERATOR_OUTPUTS) -o $(@D) -f mfcc target=$* auto_schedule=$(AUTO_SCHEDULER)

test_mfcc: $(BIN)/$(HL_TARGET)/mfcc.a

clean:
	rm -rf $(BIN) *.mex *.mexa64