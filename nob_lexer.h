/* nob_lexer.h - General purpose lexer/tokenizer for nob.h
 *
 * This is a single-header library. Define NOB_LEXER_IMPLEMENTATION in exactly one
 * source file before including this header to get the implementation.
 *
 * Requires C99 or later (for strtod hex float support).
 * Requires nob.h to be included before this header with NOB_IMPLEMENTATION defined.
 *
 * Features:
 *   - Identifiers (including UTF-8/Unicode)
 *   - Integers: decimal, hex (0x), binary (0b), octal (0o)
 *   - Floats: decimal (3.14, 1e10) and C99 hex floats (0x1.Fp+10)
 *   - String and character literals with escape sequences
 *   - Line (//) and block comments
 *   - Location tracking (file:line:column)
 *
 * Usage:
 *   #define NOB_IMPLEMENTATION
 *   #include "nob.h"
 *
 *   #define NOB_LEXER_IMPLEMENTATION
 *   #include "nob_lexer.h"
 *
 *   int main(void) {
 *       Nob_String_Builder sb = {0};
 *       nob_read_entire_file("source.c", &sb);
 *
 *       Nob_Lexer_Tokens tokens = {0};
 *       nob_lexer_tokenize(nob_sb_to_sv(sb), "source.c", &tokens);
 *
 *       nob_da_foreach(Nob_Lexer_Token, tok, &tokens) {
 *           printf("%s:%zu:%zu: %s\n",
 *                  tok->loc.file, tok->loc.line, tok->loc.column,
 *                  nob_lexer_token_kind_str(tok->kind));
 *       }
 *
 *       nob_da_free(tokens);
 *       nob_sb_free(sb);
 *       return 0;
 *   }
 *
 * Configuration (define before including):
 *   NOB_LEXER_SKIP_COMMENTS - If defined, comments are not included in token output
 */

#ifndef NOB_LEXER_H_
#define NOB_LEXER_H_

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS (1)
#endif

/* Require C99 or later */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    /* C99+ is fine */
#elif defined(__cplusplus)
    /* C++ is fine */
#elif defined(_MSC_VER) && _MSC_VER >= 1800
    /* MSVC 2013+ is fine (C99-ish support) */
#else
#error "nob_lexer.h requires C99 or later (for strtod hex float support)"
#endif

#ifndef NOBLEXERDEF
#define NOBLEXERDEF
#endif

/* Requires nob.h */
#ifndef NOB_H_
#error "nob_lexer.h requires nob.h to be included first"
#endif

#include <stdint.h>

/* Token kinds using X-macro pattern */
#define NOB_LEXER_TOKEN_KINDS           \
    /* Special */                        \
    X(NOB_LEXER_EOF)                     \
    X(NOB_LEXER_INVALID)                 \
    /* Literals */                       \
    X(NOB_LEXER_IDENT)                   \
    X(NOB_LEXER_INT)                     \
    X(NOB_LEXER_FLOAT)                   \
    X(NOB_LEXER_STRING)                  \
    X(NOB_LEXER_CHAR)                    \
    /* Comments */                       \
    X(NOB_LEXER_LINE_COMMENT)            \
    X(NOB_LEXER_BLOCK_COMMENT)           \
    /* Brackets */                       \
    X(NOB_LEXER_LPAREN)         /* ( */  \
    X(NOB_LEXER_RPAREN)         /* ) */  \
    X(NOB_LEXER_LBRACE)         /* { */  \
    X(NOB_LEXER_RBRACE)         /* } */  \
    X(NOB_LEXER_LBRACKET)       /* [ */  \
    X(NOB_LEXER_RBRACKET)       /* ] */  \
    /* Punctuation */                    \
    X(NOB_LEXER_SEMI)           /* ; */  \
    X(NOB_LEXER_COMMA)          /* , */  \
    X(NOB_LEXER_DOT)            /* . */  \
    X(NOB_LEXER_COLON)          /* : */  \
    X(NOB_LEXER_QUESTION)       /* ? */  \
    X(NOB_LEXER_TILDE)          /* ~ */  \
    X(NOB_LEXER_AT)             /* @ */  \
    X(NOB_LEXER_POUND)          /* # */  \
    X(NOB_LEXER_DOLLAR)         /* $ */  \
    X(NOB_LEXER_BACKSLASH)      /* \ */  \
    /* Operators */                      \
    X(NOB_LEXER_PLUS)           /* + */  \
    X(NOB_LEXER_MINUS)          /* - */  \
    X(NOB_LEXER_STAR)           /* * */  \
    X(NOB_LEXER_SLASH)          /* / */  \
    X(NOB_LEXER_PERCENT)        /* % */  \
    X(NOB_LEXER_AMP)            /* & */  \
    X(NOB_LEXER_PIPE)           /* | */  \
    X(NOB_LEXER_CARET)          /* ^ */  \
    X(NOB_LEXER_BANG)           /* ! */  \
    X(NOB_LEXER_LT)             /* < */  \
    X(NOB_LEXER_GT)             /* > */  \
    X(NOB_LEXER_EQ)             /* = */

