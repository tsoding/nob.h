#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>

#define NOB_IMPLEMENTATION
#include "nob.h"

void sv_assert_eq_Nob_String_View(const char *file, size_t line,
                              const char *expected_expr, Nob_String_View expected,
                              const char *actual_expr, Nob_String_View actual)
{
    if (!nob_sv_eq(expected, actual)) {
        fprintf(stderr, "%s:%zu: FAILED: %s == %s\n",
                file, line, expected_expr, actual_expr);
        fprintf(stderr, "  EXPECTED: " SV_Fmt "\n", SV_Arg(expected));
        fprintf(stderr, "  ACTUAL:   " SV_Fmt "\n", SV_Arg(actual));
        exit(1);
    }
}

void sv_assert_eq_size_t(const char *file, size_t line,
                         const char *expected_expr, size_t expected,
                         const char *actual_expr, size_t actual)
{
    if (expected != actual) {
        fprintf(stderr, "%s:%zu: FAILED: %s == %s\n",
                file, line, expected_expr, actual_expr);
        fprintf(stderr, "  EXPECTED: %zu\n", expected);
        fprintf(stderr, "  ACTUAL:   %zu\n", actual);
        exit(1);
    }
}

void sv_assert_eq_uint64_t(const char *file, size_t line,
                           const char *expected_expr, uint64_t expected,
                           const char *actual_expr, uint64_t actual)
{
    if (expected != actual) {
        fprintf(stderr, "%s:%zu: FAILED: %s == %s\n",
                file, line, expected_expr, actual_expr);
        fprintf(stderr, "  EXPECTED: %" PRIu64 "\n", expected);
        fprintf(stderr, "  ACTUAL:   %" PRIu64 "\n", actual);
        exit(1);
    }
}

#define ASSERT_EQ(type, expected, actual) sv_assert_eq_##type(__FILE__, __LINE__, #expected, expected, #actual, actual)

void sv_assert_true(const char *file, size_t line,
                    const char *expression_cstr,
                    bool expression)
{
    if (!expression) {
        fprintf(stderr, "%s:%zu: FAILED: %s\n", file, line, expression_cstr);
        exit(1);
    }
}

#define ASSERT_TRUE(expression) sv_assert_true(__FILE__, __LINE__, #expression, expression);

bool is_alpha(char x)
{
    return isalpha(x);
}

