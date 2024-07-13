#ifndef CSTUNN_H_INCLUDED
#define CSTUNN_H_INCLUDED

#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define CSTUNN_IINT_VAL_TYPE intmax_t
#define CSTUNN_UINT_VAL_TYPE uintmax_t
#define CSTUNN_REAL_VAL_TYPE long double

#define CSTUNN_TYPE_XLIST(CTX, X)    \
    X(CTX, VOID, VOID, void, 0, 0) \
                                   \
    X(CTX, ARRAY, CONT, , 0, 0)    \
    X(CTX, STRUCT, CONT, , 0, 0)   \
                                   \
    CSTUNN_TYPE_XLIST_NOVOID(CTX, X)

#define CSTUNN_TYPE_XLIST_NOVOID(CTX, X)                                  \
    X(CTX, CHAR, CHAR, char, CHAR_MIN, CHAR_MAX)                        \
                                                                        \
    X(CTX, ICHAR, IINT, signed char, SCHAR_MIN, SCHAR_MAX)              \
    X(CTX, ISHORT, IINT, short, SHRT_MIN, SHRT_MAX)                     \
    X(CTX, IINT, IINT, int, INT_MIN, INT_MAX)                           \
    X(CTX, ILONG, IINT, long, LONG_MIN, LONG_MAX)                       \
    X(CTX, ILLONG, IINT, long long, LLONG_MIN, LLONG_MAX)               \
                                                                        \
    X(CTX, IMAX, IINT, intmax_t, INTMAX_MIN, INTMAX_MAX)                \
    X(CTX, IPTR, IINT, intptr_t, INTPTR_MIN, INTPTR_MAX)                \
    X(CTX, PTRDIFF, IINT, ptrdiff_t, PTRDIFF_MIN, PTRDIFF_MAX)          \
                                                                        \
    X(CTX, I8, IINT, int8_t, INT8_MIN, INT8_MAX)                        \
    X(CTX, I16, IINT, int16_t, INT16_MIN, INT16_MAX)                    \
    X(CTX, I32, IINT, int32_t, INT32_MIN, INT32_MAX)                    \
    X(CTX, I64, IINT, int64_t, INT64_MIN, INT64_MAX)                    \
                                                                        \
    X(CTX, IF8, IINT, int_fast8_t, INT_FAST8_MIN, INT_FAST8_MAX)        \
    X(CTX, IF16, IINT, int_fast16_t, INT_FAST16_MIN, INT_FAST16_MAX)    \
    X(CTX, IF32, IINT, int_fast32_t, INT_FAST32_MIN, INT_FAST32_MAX)    \
    X(CTX, IF64, IINT, int_fast64_t, INT_FAST64_MIN, INT_FAST64_MAX)    \
                                                                        \
    X(CTX, IL8, IINT, int_least8_t, INT_LEAST8_MIN, INT_LEAST8_MAX)     \
    X(CTX, IL16, IINT, int_least16_t, INT_LEAST16_MIN, INT_LEAST16_MAX) \
    X(CTX, IL32, IINT, int_least32_t, INT_LEAST32_MIN, INT_LEAST32_MAX) \
    X(CTX, IL64, IINT, int_least64_t, INT_LEAST64_MIN, INT_LEAST64_MAX) \
                                                                        \
    X(CTX, UCHAR, UINT, unsigned char, 0, UCHAR_MAX)                    \
    X(CTX, USHORT, UINT, unsigned short, 0, SHRT_MAX)                   \
    X(CTX, UINT, UINT, unsigned int, 0, INT_MAX)                        \
    X(CTX, ULONG, UINT, unsigned long, 0, LONG_MAX)                     \
    X(CTX, ULLONG, UINT, unsigned long long, 0, LLONG_MAX)              \
                                                                        \
    X(CTX, UMAX, IINT, uintmax_t, 0, UINTMAX_MAX)                       \
    X(CTX, UPTR, IINT, uintptr_t, 0, UINTPTR_MAX)                       \
    X(CTX, SIZE, IINT, size_t, 0, SIZE_MAX)                             \
                                                                        \
    X(CTX, U8, UINT, uint8_t, 0, UINT8_MAX)                             \
    X(CTX, U16, UINT, uint16_t, 0, UINT16_MAX)                          \
    X(CTX, U32, UINT, uint32_t, 0, UINT32_MAX)                          \
    X(CTX, U64, UINT, uint64_t, 0, UINT64_MAX)                          \
                                                                        \
    X(CTX, UF8, UINT, uint_fast8_t, 0, UINT_FAST8_MAX)                  \
    X(CTX, UF16, UINT, uint_fast16_t, 0, UINT_FAST16_MAX)               \
    X(CTX, UF32, UINT, uint_fast32_t, 0, UINT_FAST32_MAX)               \
    X(CTX, UF64, UINT, uint_fast64_t, 0, UINT_FAST64_MAX)               \
                                                                        \
    X(CTX, UL8, UINT, uint_least8_t, 0, UINT_LEAST8_MAX)                \
    X(CTX, UL16, UINT, uint_least16_t, 0, UINT_LEAST16_MAX)             \
    X(CTX, UL32, UINT, uint_least32_t, 0, UINT_LEAST32_MAX)             \
    X(CTX, UL64, UINT, uint_least64_t, 0, UINT_LEAST64_MAX)             \
                                                                        \
    X(CTX, FLT, REAL, float, -FLT_MAX, FLT_MAX)                         \
    X(CTX, DBL, REAL, double, -DBL_MAX, DBL_MAX)                        \
    X(CTX, LDBL, REAL, long double, -LDBL_MAX, LDBL_MAX)

