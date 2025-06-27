file(GLOB_RECURSE DSP_SOURCE_FILES "Source/*.c")
add_library(dsp_lib STATIC
    ${DSP_SOURCE_FILES}
)