int main(void)
{
    // Construct
    {
        ASSERT_EQ(Nob_String_View, SV("Foo"), nob_sv_from_cstr("Foo"));
    }

    // Trimming Whitespace
    {
        ASSERT_EQ(Nob_String_View, SV("hello    "), nob_sv_trim_left(SV("    hello    ")));
        ASSERT_EQ(Nob_String_View, SV("    hello"), nob_sv_trim_right(SV("    hello    ")));
        ASSERT_EQ(Nob_String_View, SV("hello"), nob_sv_trim(SV("    hello    ")));
    }

    // Chop by delimiter
    {
        // Existing
        {
            Nob_String_View input = SV_STATIC("hello\nworld");
            Nob_String_View line = nob_sv_chop_by_delim(&input, '\n');
            ASSERT_EQ(Nob_String_View, SV("hello"), line);
            ASSERT_EQ(Nob_String_View, SV("world"), input);
        }

        // Non-Existing
        {
            Nob_String_View input = SV_STATIC("hello\nworld");
            Nob_String_View line = nob_sv_chop_by_delim(&input, ' ');
            ASSERT_EQ(Nob_String_View, SV("hello\nworld"), line);
            ASSERT_EQ(Nob_String_View, SV(""), input);
        }
    }

    // Chop by Nob_String_View (thicc delimiter)
    {
        // Existing
        {
            Nob_String_View input = SV_STATIC("hello\nworld\ngoodbye");
            Nob_String_View line = nob_sv_chop_by_sv(&input, SV("\nwor"));
            ASSERT_EQ(Nob_String_View, SV("hello"), line);
            ASSERT_EQ(Nob_String_View, SV("ld\ngoodbye"), input);
        }

        // Non-Existing
        {
            Nob_String_View input = SV_STATIC("hello\nworld");
            Nob_String_View line  = nob_sv_chop_by_sv(&input, SV("goodbye"));
            ASSERT_EQ(Nob_String_View, SV("hello\nworld"), line);
            ASSERT_EQ(Nob_String_View, SV(""), input);
        }
    }

    // Try to chop by delimiter
    {
        // Existing
        {
            Nob_String_View input = SV_STATIC("hello\nworld");
            Nob_String_View line = SV_NULL;
            bool result = nob_sv_try_chop_by_delim(&input, '\n', &line);
            ASSERT_TRUE(result);
            ASSERT_EQ(Nob_String_View, SV("hello"), line);
            ASSERT_EQ(Nob_String_View, SV("world"), input);
        }

        // Non-Existing
        {
            Nob_String_View input = SV_STATIC("hello\nworld");
            Nob_String_View line = SV_NULL;
            bool result = nob_sv_try_chop_by_delim(&input, ' ', &line);
            ASSERT_TRUE(!result);
            ASSERT_EQ(Nob_String_View, SV(""), line);
            ASSERT_EQ(Nob_String_View, SV("hello\nworld"), input);
        }
    }

    // Chop N characters
    {
        // Chop left
        {
            Nob_String_View input = SV_STATIC("hello");
            Nob_String_View hell = nob_sv_chop_left(&input, 4);
            ASSERT_EQ(Nob_String_View, SV("o"), input);
            ASSERT_EQ(Nob_String_View, SV("hell"), hell);
        }

        // Overchop left
        {
            Nob_String_View input = SV_STATIC("hello");
            Nob_String_View hell = nob_sv_chop_left(&input, 10);
            ASSERT_EQ(Nob_String_View, SV(""), input);
            ASSERT_EQ(Nob_String_View, SV("hello"), hell);
        }

        // Chop right
        {
            Nob_String_View input = SV_STATIC("hello");
            Nob_String_View hell = nob_sv_chop_right(&input, 4);
            ASSERT_EQ(Nob_String_View, SV("h"), input);
            ASSERT_EQ(Nob_String_View, SV("ello"), hell);
        }

        // Overchop right
        {
            Nob_String_View input = SV_STATIC("hello");
            Nob_String_View hell = nob_sv_chop_right(&input, 10);
            ASSERT_EQ(Nob_String_View, SV(""), input);
            ASSERT_EQ(Nob_String_View, SV("hello"), hell);
        }
    }

    // Take while
    {
        // Take while is_alpha
        {
            Nob_String_View input = SV_STATIC("hello1234");
            Nob_String_View hello = nob_sv_take_left_while(input, is_alpha);
            ASSERT_EQ(Nob_String_View, SV("hello1234"), input);
            ASSERT_EQ(Nob_String_View, SV("hello"), hello);
        }

        // Overtake while
        {
            Nob_String_View input = SV_STATIC("helloworld");
            Nob_String_View hello = nob_sv_take_left_while(input, is_alpha);
            ASSERT_EQ(Nob_String_View, SV("helloworld"), input);
            ASSERT_EQ(Nob_String_View, SV("helloworld"), hello);
        }
    }

    // Chop while
    {
        // Chop while is_alpha
        {
            Nob_String_View input = SV_STATIC("hello1234");
            Nob_String_View hello = nob_sv_chop_left_while(&input, is_alpha);
            ASSERT_EQ(Nob_String_View, SV("1234"), input);
            ASSERT_EQ(Nob_String_View, SV("hello"), hello);
        }

        // Overchop while
        {
            Nob_String_View input = SV_STATIC("helloworld");
            Nob_String_View hello = nob_sv_chop_left_while(&input, is_alpha);
            ASSERT_EQ(Nob_String_View, SV(""), input);
            ASSERT_EQ(Nob_String_View, SV("helloworld"), hello);
        }
    }

    // Equals, ignoring case
    {
        // exactly equal
        {
            Nob_String_View input = SV_STATIC("hello, world");
            ASSERT_TRUE(nob_sv_eq_ignorecase(input, SV("hello, world")));
        }

        // equal ignoring case
        {
            Nob_String_View input = SV_STATIC("Hello, World");
            ASSERT_TRUE(nob_sv_eq_ignorecase(input, SV("hello, world")));
        }

        // unequal
        {
            Nob_String_View input = SV_STATIC("Goodbye, World");
            ASSERT_TRUE(!(nob_sv_eq_ignorecase(input, SV("Hello, World"))));
        }
    }

    // Index of
    {
        size_t index = 0;
        ASSERT_TRUE(nob_sv_index_of(SV("hello world"), ' ', &index));
        ASSERT_EQ(size_t, 5, index);
    }

    // Prefix/suffix check
    {
        ASSERT_TRUE(nob_sv_starts_with(SV("Hello, World"), SV("Hello")));
        ASSERT_TRUE(nob_sv_end_with(SV("Hello, World"), "World"));
    }

    // To Integer
    {
        Nob_String_View input = SV_STATIC("1234567890");

        ASSERT_EQ(uint64_t, 1234567890, nob_sv_to_u64(input));
        ASSERT_EQ(Nob_String_View, input, SV("1234567890"));

        ASSERT_EQ(uint64_t, 1234567890, nob_sv_chop_u64(&input));
        ASSERT_TRUE(input.count == 0);
    }

    printf("OK\n");

    return 0;
}
