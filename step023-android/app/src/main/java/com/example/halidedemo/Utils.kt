package com.example.halidedemo

import android.util.Log

object Utils {

    private const val TAG = "Utils"

    init {
        try {
            System.loadLibrary("utils_jni")
        } catch (e: UnsatisfiedLinkError) {
            Log.w(TAG,"Native library not found, native exposure metric may be unavailable.")
        }
    }


    fun exposure(
        l: ByteArray,
        width: Int,
        height: Int,
        inter: Boolean
    ): FloatArray
    {
        var exp: FloatArray = FloatArray(2)
        try {
            exposure(l, width, height, exp)
        } catch (e: UnsatisfiedLinkError) {
            Log.w(TAG,"Native implementation not found, falling back to Java implementation")
        }
        return exp
    }

    /**
     * Exposure Metric
     * @Ref:
     *
     * @param l luminance
     * @param width The width of the input image.
     * @param height The height of the input image.
     * @return s The sharpness result
     */
    private external fun exposure(
        l: ByteArray,
        width: Int,
        height: Int,
        exp: FloatArray
    )

}


