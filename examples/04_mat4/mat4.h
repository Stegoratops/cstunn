#ifndef EXAMPLES_MAT4_H_INCLUDED
#define EXAMPLES_MAT4_H_INCLUDED

#include <stdio.h>

#define CSTUNN_WANT_MEMBER_UTIL 1
#include "include/cstunn.h"

struct mat4
{
    float m[4][4];
};

const extern
struct cstunn_specifier_node
mat4_spec[];

const
struct cstunn_specifier_node
mat4_spec[] = {
    {
        .type = CSTUNN_TYPE_STRUCT,
        .size = sizeof(struct mat4),

        .fchild = mat4_spec + 1
    },
    {
        .name = "m",
        .type = CSTUNN_TYPE_ARRAY,
        .size = sizeof_member(struct mat4, m),
        .offset = offsetof(struct mat4, m),

        .parent = mat4_spec,
        .fchild = mat4_spec + 2
    },
    {
        .type = CSTUNN_TYPE_ARRAY,
        .size = sizeof_member(struct mat4, m[0]),

        .parent = mat4_spec + 1,
        .fchild = mat4_spec + 3
    },
    {
        .type = CSTUNN_TYPE_FLT,
        .size = sizeof_member(struct mat4, m[0][0]),

        .parent = mat4_spec + 2
    }
};

const float PI = 3.141592657;
const float E =  2.718281828;
const int ANSWER = 42;
const unsigned char ONE = 1;

const
struct cstunn_named_const
mat4_consts[] = {
    {
        .name = "PI",
        .type = CSTUNN_TYPE_FLT,
        .value = &PI
    },
    {
        .name = "E",
        .type = CSTUNN_TYPE_FLT,
        .value = &E
    },
    {
        .name = "ANSWER",
        .type = CSTUNN_TYPE_IINT,
        .value = &ANSWER
    },
    {
        .name = "ONE",
        .type = CSTUNN_TYPE_UCHAR,
        .value = &ONE
    },
};

/*
 * Would usually be in its own "simple_struct.c" but for ease
 * of compilation is in the header aswell
 */
static
void
mat4_print(
    struct mat4 *m)
{
    int i, j;

    for (i = 0; i < 4; ++i) {
        printf("| ");
        for (j = 0; j < 4; ++j) {
            printf("%6.3f ", m->m[i][j]);
        }
        printf("|\n");
    }
}

#endif /* EXAMPLES_MAT4_H_INCLUDED */