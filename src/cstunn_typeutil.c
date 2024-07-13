
/* ---------------------------------------------------------------- */

#include "cstunn_typeutil.h"

/* ---------------------------------------------------------------- */

#include "cstunn_def.h"

/* ---------------------------------------------------------------- */

enum cstunn_typecat
cstunn_type_get_cat(
    enum cstunn_type type)
{
#define CSTUNN_TYPE_XLIST_type_get_cat(U0, TYPE, TYPECAT, ...) \
    case CSTUNN_TYPE_ENUMP(TYPE): return CSTUNN_TYPECAT_ENUMP(TYPECAT);

    switch (type){
    CSTUNN_TYPE_XLIST(_, CSTUNN_TYPE_XLIST_type_get_cat)
    default:
        return 0;
    }
}

void
cstunn_value_cast_to(
    union cstunn_value *val,
    enum cstunn_type type)
{
    if (cstunn_type_get_cat(type) == CSTUNN_TYPECAT_VOID
        || cstunn_type_get_cat(type) == CSTUNN_TYPECAT_CONT) {
        val->m_VOID.type = type;
        return;
    }

#define CSTUNN_TYPE_XLIST_value_cast_to(CTX, TYPEI, TYPECAT, TYPE, ...)\
case CSTUNN_TYPE_ENUMP(TYPEI):                                         \
    switch (cstunn_type_get_cat(val->m_VOID.type)) {                   \
    case CSTUNN_TYPECAT_CHAR:                                          \
        CSTUNN_VALUE_GETCAT(val, TYPECAT).val = (TYPE)val->m_CHAR.val; \
        break;                                                       \
    case CSTUNN_TYPECAT_IINT:                                          \
        CSTUNN_VALUE_GETCAT(val, TYPECAT).val = (TYPE)val->m_IINT.val; \
        break;                                                       \
    case CSTUNN_TYPECAT_UINT:                                          \
        CSTUNN_VALUE_GETCAT(val, TYPECAT).val = (TYPE)val->m_UINT.val; \
        break;                                                       \
    case CSTUNN_TYPECAT_REAL:                                          \
        CSTUNN_VALUE_GETCAT(val, TYPECAT).val = (TYPE)val->m_REAL.val; \
        break;                                                       \
    default:                                                         \
        CSTUNN_UNREACHABLE();                                          \
    }                                                                \
    break;

    switch (type) {
    CSTUNN_TYPE_XLIST_NOVOID(_, CSTUNN_TYPE_XLIST_value_cast_to)
    default:
        CSTUNN_UNREACHABLE();
    }

    val->m_VOID.type = type;
}

void
cstunn_value_write(
    void *base,
    size_t off,
    const union cstunn_value *val)
{
#define CSTUNN_TYPE_XLIST_value_write(U0, TYPEI, TYPECAT, TYPE, ...) \
case CSTUNN_TYPE_ENUMP(TYPEI):                                       \
    *(TYPE*)((char*)base + off) =                                  \
    CSTUNN_VALUE_GETCAT(val, TYPECAT).val;                           \
    break;

    switch (val->m_VOID.type) {
    CSTUNN_TYPE_XLIST_NOVOID(_, CSTUNN_TYPE_XLIST_value_write)
    default:
        CSTUNN_UNREACHABLE();
    }
}

int
cstunn_value_fromvp(
    union cstunn_value *dest,
    const void *src,
    enum cstunn_type type)
{
#define CSTUNN_TYPE_XLIST_value_fromvp(U0, TYPEI, TYPECAT, TYPE, ...)   \
case CSTUNN_TYPE_ENUMP(TYPEI):                                          \
    CSTUNN_VALUE_GETCAT(dest, TYPECAT).val = *(TYPE*)src;               \
    CSTUNN_VALUE_GETCAT(dest, TYPECAT).type = CSTUNN_TYPE_ENUMP(TYPEI); \
    break;

    switch (type) {
    CSTUNN_TYPE_XLIST_NOVOID(_, CSTUNN_TYPE_XLIST_value_fromvp)
    default:
        return -1;
    }

    return 0;
}

size_t
cstunn_type_size(
    enum cstunn_type type)
{
#define CSTUNN_TYPE_XLIST_value_size(U0, TYPEI, U2, TYPE, ...) \
case CSTUNN_TYPE_ENUMP(TYPEI):                                 \
    return sizeof(TYPE);

    switch (type) {
    CSTUNN_TYPE_XLIST_NOVOID(_, CSTUNN_TYPE_XLIST_value_size)
    default:
        return 0;
    }
}

/* ---------------------------------------------------------------- */
