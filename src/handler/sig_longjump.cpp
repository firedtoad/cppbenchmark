#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <pthread.h>
#include <unwind.h>
#include <cxxabi.h>
#include "pg_siglongjmp.h"
static _Unwind_Reason_Code
pg_sjlj_unwind_stop (int version,
                    _Unwind_Action actions,
                    _Unwind_Exception_Class exc_class,
                    struct _Unwind_Exception *exc_obj,
                    struct _Unwind_Context *context,
                    void *stop_parameter)
{
    pg_sigjmp_buf* env = (pg_sigjmp_buf*)stop_parameter;
    uintptr_t context_sp = (_Unwind_Ptr) _Unwind_GetCFA (context);
    int do_longjmp = (actions & _UA_END_OF_STACK) || (context_sp >= env->sp);
//    fprintf(stderr, "pg_sjlj_unwind_stop; context_sp=%lx envsp=%lx actions=%d do_longjmp=%d\n", context_sp, env->sp, actions, do_longjmp);
    if (do_longjmp)
        siglongjmp(env->jb, 1);
    return _URC_NO_REASON;
}

static void pg_sjlj_ex_cleanup(_Unwind_Reason_Code _c, struct _Unwind_Exception *_e)
{
    abort();
}
static struct _Unwind_Exception pg_sjlj_ex;
void pg_siglongjmp(pg_sigjmp_buf* env)
{
    pg_sjlj_ex.exception_class = 0;
    pg_sjlj_ex.exception_cleanup = &pg_sjlj_ex_cleanup;
    // Triggering a force unwind causes the destruction of C++ local variables to be performed.
    _Unwind_ForcedUnwind(&pg_sjlj_ex, pg_sjlj_unwind_stop, env);
    abort();
}

pg_sigjmp_buf *PG_exception_stack;
__attribute__((no_sanitize("address"))) _Unwind_Reason_Code _Unwind_RaiseException (struct _Unwind_Exception *e)
{
    static auto orig_unwind = reinterpret_cast<decltype(&_Unwind_RaiseException)>(
        dlsym(RTLD_NEXT, "_Unwind_RaiseException"));
    _Unwind_Reason_Code ret = orig_unwind(e);
    // At this point, the exception thrown by C++ does not have a corresponding catch handler.
    // if (!is_main_thread())
    //         return ret;
    __cxxabiv1::__cxa_begin_catch(e);
    __cxxabiv1::__cxa_end_catch();
    pg_siglongjmp(PG_exception_stack);
    abort();
    return ret;
}