/* Token kind enum */
typedef enum {
#define X(name) name,
    NOB_LEXER_TOKEN_KINDS
#undef X
    NOB_LEXER_TOKEN_COUNT
} Nob_Lexer_Token_Kind;

/* Token kind names array */
static const char *nob_lexer_token_kind_names[] = {
#define X(name) #name,
    NOB_LEXER_TOKEN_KINDS
#undef X
};

/* Location information */
typedef struct {
    const char *file;   /* Filename (not owned, just a pointer) */
    size_t line;        /* Line number (1-indexed) */
    size_t column;      /* Column number (1-indexed) */
} Nob_Lexer_Loc;

/* Single token */
typedef struct {
    Nob_Lexer_Token_Kind kind;
    Nob_String_View text;       /* Raw text view into original source */
    Nob_Lexer_Loc loc;

    /* Parsed values for numeric literals */
    union {
        int64_t int_value;      /* For NOB_LEXER_INT */
        double float_value;     /* For NOB_LEXER_FLOAT */
    } value;
} Nob_Lexer_Token;

/* Dynamic array of tokens */
typedef struct {
    Nob_Lexer_Token *items;
    size_t count;
    size_t capacity;
} Nob_Lexer_Tokens;

/* Lexer state */
typedef struct {
    Nob_String_View source;     /* Original source text */
    const char *file;           /* Filename for error messages */
    size_t pos;                 /* Current position in source */
    size_t line;                /* Current line (1-indexed) */
    size_t column;              /* Current column (1-indexed) */
} Nob_Lexer;

/* ============================================================
 * API Functions
 * ============================================================ */

/* Initialize lexer with source text and filename */
NOBLEXERDEF void nob_lexer_init(Nob_Lexer *lexer, Nob_String_View source, const char *file);

/* Get the next token (advances the lexer) */
NOBLEXERDEF Nob_Lexer_Token nob_lexer_next(Nob_Lexer *lexer);

/* Peek at the next token without consuming it */
NOBLEXERDEF Nob_Lexer_Token nob_lexer_peek(Nob_Lexer *lexer);

/* Tokenize entire source into a dynamic array of tokens */
NOBLEXERDEF void nob_lexer_tokenize(Nob_String_View source, const char *file,
                                     Nob_Lexer_Tokens *tokens);

/* Convert token kind to string */
NOBLEXERDEF const char *nob_lexer_token_kind_str(Nob_Lexer_Token_Kind kind);

/* Format location as "file:line:column" (uses temp allocator) */
NOBLEXERDEF const char *nob_lexer_loc_str(Nob_Lexer_Loc loc);

/* Unescape a string or char token into a string builder
 * Handles: \n \r \t \\ \" \' \0 \xNN
 * Returns false if escape sequence is invalid */
NOBLEXERDEF bool nob_lexer_unescape(Nob_Lexer_Token token, Nob_String_Builder *out);

