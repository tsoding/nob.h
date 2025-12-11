/* Test for nob_lexer.h */
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "../nob.h"

#define NOB_LEXER_IMPLEMENTATION
#include "../nob_lexer.h"

#include <string.h>

#define TEST(name) nob_log(INFO, "TEST: %s", name)
#define ASSERT(cond) do { \
    if (!(cond)) { \
        nob_log(ERROR, "  FAIL: %s:%d: %s", __FILE__, __LINE__, #cond); \
        return 1; \
    } \
} while(0)
#define ASSERT_EQ_INT(a, b) do { \
    long long _a = (a), _b = (b); \
    if (_a != _b) { \
        nob_log(ERROR, "  FAIL: %s:%d: %s == %lld, expected %lld", __FILE__, __LINE__, #a, _a, _b); \
        return 1; \
    } \
} while(0)
#define ASSERT_EQ_STR(a, b) do { \
    const char *_a = (a), *_b = (b); \
    if (strcmp(_a, _b) != 0) { \
        nob_log(ERROR, "  FAIL: %s:%d: %s == \"%s\", expected \"%s\"", __FILE__, __LINE__, #a, _a, _b); \
        return 1; \
    } \
} while(0)

/* Test basic token types */
static int test_basic_tokens(void)
{
    TEST("basic_tokens");

    const char *src = "( ) { } [ ] ; , . : ? ~ @ # $ \\ + - * / % & | ^ ! < > =";
    Nob_Lexer_Tokens tokens = {0};
    nob_lexer_tokenize(nob_sv_from_cstr(src), "test", &tokens);

    Nob_Lexer_Token_Kind expected[] = {
        NOB_LEXER_LPAREN, NOB_LEXER_RPAREN,
        NOB_LEXER_LBRACE, NOB_LEXER_RBRACE,
        NOB_LEXER_LBRACKET, NOB_LEXER_RBRACKET,
        NOB_LEXER_SEMI, NOB_LEXER_COMMA, NOB_LEXER_DOT,
        NOB_LEXER_COLON, NOB_LEXER_QUESTION, NOB_LEXER_TILDE,
        NOB_LEXER_AT, NOB_LEXER_POUND, NOB_LEXER_DOLLAR, NOB_LEXER_BACKSLASH,
        NOB_LEXER_PLUS, NOB_LEXER_MINUS, NOB_LEXER_STAR, NOB_LEXER_SLASH,
        NOB_LEXER_PERCENT, NOB_LEXER_AMP, NOB_LEXER_PIPE, NOB_LEXER_CARET,
        NOB_LEXER_BANG, NOB_LEXER_LT, NOB_LEXER_GT, NOB_LEXER_EQ,
        NOB_LEXER_EOF
    };

    ASSERT_EQ_INT(tokens.count, sizeof(expected)/sizeof(expected[0]));

    for (size_t i = 0; i < tokens.count; i++) {
        ASSERT_EQ_INT(tokens.items[i].kind, expected[i]);
    }

    nob_da_free(tokens);
    nob_log(INFO, "  PASS");
    return 0;
}

/* Test identifier lexing */
static int test_identifiers(void)
{
    TEST("identifiers");

    const char *src = "foo bar _test __init__ camelCase UPPER_CASE x123 _";
    Nob_Lexer_Tokens tokens = {0};
    nob_lexer_tokenize(nob_sv_from_cstr(src), "test", &tokens);

    const char *expected[] = {
        "foo", "bar", "_test", "__init__", "camelCase", "UPPER_CASE", "x123", "_"
    };

    ASSERT_EQ_INT(tokens.count, sizeof(expected)/sizeof(expected[0]) + 1); /* +1 for EOF */

    for (size_t i = 0; i < sizeof(expected)/sizeof(expected[0]); i++) {
        ASSERT_EQ_INT(tokens.items[i].kind, NOB_LEXER_IDENT);
        ASSERT_EQ_INT(tokens.items[i].text.count, strlen(expected[i]));
        ASSERT(memcmp(tokens.items[i].text.data, expected[i], tokens.items[i].text.count) == 0);
    }

    nob_da_free(tokens);
    nob_log(INFO, "  PASS");
    return 0;
}