#define CSTUNN_TYPE_ENUMP(IDEN) CSTUNN_TYPE_##IDEN
#define CSTUNN_TYPE_XLIST_TOENUM(U0, IDEN, ...) CSTUNN_TYPE_ENUMP(IDEN),

enum cstunn_type
{
    CSTUNN_TYPE_XLIST(_, CSTUNN_TYPE_XLIST_TOENUM)
    CSTUNN_TYPE_COUNT
};

struct cstunn_specifier_node
{
    const char
        *name;      /* The name of the field                                    */
    enum cstunn_type
        type;       /* The type of the field                                    */

    size_t
        offset,     /* The offset from the member towards its parent            */
        size;       /* The size of the current field                            */

    const
    struct cstunn_specifier_node
        *prev,      /* The previous member that is part of the same sub-object  */
        *next,      /* The previous member that is part of the same sub-object  */
        *parent,    /* The subobject this field is part of                      */
        *fchild;    /* The first member of this subobject                       */
};

struct cstunn_named_const
{
    const char
        *name;      /* The name of the constant             */
    enum cstunn_type
        type;       /* The type of the constant             */
    const void
        *value;     /* Pointer to the value of the constant */
};

typedef
enum cstunn_error_tag
{
    CSTUNN_OK,
    CSTUNN_ERROR_CHAR_EXPECTED = 0x100,
    CSTUNN_ERROR_CHAR_UNEXPECTED = 0x200,
    CSTUNN_ERROR_INVALID_ESCAPE_SEQUENCE = 0x300,
    CSTUNN_ERROR_INVALID_IDENTIFIER = 0x1000,
    CSTUNN_ERROR_UNKNOWN_IDENTIFIER,
    CSTUNN_ERROR_OUT_OF_BOUNDS,
    CSTUNN_ERROR_INVALID_CONSTANT,
    CSTUNN_ERROR_UNTERMINATED_CHAR_LITERAL,
    CSTUNN_ERROR_UNTERMINATED_STRING_LITERAL,
} cstunn_error_t;

cstunn_error_t
cstunn_parse(
    void *o_buf,
    const char *input,
    const void *specifier,
    char** end,
    const struct cstunn_named_const *consts);

void
cstunn_print_error(
    FILE *stream,
    const char *input_str,
    const char *error_pos,
    cstunn_error_t err);

#endif /* CSTUNN_H_INCLUDED */

#if !defined(CSTUNN_HAS_UTIL) && CSTUNN_WANT_MEMBER_UTIL
#define CSTUNN_HAS_UTIL

#ifndef sizeof_member
#define sizeof_member(TYPE, MEMBER) ( \
    sizeof((TYPE*)0)->MEMBER)
#endif

#ifndef offsetto
#define offsetto(TYPE, LMEMBER, RMEMBER) ( \
    offsetof(TYPE, LMEMBER) - offsetof(TYPE, RMEMBER))
#endif

#ifndef offsetof_child
#define offsetof_child(TYPE, PMEMBER, CMEMBER) ( \
    offsetto(TYPE, PMEMBER CMEMBER, PMEMBER))
#endif

#ifndef count_of
#define count_of(ARR) (sizeof(ARR) / sizeof(*(ARR)))
#endif

#endif /* !defined(CSTUNN_HAS_UTIL) && CSTUNN_WANT_MEMBER_UTIL */
