#ifndef CSTUNNI_PARSEUTIL_H_INCLUDED
#define CSTUNNI_PARSEUTIL_H_INCLUDED

#include <stdint.h>

union cstunn_value
cstunn_parse_literal(
    const char *input,
    char **end);

int
cstunn_esctochar(
    const char *str,
    char **end);

intmax_t
cstunn_strtochar(
    const char *str,
    char **end);

#endif /* CSTUNNI_PARSEUTIL_H_INCLUDED */