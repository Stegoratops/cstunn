#ifndef EXAMPLES_SIMPLE_STRUCT_H_INCLUDED
#define EXAMPLES_SIMPLE_STRUCT_H_INCLUDED

#include <stdio.h>

/*
 * For convinience, we define this macro
 * If your codebase already has similiar
 * functionality as provided through this
 * macro, it can be omitted.
 */
#define CSTUNN_WANT_MEMBER_UTIL 1
#include "include/cstunn.h"

struct simple_struct
{
    int a, b, c;
    float d;
    char name[20];
};

/*
 * For a better readability of the specifier
 * an enum like this can be defined
 */
enum {
    simple_struct_SPEC_a = 1,
    simple_struct_SPEC_b,
    simple_struct_SPEC_c,
    simple_struct_SPEC_d,
    simple_struct_SPEC_name,
    simple_struct_SPEC_name_0,
};

const
struct cstunn_specifier_node
simple_struct_spec[] = {
    {   /*
         * This specifies the root-node. It has
         * to be of type struct or array
         * 
         * there is no .name or .offset, since this is the
         * base
         */
        .type = CSTUNN_TYPE_STRUCT,
        .size = sizeof(struct simple_struct),

        .fchild = simple_struct_spec + simple_struct_SPEC_a
    },
    [simple_struct_SPEC_a] = {
        .name = "a",
        .type = CSTUNN_TYPE_IINT,
        .size = sizeof_member(struct simple_struct, a),
        .offset = offsetof(struct simple_struct, a),

        .parent = simple_struct_spec,
        /* first member of struct, so no .prev */
        .next = simple_struct_spec + simple_struct_SPEC_b
    },
    [simple_struct_SPEC_b] = {
        .name = "b",
        .type = CSTUNN_TYPE_IINT,
        .size = sizeof_member(struct simple_struct, b),
        .offset = offsetof(struct simple_struct, b),

        .parent = simple_struct_spec,
        .prev = simple_struct_spec + simple_struct_SPEC_a,
        .next = simple_struct_spec + simple_struct_SPEC_c
    },
    [simple_struct_SPEC_c] = {
        .name = "c",
        .type = CSTUNN_TYPE_IINT,
        .size = sizeof_member(struct simple_struct, c),
        .offset = offsetof(struct simple_struct, c),

        .parent = simple_struct_spec,
        .prev = simple_struct_spec + simple_struct_SPEC_b,
        .next = simple_struct_spec + simple_struct_SPEC_d
    },
    [simple_struct_SPEC_d] = {
        .name = "d",
        .type = CSTUNN_TYPE_FLT,
        .size = sizeof_member(struct simple_struct, d),
        .offset = offsetof(struct simple_struct, d),

        .parent = simple_struct_spec,
        .prev = simple_struct_spec + simple_struct_SPEC_c,
        .next = simple_struct_spec + simple_struct_SPEC_name
    },
    [simple_struct_SPEC_name] = {
        .name = "name",
        .type = CSTUNN_TYPE_ARRAY,
        .size = sizeof_member(struct simple_struct, name),
        .offset = offsetof(struct simple_struct, name),

        .parent = simple_struct_spec,
        /*
         * Nodes with .type == CSTUNN_TYPE_ARRAY have a single child
         * which specifies the type of the array
         */
        .fchild = simple_struct_spec + simple_struct_SPEC_name_0,
        .prev = simple_struct_spec + simple_struct_SPEC_d,
        /* last member of struct, so no .next */
    },
    [simple_struct_SPEC_name_0] = {
        /* No .name and .offset because its the child of an array */
        .type = CSTUNN_TYPE_CHAR,
        .size = sizeof_member(struct simple_struct, name[0]),

        .parent = simple_struct_spec + simple_struct_SPEC_name,
    },
};

/*
 * Would usually be in its own "simple_struct.c" but for ease
 * of compilation is in the header aswell
 */
static
void
simple_struct_print(
    struct simple_struct *st)
{
    printf("(struct simple_struct){"
        "\n\t.a = %d"
        "\n\t.b = %d"
        "\n\t.c = %d"
        "\n\t.d = %f"
        "\n\t.name = \"%.20s\""
        "\n};"
        "\n",
        st->a,
        st->b,
        st->c,
        st->d,
        st->name);
}

#endif /* EXAMPLES_SIMPLE_STRUCT_H_INCLUDED */