/* ============================================================
 * Implementation
 * ============================================================ */

#ifdef NOB_LEXER_IMPLEMENTATION

#ifndef NOB_IMPLEMENTATION
#error "nob_lexer.h requires nob.h implementation"
#endif

/* Internal: Check if at end of source */
static bool nob_lexer__at_end(Nob_Lexer *lexer)
{
    return lexer->pos >= lexer->source.count;
}

/* Internal: Peek character at offset n from current position */
static char nob_lexer__peek(Nob_Lexer *lexer, size_t n)
{
    if (lexer->pos + n >= lexer->source.count) {
        return '\0';
    }
    return lexer->source.data[lexer->pos + n];
}

/* Internal: Get current character */
static char nob_lexer__current(Nob_Lexer *lexer)
{
    return nob_lexer__peek(lexer, 0);
}

/* Internal: Advance by one character, updating line/column */
static char nob_lexer__advance(Nob_Lexer *lexer)
{
    if (nob_lexer__at_end(lexer)) {
        return '\0';
    }

    char c = lexer->source.data[lexer->pos];
    lexer->pos++;

    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }

    return c;
}

/* Internal: Check if character is alphabetic or underscore
 * Also accepts UTF-8 continuation bytes (>= 128) for Unicode identifier support */
static bool nob_lexer__is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_' ||
           (unsigned char)c >= 128;
}

/* Internal: Check if character is a digit */
static bool nob_lexer__is_digit(char c)
{
    return c >= '0' && c <= '9';
}

/* Internal: Check if character is alphanumeric or underscore */
static bool nob_lexer__is_alnum(char c)
{
    return nob_lexer__is_alpha(c) || nob_lexer__is_digit(c);
}

/* Internal: Check if character is hex digit */
static bool nob_lexer__is_hex(char c)
{
    return nob_lexer__is_digit(c) ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

/* Internal: Check if character is binary digit */
static bool nob_lexer__is_binary(char c)
{
    return c == '0' || c == '1';
}

/* Internal: Check if character is octal digit */
static bool nob_lexer__is_octal(char c)
{
    return c >= '0' && c <= '7';
}

/* Internal: Check if character is whitespace */
static bool nob_lexer__is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' ||
           c == '\v' || c == '\f';
}

/* Internal: Skip whitespace */
static void nob_lexer__skip_whitespace(Nob_Lexer *lexer)
{
    while (!nob_lexer__at_end(lexer) && nob_lexer__is_whitespace(nob_lexer__current(lexer))) {
        nob_lexer__advance(lexer);
    }
}

/* Internal: Create a token */
static Nob_Lexer_Token nob_lexer__make_token(Nob_Lexer *lexer, Nob_Lexer_Token_Kind kind,
                                              size_t start_pos, size_t start_line, size_t start_col)
{
    Nob_Lexer_Token token = {0};
    token.kind = kind;
    token.text = nob_sv_from_parts(lexer->source.data + start_pos, lexer->pos - start_pos);
    token.loc.file = lexer->file;
    token.loc.line = start_line;
    token.loc.column = start_col;
    return token;
}

/* Internal: Lex an identifier */
static Nob_Lexer_Token nob_lexer__lex_ident(Nob_Lexer *lexer)
{
    size_t start_pos = lexer->pos;
    size_t start_line = lexer->line;
    size_t start_col = lexer->column;

    while (!nob_lexer__at_end(lexer) && nob_lexer__is_alnum(nob_lexer__current(lexer))) {
        nob_lexer__advance(lexer);
    }

    return nob_lexer__make_token(lexer, NOB_LEXER_IDENT, start_pos, start_line, start_col);
}

