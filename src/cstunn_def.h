#ifndef CSTUNNI_DEF_H_INCLUDED
#define CSTUNNI_DEF_H_INCLUDED

#if __STDC_VERSION__ >= 202311L
#define CSTUNN_UNREACHABLE_() unreachable()
#define CSTUNN_FALLTHROUGH() [[fallthrough]]
#elif __GNUC__
#define CSTUNN_UNREACHABLE_() (__builtin_unreachable())
#define CSTUNN_FALLTHROUGH() __attribute__((fallthrough))
#elif _MSC_VER
#define CSTUNN_UNREACHABLE_() (__assume(0))
#define CSTUNN_FALLTHROUGH() [[fallthrough]]
#else
#define CSTUNN_UNREACHABLE_()
#define CSTUNN_FALLTHROUGH()
#endif

#if NDEBUG
#define CSTUNN_UNREACHABLE() CSTUNN_UNREACHABLE_()
#else
#include <assert.h>
#define CSTUNN_UNREACHABLE() assert(0)
#endif

#define CSTUNN_CAT(A, B) CSTUNN_CAT_(A, B)
#define CSTUNN_CAT_(A, B) A##B

#define CHSET_DIGIT "0123456789"
#define CHSET_XDIGIT CHSET_DIGIT "abcdefABCDEF"
#define CHSET_ODIGIT "01234567"
#define CHSET_ALPHA              \
    "abcdefghijklmnopqrstuvwxyz" \
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define CHSET_ALPHANUM CHSET_ALPHA CHSET_DIGIT "_"

#define CHSET_IDENSTART CHSET_ALPHA "_"
#define CHSET_IDEN CHSET_ALPHANUM

#define CHSET_SPACE "\v\t\n \r"

#define skipwhile(STR, FUNC) while (FUNC(*(STR))) ++(STR)

#endif /* CSTUNNI_DEF_H_INCLUDED */