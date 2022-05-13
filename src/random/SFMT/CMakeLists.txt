set(SFMT_SOURCES
        SFMT.c
        SFMT.h
        SFMT-alti.h
        SFMT-common.h
        SFMT-neon.h
        SFMT-params.h
        SFMT-params607.h
        SFMT-params1279.h
        SFMT-params2281.h
        SFMT-params4253.h
        SFMT-params11213.h
        SFMT-params19937.h
        SFMT-params44497.h
        SFMT-params86243.h
        SFMT-params132049.h
        SFMT-params216091.h
        SFMT-sse2.h
        SFMT-sse2-msc.h)

add_library(sfmt STATIC ${SFMT_SOURCES})

target_compile_options(sfmt PRIVATE -msse2)
target_compile_definitions(sfmt PUBLIC -DHAVE_SSE2 -DSFMT_MEXP=19937)