# MSVC C++ was never tried

- STATUS: OPEN
- PRIORITY: 100
- TAGS: bug,msvc

Every time we were doing `cl.exe nob.c` MSVC was compiling everything in "C mode". 
You can force it to compile in "C++ mode" if you provide flag `/TP` (See 
[Specify Source File Type][source-file-type]). If you do `cl.exe /TP nob.c` 
everything falls apart.

We should fix everything and have a separate "C++ on MSVC" build on GitHub Actions.

[source-file-type]: https://learn.microsoft.com/en-us/cpp/build/reference/tc-tp-tc-tp-specify-source-file-type?view=msvc-170