/* Test integer parsing */
static int test_integers(void)
{
    TEST("integers");

    const char *src = "0 123 0xFF 0xff 0b1010 0B1111 0o755 0O777";
    Nob_Lexer_Tokens tokens = {0};
    nob_lexer_tokenize(nob_sv_from_cstr(src), "test", &tokens);

    int64_t expected[] = {0, 123, 255, 255, 10, 15, 493, 511};

    ASSERT_EQ_INT(tokens.count, sizeof(expected)/sizeof(expected[0]) + 1); /* +1 for EOF */

    for (size_t i = 0; i < sizeof(expected)/sizeof(expected[0]); i++) {
        ASSERT_EQ_INT(tokens.items[i].kind, NOB_LEXER_INT);
        ASSERT_EQ_INT(tokens.items[i].value.int_value, expected[i]);
    }

    nob_da_free(tokens);
    nob_log(INFO, "  PASS");
    return 0;
}

/* Test float parsing */
static int test_floats(void)
{
    TEST("floats");

    const char *src = "3.14 0.5 1e10 2e-3 1.5e+2";
    Nob_Lexer_Tokens tokens = {0};
    nob_lexer_tokenize(nob_sv_from_cstr(src), "test", &tokens);

    double expected[] = {3.14, 0.5, 1e10, 2e-3, 1.5e+2};

    ASSERT_EQ_INT(tokens.count, sizeof(expected)/sizeof(expected[0]) + 1); /* +1 for EOF */

    for (size_t i = 0; i < sizeof(expected)/sizeof(expected[0]); i++) {
        ASSERT_EQ_INT(tokens.items[i].kind, NOB_LEXER_FLOAT);
        /* Use approximate comparison for floats */
        double diff = tokens.items[i].value.float_value - expected[i];
        if (diff < 0) diff = -diff;
        ASSERT(diff < 0.0001 || diff / expected[i] < 0.0001);
    }

    nob_da_free(tokens);
    nob_log(INFO, "  PASS");
    return 0;
}

/* Test string literals */
static int test_strings(void)
{
    TEST("strings");

    const char *src = "\"hello\" \"world\" \"with spaces\" \"with\\nnewline\"";
    Nob_Lexer_Tokens tokens = {0};
    nob_lexer_tokenize(nob_sv_from_cstr(src), "test", &tokens);

    ASSERT_EQ_INT(tokens.count, 5); /* 4 strings + EOF */

    for (size_t i = 0; i < 4; i++) {
        ASSERT_EQ_INT(tokens.items[i].kind, NOB_LEXER_STRING);
    }

    /* Test unescape */
    Nob_String_Builder sb = {0};
    ASSERT(nob_lexer_unescape(tokens.items[3], &sb));
    nob_sb_append_null(&sb);
    ASSERT_EQ_STR(sb.items, "with\nnewline");
    nob_sb_free(sb);

    nob_da_free(tokens);
    nob_log(INFO, "  PASS");
    return 0;
}

/* Test character literals */
static int test_chars(void)
{
    TEST("chars");

    const char *src = "'a' 'b' '\\n' '\\t' '\\x41'";
    Nob_Lexer_Tokens tokens = {0};
    nob_lexer_tokenize(nob_sv_from_cstr(src), "test", &tokens);

    ASSERT_EQ_INT(tokens.count, 6); /* 5 chars + EOF */

    for (size_t i = 0; i < 5; i++) {
        ASSERT_EQ_INT(tokens.items[i].kind, NOB_LEXER_CHAR);
    }

    /* Test unescape for '\x41' which should be 'A' */
    Nob_String_Builder sb = {0};
    ASSERT(nob_lexer_unescape(tokens.items[4], &sb));
    ASSERT_EQ_INT(sb.count, 1);
    ASSERT_EQ_INT(sb.items[0], 'A');
    nob_sb_free(sb);

    nob_da_free(tokens);
    nob_log(INFO, "  PASS");
    return 0;
}

/* Test comments */
static int test_comments(void)
{
    TEST("comments");

    const char *src = "// line comment\n/* block comment */ int x;";
    Nob_Lexer_Tokens tokens = {0};
    nob_lexer_tokenize(nob_sv_from_cstr(src), "test", &tokens);

    ASSERT_EQ_INT(tokens.items[0].kind, NOB_LEXER_LINE_COMMENT);
    ASSERT_EQ_INT(tokens.items[1].kind, NOB_LEXER_BLOCK_COMMENT);
    ASSERT_EQ_INT(tokens.items[2].kind, NOB_LEXER_IDENT); /* int */
    ASSERT_EQ_INT(tokens.items[3].kind, NOB_LEXER_IDENT); /* x */
    ASSERT_EQ_INT(tokens.items[4].kind, NOB_LEXER_SEMI);
    ASSERT_EQ_INT(tokens.items[5].kind, NOB_LEXER_EOF);

    nob_da_free(tokens);
    nob_log(INFO, "  PASS");
    return 0;
}

