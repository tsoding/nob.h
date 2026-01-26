#define NOB_IMPLEMENTATION
#include "nob.h"


#define stringify_impl(x) #x
#define stringify(x) stringify_impl(x)
#define test(x) do{ if(!(x)){ nob_log(NOB_ERROR, "TEST FAILED in file " __FILE__ " on line " stringify(__LINE__) " with expression `" stringify(x) "'."); } }while(false)


static char const* const k_strings[] =
{
	"Здравствуйте", /* Russian */
	"Γεια σας", /* Greek */
	"안녕", /* Korean */
	"こんにちは", /* Japanese */
	"您好", /* Chinese */
};


static void test_unicode_utf8_printf(void)
{
	int n;
	int i;

	nob_log(NOB_INFO, "%s", "Testing log...");
	n = NOB_ARRAY_LEN(k_strings);
	for(i = 0; i != n; ++i)
	{
		nob_log(NOB_INFO, "%s", k_strings[i]);
	}
}

static void test_unicode_utf8_dir(void)
{
	int n;
	int i;
	bool b;
	size_t mark;
	char const* curr_dir;

	nob_log(NOB_INFO, "%s", "Testing mkdir...");
	n = NOB_ARRAY_LEN(k_strings);
	for(i = 0; i != n; ++i)
	{
		b = nob_mkdir_if_not_exists(k_strings[i]);
		test(b);

		b = nob_set_current_dir(k_strings[i]);
		test(b);

		mark = nob_temp_save();
		curr_dir = nob_get_current_dir_temp();
		test(memcmp(curr_dir + strlen(curr_dir) - strlen(k_strings[i]), k_strings[i], strlen(k_strings[i])) == 0);
		nob_temp_rewind(mark);

		b = nob_set_current_dir("..");
		test(b);

		b = nob_delete_file(k_strings[i]);
		test(b);
	}
}

static void test_unicode_utf8_file_operations(void)
{
	int n;
	int i;
	Nob_Fd fd;
	Nob_File_Type ft;
	bool b;
	Nob_String_Builder sb;

	nob_log(NOB_INFO, "%s", "Testing file operations...");
	n = NOB_ARRAY_LEN(k_strings);
	for(i = 0; i != n; ++i)
	{
		fd = nob_fd_open_for_write(k_strings[i]);
		test(fd != NOB_INVALID_FD);
		nob_fd_close(fd);

		fd = nob_fd_open_for_read(k_strings[i]);
		test(fd != NOB_INVALID_FD);
		nob_fd_close(fd);

		ft = nob_get_file_type(k_strings[i]);
		test(ft == NOB_FILE_REGULAR);

		b = nob_rename(k_strings[i], k_strings[(i + 1) % NOB_ARRAY_LEN(k_strings)]);
		test(b);
		b = nob_rename(k_strings[(i + 1) % NOB_ARRAY_LEN(k_strings)], k_strings[i]);
		test(b);

		b = nob_write_entire_file(k_strings[i], "test", 4);
		test(b);

		sb.items = NULL;
		sb.capacity = 0;
		sb.count = 0;
		b = nob_read_entire_file(k_strings[i], &sb);
		test(b);
		test(sb.count == 4);
		test(memcmp(sb.items, "test", 4) == 0);
		NOB_FREE(sb.items);

		b = nob_delete_file(k_strings[i]);
		test(b);
	}
}


int main(void)
{
	test_unicode_utf8_printf();
	test_unicode_utf8_dir();
	test_unicode_utf8_file_operations();
	return 0;
}
