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
	int len;

	nob_custom_printf("%s\n", "Testing printf...");
	n = NOB_ARRAY_LEN(k_strings);
	for(i = 0; i != n; ++i)
	{
		len = nob_custom_printf("%s\n", k_strings[i]);
		nob_custom_printf("Last printf was %d bytes long.\n", len);
	}
	nob_custom_printf("\n");
}

static void not_nob_delete_dir(char const* dir_name)
{
#if defined _WIN32
	size_t mark;
	wchar_t* wide_dir;
	BOOL b;

	mark = nob_temp_save();
	wide_dir = nob_unicode_utf8_to_unicode_utf16(dir_name);
	b = RemoveDirectoryW(wide_dir);
	nob_temp_rewind(mark);
	test(b != FALSE);
#else
	bool b;

	b = nob_delete_file(dir_name);
	test(b);
#endif
}

static void test_unicode_utf8_dir(void)
{
	int n;
	int i;
	bool b;
	size_t mark;
	char const* curr_dir;

	nob_custom_printf("%s\n", "Testing mkdir...");
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

		not_nob_delete_dir(k_strings[i]);
	}
	nob_custom_printf("\n");
}

static void test_unicode_utf8_file_operations(void)
{
	int n;
	int i;
	Nob_Fd fd;
	Nob_File_Type ft;
	bool b;

	nob_custom_printf("%s\n", "Testing file operations...");
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

		b = nob_delete_file(k_strings[i]);
		test(b);
	}
	nob_custom_printf("\n");
}


int main(void)
{
	test_unicode_utf8_printf();
	test_unicode_utf8_dir();
	test_unicode_utf8_file_operations();
	return 0;
}
