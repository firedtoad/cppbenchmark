
#ifndef HANDLER_PG_SIGLONGJMP_H_
#define HANDLER_PG_SIGLONGJMP_H_
#include <setjmp.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#if defined(__aarch64__) || defined(__aarch64)
    static __attribute__((always_inline)) uintptr_t holo_get_sp() {
        uintptr_t rsp;
        __asm__("mov %0, sp" : "=r" (sp));
        return rsp;
    }
#elif defined(__x86_64__)
static __attribute__((always_inline)) uintptr_t holo_get_sp() {
    uintptr_t rsp;
    __asm__("movq %%rsp, %0" : "=r" (rsp));
    return rsp;
}
#else
#error "unsupport arch"
#endif


    typedef struct pg_sigjmp_buf {
        sigjmp_buf jb;
        uintptr_t sp;
    } pg_sigjmp_buf;
// pg_sigsetjmp must be a macro, even a pg_attribute_always_inline can cause pg_siglongjmp to jump to the wrong position.
#define pg_sigsetjmp(out, savemask) ({  \
        pg_sigjmp_buf* _out = out;      \
        int _savemask = savemask;       \
        _out->sp = holo_get_sp();       \
        sigsetjmp(_out->jb, _savemask); })
    extern void pg_siglongjmp(pg_sigjmp_buf* env);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HANDLER_PG_SIGLONGJMP_H_
