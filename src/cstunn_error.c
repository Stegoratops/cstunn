
/* ---------------------------------------------------------------- */

#include "cstunn_internal.h"

#include <ctype.h>
#include <stdint.h>
#include <string.h>

/* ---------------------------------------------------------------- */

static
void
cstunn_printchar(
    FILE *stream,
    int ch);

/* ---------------------------------------------------------------- */

#define MAXLINELENGTH ((size_t)40)

void
cstunn_print_error(
    FILE *stream,
    const char *input_str,
    const char *error_pos,
    cstunn_error_t err)
{
    const char *p = input_str;
    size_t line = 1, chpos, length;

    while (p < error_pos) {
        if (*p == '\n') ++line;
        ++p;
    };

    while (p != input_str
        && p[-1] != '\n') --p;
    chpos = error_pos - p;
    length = strcspn(p, "\n");

    fprintf(
        stream,
        "%zu:%ti: ",
        line,
        chpos + 1);
    
    if ((err & ~0xFFu) == CSTUNN_ERROR_CHAR_EXPECTED) {
        fprintf(stream, "error: Expected character ");
        cstunn_printchar(stream, err & 0xFF);
    }
    else if (err == (CSTUNN_ERROR_CHAR_UNEXPECTED | '\n')) {
        fprintf(stream, "error: Unexpected newline");
    }
    else if ((err & ~0xFFu) == CSTUNN_ERROR_CHAR_UNEXPECTED) {
        fprintf(stream, "error: Unexpected character ");
        cstunn_printchar(stream, err & 0xFF);
    }
    else switch (err) {
    case CSTUNN_ERROR_INVALID_IDENTIFIER:
        fprintf(stream, "error: Invalid identifier");
        break;
    case CSTUNN_ERROR_UNKNOWN_IDENTIFIER:
        fprintf(
            stream,
            "error: Identifier doesn't match any known one");
        break;
    case CSTUNN_ERROR_OUT_OF_BOUNDS:
        fprintf(stream, "error: Array Index out of Bounds");
        break;
    case CSTUNN_ERROR_INVALID_CONSTANT:
        fprintf(stream, "error: Invalid value for constant");
        break;
    default:
        break;
    }
    fprintf(stream, "\n");

    if (length > MAXLINELENGTH) {
        p = error_pos - p + MAXLINELENGTH / 2 < length
            ? error_pos - MAXLINELENGTH / 2
            : p + length - MAXLINELENGTH;
        length = MAXLINELENGTH;
    }

    fprintf(stream, "%5zu | %.*s\n", line, (int)length, p);
    fprintf(stream, "      | ");
    while (p < error_pos) {
        if (*p == '\t') fprintf(stream, "\t");
        else fprintf(stream, " ");
        ++p;
    }
    fprintf(stream, "^\n");
}

/* ---------------------------------------------------------------- */

static
void
cstunn_printchar(
    FILE *stream,
    int ch)
{
    if (isprint(ch)) {
        fprintf(stream, "'%c'", ch);
    }
    else {
        fprintf(stream, "'\\x%x'", ch);
    }
}

/* ---------------------------------------------------------------- */