/* Internal: Lex a number (int or float) */
static Nob_Lexer_Token nob_lexer__lex_number(Nob_Lexer *lexer)
{
    size_t start_pos = lexer->pos;
    size_t start_line = lexer->line;
    size_t start_col = lexer->column;

    bool is_float = false;
    int base = 10;

    /* Check for base prefixes: 0x, 0b, 0o */
    if (nob_lexer__current(lexer) == '0') {
        char next = nob_lexer__peek(lexer, 1);
        if (next == 'x' || next == 'X') {
            base = 16;
            nob_lexer__advance(lexer); /* 0 */
            nob_lexer__advance(lexer); /* x */
            while (!nob_lexer__at_end(lexer) && nob_lexer__is_hex(nob_lexer__current(lexer))) {
                nob_lexer__advance(lexer);
            }

            /* C99 hex float: check for decimal point */
            if (nob_lexer__current(lexer) == '.') {
                is_float = true;
                nob_lexer__advance(lexer); /* . */
                while (!nob_lexer__at_end(lexer) && nob_lexer__is_hex(nob_lexer__current(lexer))) {
                    nob_lexer__advance(lexer);
                }
            }

            /* C99 hex float: binary exponent (p/P) - required for hex floats */
            char exp = nob_lexer__current(lexer);
            if (exp == 'p' || exp == 'P') {
                is_float = true;
                nob_lexer__advance(lexer); /* p */
                char sign = nob_lexer__current(lexer);
                if (sign == '+' || sign == '-') {
                    nob_lexer__advance(lexer);
                }
                while (!nob_lexer__at_end(lexer) && nob_lexer__is_digit(nob_lexer__current(lexer))) {
                    nob_lexer__advance(lexer);
                }
            }
        } else if (next == 'b' || next == 'B') {
            base = 2;
            nob_lexer__advance(lexer); /* 0 */
            nob_lexer__advance(lexer); /* b */
            while (!nob_lexer__at_end(lexer) && nob_lexer__is_binary(nob_lexer__current(lexer))) {
                nob_lexer__advance(lexer);
            }
        } else if (next == 'o' || next == 'O') {
            base = 8;
            nob_lexer__advance(lexer); /* 0 */
            nob_lexer__advance(lexer); /* o */
            while (!nob_lexer__at_end(lexer) && nob_lexer__is_octal(nob_lexer__current(lexer))) {
                nob_lexer__advance(lexer);
            }
        } else {
            /* Decimal number starting with 0, or just 0 */
            goto decimal;
        }
    } else {
decimal:
        /* Decimal integer part */
        while (!nob_lexer__at_end(lexer) && nob_lexer__is_digit(nob_lexer__current(lexer))) {
            nob_lexer__advance(lexer);
        }

        /* Check for decimal point */
        if (nob_lexer__current(lexer) == '.' && nob_lexer__is_digit(nob_lexer__peek(lexer, 1))) {
            is_float = true;
            nob_lexer__advance(lexer); /* . */
            while (!nob_lexer__at_end(lexer) && nob_lexer__is_digit(nob_lexer__current(lexer))) {
                nob_lexer__advance(lexer);
            }
        }

        /* Check for exponent */
        char c = nob_lexer__current(lexer);
        if (c == 'e' || c == 'E') {
            is_float = true;
            nob_lexer__advance(lexer); /* e */
            c = nob_lexer__current(lexer);
            if (c == '+' || c == '-') {
                nob_lexer__advance(lexer);
            }
            while (!nob_lexer__at_end(lexer) && nob_lexer__is_digit(nob_lexer__current(lexer))) {
                nob_lexer__advance(lexer);
            }
        }
    }

    Nob_Lexer_Token token = nob_lexer__make_token(
        lexer,
        is_float ? NOB_LEXER_FLOAT : NOB_LEXER_INT,
        start_pos, start_line, start_col
    );

    /* Parse the numeric value */
    /* C99 strtod handles hex floats (0x1.Fp+10) natively */
    const char *numstr = nob_temp_sv_to_cstr(token.text);

    if (is_float) {
        token.value.float_value = strtod(numstr, NULL);
    } else {
        if (base == 16) {
            token.value.int_value = strtoll(numstr + 2, NULL, 16); /* Skip 0x */
        } else if (base == 2) {
            token.value.int_value = strtoll(numstr + 2, NULL, 2);  /* Skip 0b */
        } else if (base == 8) {
            token.value.int_value = strtoll(numstr + 2, NULL, 8);  /* Skip 0o */
        } else {
            token.value.int_value = strtoll(numstr, NULL, 10);
        }
    }

    return token;
}

