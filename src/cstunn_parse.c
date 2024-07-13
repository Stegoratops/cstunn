
/* ---------------------------------------------------------------- */

#include "cstunn_parse.h"

#include <ctype.h>
#include <inttypes.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

#include "cstunn_config.h"
#include "cstunn_def.h"
#include "cstunn_internal.h"
#include "cstunn_parseutil.h"
#include "cstunn_typeutil.h"

/* ---------------------------------------------------------------- */

static
void
cstunn_parse_internal(
    struct cstunn_parse_state *r_state);

static
void
cstunn_parse_expression(
    struct cstunn_parse_state *r_state);

static
void
cstunn_parse_string(
    struct cstunn_parse_state *r_state);

static
void
cstunn_parse_initlist(
    struct cstunn_parse_state *r_state);

static
void
cstunn_parse_initlist_member(
    struct cstunn_parse_state *r_state);

static
void
cstunn_parse_struct_path(
    struct cstunn_parse_state *r_state);

static
void
cstunn_parse_array_path(
    struct cstunn_parse_state *r_state);

static
union cstunn_value
cstunn_parse_named_const(
    const char *input,
    char **end,
    const struct cstunn_named_const *consts);

/* ---------------------------------------------------------------- */

static
int
cstunn_handle_out_of_bounds(
    struct cstunn_parse_state *r_state,
    int config_value);

/* ---------------------------------------------------------------- */

cstunn_error_t
cstunn_parse(
    void *o_buf,
    const char *input,
    const void *specifier,
    char** end,
    const struct cstunn_named_const *consts)
{
    struct cstunn_parse_state state = {0};

    input = strchr(input, '{');

    state.v_input = input;
    state.output_buf = o_buf;
    state.specifier_base = state.specifier_curr = specifier;
    state.consts = consts;

    state.v_err = CSTUNN_OK;

    cstunn_parse_internal(&state);

    *end = (char*)state.v_input;
    return state.v_err;
}

/* ---------------------------------------------------------------- */

static
void
cstunn_parse_internal(
    struct cstunn_parse_state *r_state)
{
    if (setjmp(r_state->on_error)) {
        return;
    }

    cstunn_parse_initlist(r_state);
}

static
void
cstunn_parse_expression(
    struct cstunn_parse_state *r_state)
{
    union cstunn_value value;
    char *end;

    const struct cstunn_specifier_node
        *oobnoop_tmp,
        *oobnoop_tmp2;
    size_t oobnoop_tmp_off;

    skipwhile(r_state->v_input, isspace);

    if (*r_state->v_input == '{') {
        cstunn_parse_initlist(r_state);
        return;
    }
    else if (*r_state->v_input == '\"') {
        cstunn_parse_string(r_state);
        return;
    }
    
    if (cstunn_type_get_cat(r_state->specifier_curr->type)
        == CSTUNN_TYPECAT_CONT) {
        cstunn_parse_throw_error(r_state, CSTUNN_ERROR_CHAR_EXPECTED | '{');
    }
    
    if (strchr(CHSET_IDENSTART, *r_state->v_input)) {
        value = cstunn_parse_named_const(
            r_state->v_input,
            &end,
            r_state->consts);
        if (value.m_VOID.type == CSTUNN_TYPE_VOID) {
            cstunn_parse_throw_error(r_state, value.m_UINT.val);
        }
        r_state->v_input = end;
    }
    else {
        value = cstunn_parse_literal(r_state->v_input, &end);
        if (value.m_VOID.type == CSTUNN_TYPE_VOID) {
            cstunn_parse_throw_error(r_state, value.m_UINT.val);
        }
        r_state->v_input = end;
    }

    /* Default writer, used if no early return */

    cstunn_value_cast_to(
        &value,
        r_state->specifier_curr->type);

    /*
     * Pretty much just a hack to get the semantics of
     * CSTUNN_CV_ON_OUT_OF_BOUNDS_NOOP working
     * 
     * Just walks through the tree to compute the current bounds
     * 
     * Could be improved by updating the whole parsing code
     * to store the metadata for the boundschecks
     */
    if (CSTUNN_C_ON_OUT_OF_BOUNDS == CSTUNN_CV_ON_OUT_OF_BOUNDS_NOOP) {
        oobnoop_tmp2 = r_state->specifier_curr;
        oobnoop_tmp_off = r_state->offset;
        while (oobnoop_tmp2->parent && oobnoop_tmp2->type != CSTUNN_TYPE_ARRAY) {
            oobnoop_tmp2 = oobnoop_tmp2->parent;
        }
        oobnoop_tmp = oobnoop_tmp2;
        while (oobnoop_tmp->parent) {
            oobnoop_tmp_off -= oobnoop_tmp->offset;
            oobnoop_tmp = oobnoop_tmp->parent;
        }

        if (oobnoop_tmp_off < oobnoop_tmp2->size) {
            cstunn_value_write(
                r_state->output_buf,
                r_state->offset,
                &value);
        }
    }
    else {
        cstunn_value_write(
            r_state->output_buf,
            r_state->offset,
            &value);
    }
    
    r_state->next_offset = r_state->offset
        + cstunn_type_size(value.m_VOID.type);
}

