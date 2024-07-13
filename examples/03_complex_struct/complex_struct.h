#ifndef EXAMPLES_COMPLEX_STRUCT_H_INCLUDED
#define EXAMPLES_COMPLEX_STRUCT_H_INCLUDED

#include <stdio.h>

#define CSTUNN_WANT_MEMBER_UTIL 1
#include "include/cstunn.h"

struct complex_struct
{
    struct {
        int an_i;
        float an_f;
    } /* Annonymous */;

    int i;
    float f;

    struct {
        int i;
        float f;
    } named;

    struct {
        int i;
        float f;
    } arr[3];
};

enum
{
    /* Note there is no entry for the anonymous struct */
    complex_struct_SPEC_an_i = 1,
    complex_struct_SPEC_an_f,
    complex_struct_SPEC_i,
    complex_struct_SPEC_f,
    complex_struct_SPEC_named,
    complex_struct_SPEC_named_i,
    complex_struct_SPEC_named_f,
    complex_struct_SPEC_arr,
    complex_struct_SPEC_arr_0,
    complex_struct_SPEC_arr_0_i,
    complex_struct_SPEC_arr_0_f,
};

const
struct cstunn_specifier_node
complex_struct_spec[] = {
    {
        .type = CSTUNN_TYPE_STRUCT,
        .size = sizeof(struct complex_struct),

        .fchild = complex_struct_spec + complex_struct_SPEC_an_i
    },
    /*
     * Members of anonymous structs/unions
     * behave as if direct member of parent
     */
    [complex_struct_SPEC_an_i] = {
        .name = "an_i",
        .type = CSTUNN_TYPE_IINT,
        .size = sizeof_member(struct complex_struct, an_i),
        .offset = offsetof(struct complex_struct, an_i),

        .parent = complex_struct_spec,
        /* first member of struct, so no .prev */
        .next = complex_struct_spec + complex_struct_SPEC_an_f
    },
    [complex_struct_SPEC_an_f] = {
        .name = "an_f",
        .type = CSTUNN_TYPE_FLT,
        .size = sizeof_member(struct complex_struct, an_f),
        .offset = offsetof(struct complex_struct, an_f),

        .parent = complex_struct_spec,
        .prev = complex_struct_spec + complex_struct_SPEC_an_i,
        .next = complex_struct_spec + complex_struct_SPEC_i
    },
    [complex_struct_SPEC_i] = {
        .name = "i",
        .type = CSTUNN_TYPE_IINT,
        .size = sizeof_member(struct complex_struct, i),
        .offset = offsetof(struct complex_struct, i),

        .parent = complex_struct_spec,
        .prev = complex_struct_spec + complex_struct_SPEC_an_f,
        .next = complex_struct_spec + complex_struct_SPEC_f
    },
    [complex_struct_SPEC_f] = {
        .name = "f",
        .type = CSTUNN_TYPE_FLT,
        .size = sizeof_member(struct complex_struct, f),
        .offset = offsetof(struct complex_struct, f),

        .parent = complex_struct_spec,
        .prev = complex_struct_spec + complex_struct_SPEC_i,
        .next = complex_struct_spec + complex_struct_SPEC_named
    },
    [complex_struct_SPEC_named] = {
        .name = "named",
        .type = CSTUNN_TYPE_STRUCT,
        .size = sizeof_member(struct complex_struct, named),
        .offset = offsetof(struct complex_struct, named),

        .parent = complex_struct_spec,
        .fchild = complex_struct_spec + complex_struct_SPEC_named_i,
        .prev = complex_struct_spec + complex_struct_SPEC_i,
        .next = complex_struct_spec + complex_struct_SPEC_arr
    },
    [complex_struct_SPEC_named_i] = {
        .name = "i",
        .type = CSTUNN_TYPE_IINT,
        .size = sizeof_member(struct complex_struct, named.i),
        .offset = offsetof_child(struct complex_struct, named, .i),

        .parent = complex_struct_spec + complex_struct_SPEC_named,
        /* first member of struct, so no .prev */
        .next = complex_struct_spec + complex_struct_SPEC_named_f
    },
    [complex_struct_SPEC_named_f] = {
        .name = "f",
        .type = CSTUNN_TYPE_FLT,
        .size = sizeof_member(struct complex_struct, named.f),
        .offset = offsetof_child(struct complex_struct, named, .f),

        .parent = complex_struct_spec + complex_struct_SPEC_named,
        .prev = complex_struct_spec + complex_struct_SPEC_named_i
        /* last member of struct, so no .next */
    },
    [complex_struct_SPEC_arr] = {
        .name = "arr",
        .type = CSTUNN_TYPE_ARRAY,
        .size = sizeof_member(struct complex_struct, arr),
        .offset = offsetof(struct complex_struct, arr),

        .parent = complex_struct_spec,
        .fchild = complex_struct_spec + complex_struct_SPEC_arr_0,
        .prev = complex_struct_spec + complex_struct_SPEC_named,
        /* last member of struct, so no .next */
    },
    [complex_struct_SPEC_arr_0] = {
        /* No .name and .offset because its the child of an array */
        .type = CSTUNN_TYPE_STRUCT,
        .size = sizeof_member(struct complex_struct, arr[0]),

        .parent = complex_struct_spec + complex_struct_SPEC_arr,
        .fchild = complex_struct_spec + complex_struct_SPEC_arr_0_i,
    },
    [complex_struct_SPEC_arr_0_i] = {
        .name = "i",
        .type = CSTUNN_TYPE_IINT,
        .size = sizeof_member(struct complex_struct, arr[0].i),
        .offset = offsetof_child(struct complex_struct, arr[0], .i),

        .parent = complex_struct_spec + complex_struct_SPEC_arr_0,
        /* first member of struct, so no .prev */
        .next = complex_struct_spec + complex_struct_SPEC_arr_0_f
    },
    [complex_struct_SPEC_arr_0_f] = {
        .name = "f",
        .type = CSTUNN_TYPE_FLT,
        .size = sizeof_member(struct complex_struct, arr[0].f),
        .offset = offsetof_child(struct complex_struct, arr[0], .f),

        .parent = complex_struct_spec + complex_struct_SPEC_arr_0,
        .prev = complex_struct_spec + complex_struct_SPEC_arr_0_i
        /* last member of struct, so no .next */
    },
};


/*
 * Would usually be in its own "simple_struct.c" but for ease
 * of compilation is in the header aswell
 */
static
void
complex_struct_print(
    struct complex_struct *st)
{
    printf("(struct complex_struct){"
        "\n\t.an_i = %d,"
        "\n\t.an_f = %f,"
        "\n\t.i = %d,"
        "\n\t.f = %f,"
        "\n\t.named.i = %d,"
        "\n\t.named.f = %f,"
        "\n\t.arr[0].i = %d,"
        "\n\t.arr[0].f = %f,"
        "\n\t.arr[1].i = %d,"
        "\n\t.arr[1].f = %f,"
        "\n\t.arr[2].i = %d,"
        "\n\t.arr[2].f = %f,"
        "\n};"
        "\n",
        st->an_i,
        st->an_f,
        st->i,
        st->f,
        st->named.i,
        st->named.f,
        st->arr[0].i,
        st->arr[0].f,
        st->arr[1].i,
        st->arr[1].f,
        st->arr[2].i,
        st->arr[2].f);
}

#endif /* EXAMPLES_COMPLEX_STRUCT_H_INCLUDED */