/* Internal: Lex a string literal */
static Nob_Lexer_Token nob_lexer__lex_string(Nob_Lexer *lexer)
{
    size_t start_pos = lexer->pos;
    size_t start_line = lexer->line;
    size_t start_col = lexer->column;

    nob_lexer__advance(lexer); /* Opening " */

    while (!nob_lexer__at_end(lexer)) {
        char c = nob_lexer__current(lexer);
        if (c == '"') {
            nob_lexer__advance(lexer); /* Closing " */
            return nob_lexer__make_token(lexer, NOB_LEXER_STRING, start_pos, start_line, start_col);
        }
        if (c == '\\') {
            nob_lexer__advance(lexer); /* Backslash */
            if (!nob_lexer__at_end(lexer)) {
                nob_lexer__advance(lexer); /* Escaped char */
            }
        } else if (c == '\n') {
            /* Unterminated string - newline before closing quote */
            nob_log(NOB_ERROR, "%s:%zu:%zu: Unterminated string literal",
                    lexer->file, start_line, start_col);
            return nob_lexer__make_token(lexer, NOB_LEXER_INVALID, start_pos, start_line, start_col);
        } else {
            nob_lexer__advance(lexer);
        }
    }

    /* Reached end of file without closing quote */
    nob_log(NOB_ERROR, "%s:%zu:%zu: Unterminated string literal",
            lexer->file, start_line, start_col);
    return nob_lexer__make_token(lexer, NOB_LEXER_INVALID, start_pos, start_line, start_col);
}

/* Internal: Lex a character literal */
static Nob_Lexer_Token nob_lexer__lex_char(Nob_Lexer *lexer)
{
    size_t start_pos = lexer->pos;
    size_t start_line = lexer->line;
    size_t start_col = lexer->column;

    nob_lexer__advance(lexer); /* Opening ' */

    while (!nob_lexer__at_end(lexer)) {
        char c = nob_lexer__current(lexer);
        if (c == '\'') {
            nob_lexer__advance(lexer); /* Closing ' */
            return nob_lexer__make_token(lexer, NOB_LEXER_CHAR, start_pos, start_line, start_col);
        }
        if (c == '\\') {
            nob_lexer__advance(lexer); /* Backslash */
            if (!nob_lexer__at_end(lexer)) {
                nob_lexer__advance(lexer); /* Escaped char */
            }
        } else if (c == '\n') {
            /* Unterminated char literal */
            nob_log(NOB_ERROR, "%s:%zu:%zu: Unterminated character literal",
                    lexer->file, start_line, start_col);
            return nob_lexer__make_token(lexer, NOB_LEXER_INVALID, start_pos, start_line, start_col);
        } else {
            nob_lexer__advance(lexer);
        }
    }

    /* Reached end of file without closing quote */
    nob_log(NOB_ERROR, "%s:%zu:%zu: Unterminated character literal",
            lexer->file, start_line, start_col);
    return nob_lexer__make_token(lexer, NOB_LEXER_INVALID, start_pos, start_line, start_col);
}

/* Internal: Lex a line comment (// ...) */
static Nob_Lexer_Token nob_lexer__lex_line_comment(Nob_Lexer *lexer)
{
    size_t start_pos = lexer->pos;
    size_t start_line = lexer->line;
    size_t start_col = lexer->column;

    nob_lexer__advance(lexer); /* First / */
    nob_lexer__advance(lexer); /* Second / */

    while (!nob_lexer__at_end(lexer) && nob_lexer__current(lexer) != '\n') {
        nob_lexer__advance(lexer);
    }

    return nob_lexer__make_token(lexer, NOB_LEXER_LINE_COMMENT, start_pos, start_line, start_col);
}