static
void
cstunn_parse_string(
    struct cstunn_parse_state *r_state)
{
    char *end;
    int ch;

    r_state->next_offset = r_state->offset;

    skipwhile(r_state->v_input, isspace);

    if (r_state->specifier_curr->type != CSTUNN_TYPE_ARRAY) {
        cstunn_parse_throw_error(r_state, CSTUNN_ERROR_CHAR_UNEXPECTED | '\"');
    }

    r_state->specifier_curr = r_state->specifier_curr->fchild;

    if (
        r_state->specifier_curr->type != CSTUNN_TYPE_CHAR
        && r_state->specifier_curr->type != CSTUNN_TYPE_UCHAR
        && r_state->specifier_curr->type != CSTUNN_TYPE_ICHAR) {
        cstunn_parse_throw_error(r_state, CSTUNN_ERROR_CHAR_UNEXPECTED | '\"');
    }

    do {
        ++r_state->v_input;

        while (*r_state->v_input != '\"') {
            if (*r_state->v_input == '\n') {
                cstunn_parse_throw_error(r_state, CSTUNN_ERROR_CHAR_UNEXPECTED | '\n');
            }

            ch = cstunn_esctochar(r_state->v_input, &end);
            r_state->v_input = end;

            if (ch < 0) {
                cstunn_parse_throw_error(r_state, -ch);
            }
            if (r_state->next_offset - r_state->offset
                < r_state->specifier_curr->parent->size
                || cstunn_handle_out_of_bounds(r_state, CSTUNN_C_ON_OUT_OF_BOUNDS))
                ((char*)r_state->output_buf)[r_state->next_offset++] = ch;
        }
        ++r_state->v_input;

        skipwhile(r_state->v_input, isspace);
    } while (*r_state->v_input == '\"');

    /* try to write final nul-char */
    if (r_state->next_offset - r_state->offset
        < r_state->specifier_curr->parent->size
        || cstunn_handle_out_of_bounds(r_state, CSTUNN_C_ON_NULCHAR_OUT_OF_BOUNDS)) {
        ((char*)r_state->output_buf)[r_state->next_offset++] = 0;
    }
}

static
void
cstunn_parse_initlist(
    struct cstunn_parse_state *r_state)
{
    skipwhile(r_state->v_input, isspace);

    if (cstunn_type_get_cat(r_state->specifier_curr->type)
        != CSTUNN_TYPECAT_CONT) {
        cstunn_parse_throw_error(r_state, CSTUNN_ERROR_CHAR_UNEXPECTED | '{');
    }

    if (r_state->specifier_curr->type == CSTUNN_TYPE_ARRAY) {
        r_state->specifier_next = r_state->specifier_curr->fchild;
    }

    while (*r_state->v_input != '}') {
        ++r_state->v_input;
        cstunn_parse_initlist_member(r_state);
        skipwhile(r_state->v_input, isspace);
    }

    ++r_state->v_input;

    r_state->next_offset = r_state->offset
        + r_state->specifier_curr->size;
    r_state->specifier_next = r_state->specifier_curr->parent;
}

static
void
cstunn_parse_initlist_member(
    struct cstunn_parse_state *r_state)
{
    size_t offset = r_state->offset;
    const struct cstunn_specifier_node *node = r_state->specifier_curr;
    
    skipwhile(r_state->v_input, isspace);

    if (*r_state->v_input == '}') {

    }
    else if (
        *r_state->v_input != '.'
        && *r_state->v_input != '[') {
        r_state->specifier_curr = r_state->specifier_next
            ? r_state->specifier_next
            : r_state->specifier_curr->fchild;
        if ((r_state->next_offset - r_state->offset)
            >= r_state->specifier_curr->parent->size
            && !cstunn_handle_out_of_bounds(r_state, CSTUNN_C_ON_OUT_OF_BOUNDS)) {
        }
        r_state->offset = r_state->next_offset;
        if (r_state->specifier_curr->parent->type
            == CSTUNN_TYPE_STRUCT) {
            cstunn_parse_throw_error(r_state, CSTUNN_ERROR_INVALID_IDENTIFIER);
        }

        cstunn_parse_expression(r_state);
    }
    else while (
        *r_state->v_input != ','
        && *r_state->v_input != '}') {
        skipwhile(r_state->v_input, isspace);

        switch (*r_state->v_input) {
        case '.':
            cstunn_parse_struct_path(r_state);
            break;
        case '[':
            cstunn_parse_array_path(r_state);
            break;
        case '=':
            ++r_state->v_input;
            skipwhile(r_state->v_input, isspace);
            cstunn_parse_expression(r_state);
            break;
        }
    }

    r_state->specifier_next = r_state->specifier_curr->next;

    r_state->offset = offset;
    r_state->specifier_curr = node;
}

static
void
cstunn_parse_struct_path(
    struct cstunn_parse_state *r_state)
{
    size_t length;

    skipwhile(r_state->v_input, isspace);

