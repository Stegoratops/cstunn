#ifndef CSTUNN_INTERNAL_H_INCLUDED
#define CSTUNN_INTERNAL_H_INCLUDED

#include "include/cstunn.h"


#define CSTUNN_TYPECAT_ENUMP(IDEN) CSTUNN_CAT(CSTUNN_TYPECAT_, IDEN)
#define CSTUNN_VALUE_GETCAT(VALUE, CAT) (VALUE)->CSTUNN_CAT(m_,CAT)

enum cstunn_typecat
{
    CSTUNN_TYPECAT_VOID,
    CSTUNN_TYPECAT_CONT,
    CSTUNN_TYPECAT_CHAR,
    CSTUNN_TYPECAT_IINT,
    CSTUNN_TYPECAT_UINT,
    CSTUNN_TYPECAT_REAL
};

struct cstunn_VOID
{
    enum cstunn_type type;
};

struct cstunn_CONT
{
    enum cstunn_type type;
};

struct cstunn_CHAR
{
    enum cstunn_type type;
    char val;
};

struct cstunn_IINT
{
    enum cstunn_type type;
    CSTUNN_IINT_VAL_TYPE val;
};

struct cstunn_UINT
{
    enum cstunn_type type;
    CSTUNN_UINT_VAL_TYPE val;
};

struct cstunn_REAL
{
    enum cstunn_type type;
    CSTUNN_REAL_VAL_TYPE val;
};

union cstunn_value
{
    struct cstunn_VOID m_VOID;
    struct cstunn_CONT m_CONT;
    struct cstunn_CHAR m_CHAR;
    struct cstunn_IINT m_IINT;
    struct cstunn_UINT m_UINT;
    struct cstunn_REAL m_REAL;
};

#endif /* CSTUNN_INTERNAL_H_INCLUDED */