/* Internal: Lex a block comment */
static Nob_Lexer_Token nob_lexer__lex_block_comment(Nob_Lexer *lexer)
{
    size_t start_pos = lexer->pos;
    size_t start_line = lexer->line;
    size_t start_col = lexer->column;

    nob_lexer__advance(lexer); /* / */
    nob_lexer__advance(lexer); /* * */

    while (!nob_lexer__at_end(lexer)) {
        if (nob_lexer__current(lexer) == '*' && nob_lexer__peek(lexer, 1) == '/') {
            nob_lexer__advance(lexer); /* * */
            nob_lexer__advance(lexer); /* / */
            return nob_lexer__make_token(lexer, NOB_LEXER_BLOCK_COMMENT, start_pos, start_line, start_col);
        }
        nob_lexer__advance(lexer);
    }

    /* Unterminated block comment */
    nob_log(NOB_ERROR, "%s:%zu:%zu: Unterminated block comment",
            lexer->file, start_line, start_col);
    return nob_lexer__make_token(lexer, NOB_LEXER_INVALID, start_pos, start_line, start_col);
}

/* Initialize lexer */
NOBLEXERDEF void nob_lexer_init(Nob_Lexer *lexer, Nob_String_View source, const char *file)
{
    lexer->source = source;
    lexer->file = file ? file : "<unknown>";
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
}

/* Get next token */
NOBLEXERDEF Nob_Lexer_Token nob_lexer_next(Nob_Lexer *lexer)
{
    nob_lexer__skip_whitespace(lexer);

    if (nob_lexer__at_end(lexer)) {
        Nob_Lexer_Token token = {0};
        token.kind = NOB_LEXER_EOF;
        token.text = nob_sv_from_parts(lexer->source.data + lexer->pos, 0);
        token.loc.file = lexer->file;
        token.loc.line = lexer->line;
        token.loc.column = lexer->column;
        return token;
    }

    size_t start_pos = lexer->pos;
    size_t start_line = lexer->line;
    size_t start_col = lexer->column;

    char c = nob_lexer__current(lexer);

    /* Identifier or keyword */
    if (nob_lexer__is_alpha(c)) {
        return nob_lexer__lex_ident(lexer);
    }

    /* Number */
    if (nob_lexer__is_digit(c)) {
        return nob_lexer__lex_number(lexer);
    }

    /* String literal */
    if (c == '"') {
        return nob_lexer__lex_string(lexer);
    }

    /* Character literal */
    if (c == '\'') {
        return nob_lexer__lex_char(lexer);
    }

    /* Slash - could be comment or operator */
    if (c == '/') {
        char next = nob_lexer__peek(lexer, 1);
        if (next == '/') {
            return nob_lexer__lex_line_comment(lexer);
        }
        if (next == '*') {
            return nob_lexer__lex_block_comment(lexer);
        }
        nob_lexer__advance(lexer);
        return nob_lexer__make_token(lexer, NOB_LEXER_SLASH, start_pos, start_line, start_col);
    }

    /* Single-character tokens */
    nob_lexer__advance(lexer);

    Nob_Lexer_Token_Kind kind;
    switch (c) {
        case '(': kind = NOB_LEXER_LPAREN; break;
        case ')': kind = NOB_LEXER_RPAREN; break;
        case '{': kind = NOB_LEXER_LBRACE; break;
        case '}': kind = NOB_LEXER_RBRACE; break;
        case '[': kind = NOB_LEXER_LBRACKET; break;
        case ']': kind = NOB_LEXER_RBRACKET; break;
        case ';': kind = NOB_LEXER_SEMI; break;
        case ',': kind = NOB_LEXER_COMMA; break;
        case '.': kind = NOB_LEXER_DOT; break;
        case ':': kind = NOB_LEXER_COLON; break;
        case '?': kind = NOB_LEXER_QUESTION; break;
        case '~': kind = NOB_LEXER_TILDE; break;
        case '@': kind = NOB_LEXER_AT; break;
        case '#': kind = NOB_LEXER_POUND; break;
        case '$': kind = NOB_LEXER_DOLLAR; break;
        case '\\': kind = NOB_LEXER_BACKSLASH; break;
        case '+': kind = NOB_LEXER_PLUS; break;
        case '-': kind = NOB_LEXER_MINUS; break;
        case '*': kind = NOB_LEXER_STAR; break;
        case '%': kind = NOB_LEXER_PERCENT; break;
        case '&': kind = NOB_LEXER_AMP; break;
        case '|': kind = NOB_LEXER_PIPE; break;
        case '^': kind = NOB_LEXER_CARET; break;
        case '!': kind = NOB_LEXER_BANG; break;
        case '<': kind = NOB_LEXER_LT; break;
        case '>': kind = NOB_LEXER_GT; break;
        case '=': kind = NOB_LEXER_EQ; break;
        default:
            kind = NOB_LEXER_INVALID;
            break;
    }

    return nob_lexer__make_token(lexer, kind, start_pos, start_line, start_col);
}