    if (*r_state->v_input != '.') {
        cstunn_parse_throw_error(r_state, CSTUNN_ERROR_CHAR_EXPECTED | '.');
    }
    if (r_state->specifier_curr->type != CSTUNN_TYPE_STRUCT) {
        cstunn_parse_throw_error(r_state, CSTUNN_ERROR_CHAR_UNEXPECTED | '.');
    }

    ++r_state->v_input;

    skipwhile(r_state->v_input, isspace);

    r_state->specifier_curr = r_state->specifier_curr->fchild;
    
    if (!strchr(CHSET_IDENSTART, *r_state->v_input)) {
        cstunn_parse_throw_error(r_state, CSTUNN_ERROR_INVALID_IDENTIFIER);
    }
    length = strspn(r_state->v_input, CHSET_IDEN);

    while (
        strlen(r_state->specifier_curr->name) != length
        || memcmp(
            r_state->specifier_curr->name,
            r_state->v_input, length)) {
        r_state->specifier_curr = r_state->specifier_curr->next;
        if (!r_state->specifier_curr) {
            cstunn_parse_throw_error(r_state, CSTUNN_ERROR_UNKNOWN_IDENTIFIER);
        }
    }

    r_state->v_input += length;

    r_state->offset += r_state->specifier_curr->offset;
    r_state->next_offset = r_state->offset;
}

static
void
cstunn_parse_array_path(
    struct cstunn_parse_state *r_state)
{
    union cstunn_value value;
    char *end;

    skipwhile(r_state->v_input, isspace);

    if (r_state->specifier_curr->type != CSTUNN_TYPE_ARRAY) {
        cstunn_parse_throw_error(r_state, CSTUNN_ERROR_CHAR_UNEXPECTED | '[');
    }

    ++r_state->v_input;

    skipwhile(r_state->v_input, isspace);

    value = cstunn_parse_literal(r_state->v_input, &end);
    if (value.m_VOID.type == CSTUNN_TYPE_VOID) {
        value = cstunn_parse_named_const(r_state->v_input, &end, r_state->consts);
    }
    if (value.m_VOID.type == CSTUNN_TYPE_VOID) {
        cstunn_parse_throw_error(r_state, value.m_UINT.val);
    }

    cstunn_value_cast_to(&value, CSTUNN_TYPE_SIZE);
    
    if (value.m_UINT.val
        >= r_state->specifier_curr->size
        / r_state->specifier_curr->fchild->size) {
        cstunn_handle_out_of_bounds(r_state, CSTUNN_C_ON_OUT_OF_BOUNDS);
    }

    r_state->v_input = end;
    skipwhile(r_state->v_input, isspace);
    if (*r_state->v_input != ']') {
        cstunn_parse_throw_error(
            r_state,
            CSTUNN_ERROR_CHAR_UNEXPECTED | *r_state->v_input);
    }
    ++r_state->v_input;

    r_state->specifier_curr = r_state->specifier_curr->fchild;
    r_state->offset += r_state->specifier_curr->size
        * value.m_UINT.val;
    r_state->next_offset = r_state->offset;
}

void
cstunn_parse_throw_error_(
    struct cstunn_parse_state *r_state,
    cstunn_error_t err)
{
    r_state->v_err = err;
    longjmp(r_state->on_error, 1);
}

static
union cstunn_value
cstunn_parse_named_const(
    const char *input,
    char **end,
    const struct cstunn_named_const *consts)
{
    union cstunn_value result;
    size_t length = strspn(input, CHSET_IDEN);

    if (!consts) {
        result.m_IINT.val = CSTUNN_ERROR_UNKNOWN_IDENTIFIER;
        result.m_IINT.type = CSTUNN_TYPE_VOID;
        return result;
    }

    while (
        strlen(consts->name) != length
        || memcmp(
            consts->name,
            input, length)) {
        ++consts;
        if (!consts->name) {
            result.m_IINT.val = CSTUNN_ERROR_UNKNOWN_IDENTIFIER;
            result.m_IINT.type = CSTUNN_TYPE_VOID;
            return result;
        }
    }

    if (cstunn_value_fromvp(
        &result,
        consts->value,
        consts->type) < 0) {
        result.m_IINT.val = CSTUNN_ERROR_INVALID_CONSTANT;
        result.m_IINT.type = CSTUNN_TYPE_VOID;
        return result;
    }
    
    if (end) *end = (char*)(input + length);

    return result;
}

/* ---------------------------------------------------------------- */

static
int
cstunn_handle_out_of_bounds(
    struct cstunn_parse_state *r_state,
    int config_value)
{
    switch (config_value) {

    case CSTUNN_CV_ON_OUT_OF_BOUNDS_THROW:
        cstunn_parse_throw_error(r_state, CSTUNN_ERROR_OUT_OF_BOUNDS);

    case CSTUNN_CV_ON_OUT_OF_BOUNDS_TRAP:
        exit(1);
    
    case CSTUNN_CV_ON_OUT_OF_BOUNDS_NOOP:
        return 0;
    
    case CSTUNN_CV_ON_OUT_OF_BOUNDS_WRITE:
        return 1;

    default:
        CSTUNN_UNREACHABLE();
    }

    CSTUNN_UNREACHABLE();
    return 0;
}

/* ---------------------------------------------------------------- */
