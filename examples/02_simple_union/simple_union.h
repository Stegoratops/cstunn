#ifndef EXAMPLES_SIMPLE_UNION_H_INCLUDED
#define EXAMPLES_SIMPLE_UNION_H_INCLUDED

#include <stdio.h>

#define CSTUNN_WANT_MEMBER_UTIL 1
#include "include/cstunn.h"

union simple_union
{
    int a;
    float b;
    unsigned char c[10];
};

enum {
    simple_union_SPEC_a = 1,
    simple_union_SPEC_b,
    simple_union_SPEC_c,
    simple_union_SPEC_c_0,
};

const
struct cstunn_specifier_node
simple_union_spec[] = {
    {   /*
         * Note that there isn't any unique
         * union type, and we are still using
         * CSTUNN_TYPE_STRUCT, since there is
         * no specific necessity for the values
         * to not overlap
         */
        .type = CSTUNN_TYPE_STRUCT,
        .size = sizeof(union simple_union),

        .fchild = simple_union_spec + simple_union_SPEC_a
    },
    [simple_union_SPEC_a] = {
        .name = "a",
        .type = CSTUNN_TYPE_IINT,
        .size = sizeof_member(union simple_union, a),
        .offset = offsetof(union simple_union, a),

        .parent = simple_union_spec,
        /* first member of union, so no .prev */
        .next = simple_union_spec + simple_union_SPEC_b
    },
    [simple_union_SPEC_b] = {
        .name = "b",
        .type = CSTUNN_TYPE_FLT,
        .size = sizeof_member(union simple_union, b),
        .offset = offsetof(union simple_union, b),

        .parent = simple_union_spec,
        .prev = simple_union_spec + simple_union_SPEC_a,
        .next = simple_union_spec + simple_union_SPEC_c
    },
    [simple_union_SPEC_c] = {
        .name = "c",
        .type = CSTUNN_TYPE_ARRAY,
        .size = sizeof_member(union simple_union, c),
        .offset = offsetof(union simple_union, c),

        .parent = simple_union_spec,
        .fchild = simple_union_spec + simple_union_SPEC_c_0,
        .prev = simple_union_spec + simple_union_SPEC_b,
        /* last member of union, so no .next */
    },
    [simple_union_SPEC_c_0] = {
        /* No .name and .offset because its the child of an array */
        .type = CSTUNN_TYPE_CHAR,
        .size = sizeof_member(union simple_union, c[0]),

        .parent = simple_union_spec + simple_union_SPEC_c
    }
};

/*
 * Would usually be in its own "simple_union.c" but for ease
 * of compilation is in the header aswell
 */
static
void
simple_union_print(
    union simple_union *sun)
{
    printf("(union simple_union){"
        "\n\t.a = %d"
        "\n\t.b = %f"
        "\n\t.c = \"%.10s\""
        "\n};"
        "\n",
        sun->a,
        sun->b,
        sun->c);
}

#endif /* EXAMPLES_SIMPLE_UNION_H_INCLUDED */