/* Test location tracking */
static int test_locations(void)
{
    TEST("locations");

    const char *src = "int\n  x\n    y";
    Nob_Lexer_Tokens tokens = {0};
    nob_lexer_tokenize(nob_sv_from_cstr(src), "testfile.c", &tokens);

    ASSERT_EQ_INT(tokens.count, 4); /* int, x, y, EOF */

    /* int at line 1, column 1 */
    ASSERT_EQ_INT(tokens.items[0].loc.line, 1);
    ASSERT_EQ_INT(tokens.items[0].loc.column, 1);
    ASSERT_EQ_STR(tokens.items[0].loc.file, "testfile.c");

    /* x at line 2, column 3 */
    ASSERT_EQ_INT(tokens.items[1].loc.line, 2);
    ASSERT_EQ_INT(tokens.items[1].loc.column, 3);

    /* y at line 3, column 5 */
    ASSERT_EQ_INT(tokens.items[2].loc.line, 3);
    ASSERT_EQ_INT(tokens.items[2].loc.column, 5);

    nob_da_free(tokens);
    nob_log(INFO, "  PASS");
    return 0;
}

/* Test peek functionality */
static int test_peek(void)
{
    TEST("peek");

    const char *src = "a b c";
    Nob_Lexer lexer;
    nob_lexer_init(&lexer, nob_sv_from_cstr(src), "test");

    /* Peek should not advance */
    Nob_Lexer_Token t1 = nob_lexer_peek(&lexer);
    Nob_Lexer_Token t2 = nob_lexer_peek(&lexer);
    ASSERT_EQ_INT(t1.kind, NOB_LEXER_IDENT);
    ASSERT_EQ_INT(t2.kind, NOB_LEXER_IDENT);
    ASSERT(t1.text.data == t2.text.data); /* Same token */

    /* Next should advance */
    Nob_Lexer_Token t3 = nob_lexer_next(&lexer);
    Nob_Lexer_Token t4 = nob_lexer_next(&lexer);
    ASSERT(t3.text.data[0] == 'a');
    ASSERT(t4.text.data[0] == 'b');

    nob_log(INFO, "  PASS");
    return 0;
}

/* Test invalid tokens */
static int test_invalid(void)
{
    TEST("invalid");

    const char *src = "hello ` world";
    Nob_Lexer_Tokens tokens = {0};
    nob_lexer_tokenize(nob_sv_from_cstr(src), "test", &tokens);

    ASSERT_EQ_INT(tokens.items[0].kind, NOB_LEXER_IDENT);  /* hello */
    ASSERT_EQ_INT(tokens.items[1].kind, NOB_LEXER_INVALID); /* ` */
    ASSERT_EQ_INT(tokens.items[2].kind, NOB_LEXER_IDENT);  /* world */
    ASSERT_EQ_INT(tokens.items[3].kind, NOB_LEXER_EOF);

    nob_da_free(tokens);
    nob_log(INFO, "  PASS");
    return 0;
}

/* Test token kind to string */
static int test_token_kind_str(void)
{
    TEST("token_kind_str");

    ASSERT_EQ_STR(nob_lexer_token_kind_str(NOB_LEXER_EOF), "NOB_LEXER_EOF");
    ASSERT_EQ_STR(nob_lexer_token_kind_str(NOB_LEXER_IDENT), "NOB_LEXER_IDENT");
    ASSERT_EQ_STR(nob_lexer_token_kind_str(NOB_LEXER_INT), "NOB_LEXER_INT");
    ASSERT_EQ_STR(nob_lexer_token_kind_str(NOB_LEXER_LPAREN), "NOB_LEXER_LPAREN");

    nob_log(INFO, "  PASS");
    return 0;
}

/* Test loc_str helper */
static int test_loc_str(void)
{
    TEST("loc_str");

    Nob_Lexer_Loc loc = {.file = "test.c", .line = 42, .column = 10};
    const char *str = nob_lexer_loc_str(loc);
    ASSERT_EQ_STR(str, "test.c:42:10");

    nob_log(INFO, "  PASS");
    return 0;
}

int main(void)
{
    int result = 0;

    result |= test_basic_tokens();
    result |= test_identifiers();
    result |= test_integers();
    result |= test_floats();
    result |= test_strings();
    result |= test_chars();
    result |= test_comments();
    result |= test_locations();
    result |= test_peek();
    result |= test_invalid();
    result |= test_token_kind_str();
    result |= test_loc_str();

    if (result == 0) {
        nob_log(INFO, "All tests passed!");
    } else {
        nob_log(ERROR, "Some tests failed!");
    }

    return result;
}
