set(V8_LIB_TARGETS "${V8_LIB_TARGETS} v8_math")
add_library(
    v8_math
    camera.cc
    color.cc
    light.cc
    pch_hdr.cc
    )
