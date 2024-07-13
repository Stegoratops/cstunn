
/* ---------------------------------------------------------------- */

#include "cstunn_parseutil.h"

/* ---------------------------------------------------------------- */

#include <ctype.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "cstunn_internal.h"
#include "cstunn_def.h"

/* ---------------------------------------------------------------- */

union cstunn_value
cstunn_parse_literal(
    const char *str,
    char **end)
{
    union cstunn_value res = { { CSTUNN_TYPE_VOID } };

    CSTUNN_IINT_VAL_TYPE v_char;
    CSTUNN_IINT_VAL_TYPE v_iint;
    CSTUNN_UINT_VAL_TYPE v_uint;
    CSTUNN_REAL_VAL_TYPE v_real;
    char *e_char;
    char *e_iint;
    char *e_uint;
    char *e_real;

    skipwhile(str, isspace);

    v_char = cstunn_strtochar(str, &e_char);
    v_iint = strtoimax(str, &e_iint, 0);
    v_uint = strtoumax(str, &e_uint, 0);
    v_real = strtold(str, &e_real);

    if (str == e_char
        && str == e_iint
        && str == e_uint
        && str == e_real) {
        res.m_UINT.val = CSTUNN_ERROR_CHAR_UNEXPECTED | *str;
        ++str;
    }
    else if (*str == '\'') {
        str = e_char;
        if (v_char >= 0) {
            res.m_IINT.type = CSTUNN_TYPE_IMAX;
            res.m_IINT.val = v_char;
        }
        else {
            res.m_UINT.val = -v_char;
        }
    }
    else if (*e_iint == '.') {
        res.m_REAL.type = CSTUNN_TYPE_LDBL;
        res.m_REAL.val = v_real;
        str = e_real;
    }
    else if (*str == '-') {
        res.m_IINT.type = CSTUNN_TYPE_IMAX;
        res.m_IINT.val = v_iint;
        str = e_iint;
    }
    else {
        res.m_UINT.type = CSTUNN_TYPE_UMAX;
        res.m_UINT.val = v_uint;
        str = e_uint;
    }

    if (end) *end = (char*)str;

    return res;
}

int
cstunn_esctochar(
    const char *str,
    char **end)
{
    char *e;
    int res;

    if (*str == '\\') {
        ++str;
        switch (*str) {
        case 'a':
            res = '\a';
            ++str;
            break;
        case 'b':
            res = '\b';
            ++str;
            break;
        case 'f':
            res = '\f';
            ++str;
            break;
        case 'n':
            res = '\n';
            ++str;
            break;
        case 'r':
            res = '\r';
            ++str;
            break;
        case 't':
            res = '\t';
            ++str;
            break;
        case 'v':
            res = '\v';
            ++str;
            break;
        case '\'':
            res = '\'';
            ++str;
            break;
        case '\"':
            res = '\"';
            ++str;
            break;
        case '\\':
            res = '\\';
            ++str;
            break;
        case 'x':
            ++str;
            if (!strchr(CHSET_XDIGIT, *str)) {
                res = -(CSTUNN_ERROR_CHAR_UNEXPECTED | *str);
                goto f_end;
            }
            res = strtol(str, &e, 16);
            str = e;
            break;
        default:
            if (!strchr(CHSET_ODIGIT, *str)) {
                res = -(CSTUNN_ERROR_CHAR_UNEXPECTED | *str);
                goto f_end;
            }

            res = strtol(str, &e, 8);
            str = e;
            break;
        }
    }
    else {
        res = *str++;
    }

f_end:
    if (end) *end = (char*)str;

    return res;
}

intmax_t
cstunn_strtochar(
    const char *str,
    char **end)
{
    char *iend;
    intmax_t res = 0, t;

    skipwhile(str, isspace);

    if (*str != '\'') {
        res = -(CSTUNN_ERROR_CHAR_EXPECTED | '\'');
        goto f_end;
    }

    ++str;

    do {
        res <<= CHAR_BIT;
        t = cstunn_esctochar(str, &iend);
        res |= t;
        str = iend;
    }
    while (
        *str
        && *str != '\n'
        && *str != '\''
        && t >= 0
        && res <= INTMAX_MAX >> CHAR_BIT);

    if (*str != '\'') {
        res = -CSTUNN_ERROR_UNTERMINATED_CHAR_LITERAL;
        goto f_end;
    }
    else if (t < 0) {
        res = -t;
        goto f_end;
    }
    ++str;

f_end:
    if (end) *end = (char*)str;

    return res;
}

/* ---------------------------------------------------------------- */
