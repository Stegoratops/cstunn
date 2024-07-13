#ifndef CSTUNNI_PARSE_H_INCLUDED
#define CSTUNNI_PARSE_H_INCLUDED

#include <stddef.h>
#include <setjmp.h>

#include "cstunn_internal.h"

#if NDEBUG
#define cstunn_parse_store_debug(STATE) ((void)0)
#else
#define cstunn_parse_store_debug(STATE) ((void)( \
    (STATE)->v_debug.file = __FILE__,          \
    (STATE)->v_debug.line = __LINE__,          \
    (STATE)->v_debug.func = __func__))
#endif
#define cstunn_parse_throw_error(STATE, ERR) ( \
    cstunn_parse_store_debug(STATE),     \
    cstunn_parse_throw_error_(STATE, ERR),     \
    CSTUNN_UNREACHABLE())

struct cstunn_parse_state
{
    const char *volatile v_input;
    void *output_buf;

    const struct cstunn_specifier_node
        *specifier_base,
        *specifier_curr,
        *specifier_next;
    
    volatile
    cstunn_error_t v_err;

    size_t
        offset, next_offset;
    
    jmp_buf on_error;

    const struct cstunn_named_const
        *consts;

#if !NDEBUG
    volatile
    struct {
        const char
            *file,
            *func;
        long line;
    } v_debug;
#endif
};

void
cstunn_parse_throw_error_(
    struct cstunn_parse_state *r_state,
    cstunn_error_t err);

#endif /* CSTUNNI_PARSE_H_INCLUDED */
