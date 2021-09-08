cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

get_filename_component(MY_SRC_DIR  ${CMAKE_SOURCE_DIR}/..  ABSOLUTE)

if (ANDROID_ABI MATCHES "^armeabi-v7a$")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=softfp -mfpu=neon")
elseif(ANDROID_ABI MATCHES "^arm64-v8a")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ftree-vectorize")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
                    -std=c++17 -fno-exceptions -fno-rtti -O2 -Wno-narrowing \
                    -fPIE")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} \
                              -Wl,--allow-multiple-definition \
                              -Wl,--whole-archive -fPIE -v")

# Creates the project's shared lib: libexposure_jni.so.
add_library( utils_jni
             SHARED
             ${MY_SRC_DIR}/jni/utils_jni.cpp )

# Create sharpness static import
add_library(exposure STATIC IMPORTED)
set_target_properties(
        exposure
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_LIST_DIR}/../jnilibs/${ANDROID_ABI}/exposure.a
)

find_library(log-lib
             log )

find_library( m-lib m )
find_library( android-lib android )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        utils_jni
        exposure

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        ${m-lib}
        ${android-lib}
        )

target_include_directories(utils_jni PUBLIC
        $ENV{HALIDE_PATH}/tools
        $ENV{HALIDE_PATH}/include
        ${CMAKE_CURRENT_LIST_DIR}/exposure/include/${ANDROID_ABI}
        )

