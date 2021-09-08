
// This file binds the native metric image utility code to the Java class
// which exposes them.

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>

//#include "HalideBuffer.h"
//using namespace Halide::Runtime;
#include "exposure.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "halidedemo_native", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "halidedemo_native", __VA_ARGS__)

#define DEBUG 1

extern "C" int halide_host_cpu_count();
extern "C" int halide_start_clock(void *user_context);
extern "C" int64_t halide_current_time_ns();

void handler(void * /* user_context */, const char *msg) {
    LOGE("%s", msg);
}


#define IMAGE_METHOD(METHOD_NAME) \
  Java_com_example_halidedemo_Utils_##METHOD_NAME  // NOLINT

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT bool JNICALL
IMAGE_METHOD(exposure)(
        JNIEnv* env, jobject clazz, jbyteArray input,
        jint width, jint height,
        jfloatArray exp
        );


#ifdef __cplusplus
}
#endif

JNIEXPORT bool JNICALL
IMAGE_METHOD(exposure)(
        JNIEnv* env, jobject clazz, jbyteArray input,
        jint width, jint height, jfloatArray exp) {

    halide_start_clock(NULL);
    halide_set_error_handler(handler);
    //LOGD("According to Halide, host system has %d cpus\n", halide_host_cpu_count());

    jboolean inputCopy = JNI_FALSE;
    jbyte* const in = env->GetByteArrayElements(input, &inputCopy);
    if (!in) {
        LOGD("image src is null\n");
        return false;
    }

    jboolean outputCopy = JNI_FALSE;
    jfloat* const jexp = env->GetFloatArrayElements(exp, &outputCopy);

    int64_t t1 = halide_current_time_ns();

    // Make these static so that we can reuse device allocations across frames.
    static halide_buffer_t srcBuf = {0};
    static halide_dimension_t srcDim[2];

    float out_over, out_under;
    static halide_buffer_t dstBufOver = {0};
    static halide_buffer_t dstBufUnder = {0};

    srcBuf.host = (uint8_t *)in;
    srcBuf.set_host_dirty();
    srcBuf.dim = srcDim;
    srcBuf.dim[0].min = 0;
    srcBuf.dim[0].extent = width;
    srcBuf.dim[0].stride = 1;
    srcBuf.dim[1].min = 0;
    srcBuf.dim[1].extent = height;
    srcBuf.dim[1].stride = width;
    srcBuf.dimensions = 2;
    srcBuf.type = halide_type_of<uint8_t>();

    dstBufOver.host = (uint8_t*)&out_over;
    dstBufOver.set_host_dirty();
    dstBufOver.dimensions = 0;
    dstBufOver.type = halide_type_of<float>();

    dstBufUnder.host = (uint8_t*)&out_under;
    dstBufUnder.set_host_dirty();
    dstBufUnder.dimensions = 0;
    dstBufUnder.type = halide_type_of<float>();

    exposure(&srcBuf, &dstBufUnder, &dstBufOver);
    jexp[0] = out_under;
    jexp[1] = out_over;

    int64_t t2 = halide_current_time_ns();
    unsigned elapsed_us = (t2 - t1) / 1000;
    LOGD("Time taken: %d", elapsed_us);

    env->ReleaseByteArrayElements(input, in, JNI_ABORT);
    env->ReleaseFloatArrayElements(exp, jexp, 0);

    return true;

}
