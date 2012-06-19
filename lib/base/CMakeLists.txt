set(V8_LIB_TARGETS "${V8_LIB_TARGETS} v8_base")

add_library(
    v8_base
    debug_helpers.cc
    win32_utils.cc
    pch_hdr.cc
    )