/* Peek at next token without consuming */
NOBLEXERDEF Nob_Lexer_Token nob_lexer_peek(Nob_Lexer *lexer)
{
    /* Save state */
    size_t saved_pos = lexer->pos;
    size_t saved_line = lexer->line;
    size_t saved_column = lexer->column;

    Nob_Lexer_Token token = nob_lexer_next(lexer);

    /* Restore state */
    lexer->pos = saved_pos;
    lexer->line = saved_line;
    lexer->column = saved_column;

    return token;
}

/* Tokenize entire source */
NOBLEXERDEF void nob_lexer_tokenize(Nob_String_View source, const char *file,
                                     Nob_Lexer_Tokens *tokens)
{
    Nob_Lexer lexer;
    nob_lexer_init(&lexer, source, file);

    for (;;) {
        Nob_Lexer_Token token = nob_lexer_next(&lexer);

#ifdef NOB_LEXER_SKIP_COMMENTS
        if (token.kind == NOB_LEXER_LINE_COMMENT || token.kind == NOB_LEXER_BLOCK_COMMENT) {
            continue;
        }
#endif

        nob_da_append(tokens, token);

        if (token.kind == NOB_LEXER_EOF) {
            break;
        }
    }
}

/* Convert token kind to string */
NOBLEXERDEF const char *nob_lexer_token_kind_str(Nob_Lexer_Token_Kind kind)
{
    if (kind >= 0 && kind < NOB_LEXER_TOKEN_COUNT) {
        return nob_lexer_token_kind_names[kind];
    }
    return "UNKNOWN";
}

/* Format location as string */
NOBLEXERDEF const char *nob_lexer_loc_str(Nob_Lexer_Loc loc)
{
    return nob_temp_sprintf("%s:%zu:%zu", loc.file, loc.line, loc.column);
}

/* Unescape a string or char token */
NOBLEXERDEF bool nob_lexer_unescape(Nob_Lexer_Token token, Nob_String_Builder *out)
{
    if (token.kind != NOB_LEXER_STRING && token.kind != NOB_LEXER_CHAR) {
        return false;
    }

    /* Skip the opening quote */
    if (token.text.count < 2) {
        return false;
    }

    const char *start = token.text.data + 1;
    const char *end = token.text.data + token.text.count - 1;

    while (start < end) {
        if (*start == '\\' && start + 1 < end) {
            start++;
            switch (*start) {
                case 'n':  nob_da_append(out, '\n'); break;
                case 'r':  nob_da_append(out, '\r'); break;
                case 't':  nob_da_append(out, '\t'); break;
                case '\\': nob_da_append(out, '\\'); break;
                case '"':  nob_da_append(out, '"'); break;
                case '\'': nob_da_append(out, '\''); break;
                case '0':  nob_da_append(out, '\0'); break;
                case 'x': {
                    /* Hex escape: \xNN */
                    if (start + 2 < end &&
                        nob_lexer__is_hex(start[1]) &&
                        nob_lexer__is_hex(start[2])) {
                        char hex[3] = {start[1], start[2], '\0'};
                        char value = (char)strtol(hex, NULL, 16);
                        nob_da_append(out, value);
                        start += 2;
                    } else {
                        /* Invalid hex escape */
                        return false;
                    }
                } break;
                default:
                    /* Unknown escape - include as-is */
                    nob_da_append(out, *start);
                    break;
            }
        } else {
            nob_da_append(out, *start);
        }
        start++;
    }

    return true;
}

