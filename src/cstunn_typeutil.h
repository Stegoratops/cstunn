#ifndef CSTUNNI_TYPEUTIL_H_INCLUDED
#define CSTUNNI_TYPEUTIL_H_INCLUDED

#include "cstunn_internal.h"

enum cstunn_typecat
cstunn_type_get_cat(
    enum cstunn_type type);

void
cstunn_value_cast_to(
    union cstunn_value *val,
    enum cstunn_type type);

void
cstunn_value_write(
    void *base,
    size_t off,
    const union cstunn_value *val);

int
cstunn_value_fromvp(
    union cstunn_value *dest,
    const void *src,
    enum cstunn_type type);

size_t
cstunn_type_size(
    enum cstunn_type type);

#endif /* CSTUNNI_TYPEUTIL_H_INCLUDED */