
# QuantLib
set(QL_BUILD_BENCHMARK OFF)
set(QL_BUILD_TEST_SUITE OFF)
set(QL_BUILD_EXAMPLES OFF)
set(QL_TAGGED_LAYOUT OFF)

# XAD
if(MSVC)
    set(XAD_SIMD_OPTION AVX2)
else()
    set(XAD_SIMD_OPTION SSE)
endif()

add_subdirectory(QuantLib)
add_subdirectory(XAD)