#endif /* NOB_LEXER_IMPLEMENTATION */

/* ============================================================
 * Strip Prefix
 * ============================================================ */

#ifdef NOB_LEXER_STRIP_PREFIX

/* Token kinds */
#define LEXER_EOF           NOB_LEXER_EOF
#define LEXER_INVALID       NOB_LEXER_INVALID
#define LEXER_IDENT         NOB_LEXER_IDENT
#define LEXER_INT           NOB_LEXER_INT
#define LEXER_FLOAT         NOB_LEXER_FLOAT
#define LEXER_STRING        NOB_LEXER_STRING
#define LEXER_CHAR          NOB_LEXER_CHAR
#define LEXER_LINE_COMMENT  NOB_LEXER_LINE_COMMENT
#define LEXER_BLOCK_COMMENT NOB_LEXER_BLOCK_COMMENT
#define LEXER_LPAREN        NOB_LEXER_LPAREN
#define LEXER_RPAREN        NOB_LEXER_RPAREN
#define LEXER_LBRACE        NOB_LEXER_LBRACE
#define LEXER_RBRACE        NOB_LEXER_RBRACE
#define LEXER_LBRACKET      NOB_LEXER_LBRACKET
#define LEXER_RBRACKET      NOB_LEXER_RBRACKET
#define LEXER_SEMI          NOB_LEXER_SEMI
#define LEXER_COMMA         NOB_LEXER_COMMA
#define LEXER_DOT           NOB_LEXER_DOT
#define LEXER_COLON         NOB_LEXER_COLON
#define LEXER_QUESTION      NOB_LEXER_QUESTION
#define LEXER_TILDE         NOB_LEXER_TILDE
#define LEXER_AT            NOB_LEXER_AT
#define LEXER_POUND         NOB_LEXER_POUND
#define LEXER_DOLLAR        NOB_LEXER_DOLLAR
#define LEXER_BACKSLASH     NOB_LEXER_BACKSLASH
#define LEXER_PLUS          NOB_LEXER_PLUS
#define LEXER_MINUS         NOB_LEXER_MINUS
#define LEXER_STAR          NOB_LEXER_STAR
#define LEXER_SLASH         NOB_LEXER_SLASH
#define LEXER_PERCENT       NOB_LEXER_PERCENT
#define LEXER_AMP           NOB_LEXER_AMP
#define LEXER_PIPE          NOB_LEXER_PIPE
#define LEXER_CARET         NOB_LEXER_CARET
#define LEXER_BANG          NOB_LEXER_BANG
#define LEXER_LT            NOB_LEXER_LT
#define LEXER_GT            NOB_LEXER_GT
#define LEXER_EQ            NOB_LEXER_EQ

/* Types */
typedef Nob_Lexer_Token_Kind Lexer_Token_Kind;
typedef Nob_Lexer_Loc        Lexer_Loc;
typedef Nob_Lexer_Token      Lexer_Token;
typedef Nob_Lexer_Tokens     Lexer_Tokens;
typedef Nob_Lexer            Lexer;

/* Functions */
#define lexer_init           nob_lexer_init
#define lexer_next           nob_lexer_next
#define lexer_peek           nob_lexer_peek
#define lexer_tokenize       nob_lexer_tokenize
#define lexer_token_kind_str nob_lexer_token_kind_str
#define lexer_loc_str        nob_lexer_loc_str
#define lexer_unescape       nob_lexer_unescape

#endif /* NOB_LEXER_STRIP_PREFIX */

#endif /* NOB_LEXER_H_ */
