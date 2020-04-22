# this script creates a single header + source file combination out of the DuckDB sources
import os, re, sys, shutil
amal_dir = os.path.join('amalgamation')
header_file = os.path.join(amal_dir, "icu-collate.hpp")
source_file = os.path.join(amal_dir, "icu-collate.cpp")

src_dir = 'icu'

# files included in the amalgamated "icu-collate.hpp" file
main_header_files = [os.path.join(src_dir, 'i18n', 'unicode', 'coll.h'), 'icu/i18n/unicode/sortkey.h']

# include paths for where to search for include files during amalgamation
include_paths = [os.path.join(src_dir, 'common'), os.path.join(src_dir, 'common', 'unicode'), os.path.join(src_dir, 'i18n'), os.path.join(src_dir, 'i18n', 'unicode')]
# paths of where to look for files to compile and include to the final amalgamation
compile_directories = include_paths + [os.path.join(src_dir, 'stubdata')]
# files that are ignored
ignored_includes = ['uconfig_local.h', 'ucln_local_hook.c', 'stdio.h']



linenumbers = False

for arg in sys.argv:
    if arg == '--linenumbers':
        linenumbers = True
    elif arg == '--no-linenumbers':
        linenumbers = False
    elif arg.startswith('--header='):
        header_file = os.path.join(*arg.split('=', 1)[1].split('/'))
    elif arg.startswith('--source='):
        source_file = os.path.join(*arg.split('=', 1)[1].split('/'))
if not os.path.exists(amal_dir):
    os.makedirs(amal_dir)

def file_is_ignored(file):
    for ignored_file in ignored_includes:
        if ignored_file in file:
            return True
    return False

def get_includes(fpath, text):
    # find all the includes referred to in the directory
    include_statements = re.findall("(^[#][ \t]*include[\t ]+[\"]([^\"]+)[\"])", text, flags=re.MULTILINE)
    include_files = []
    # figure out where they are located
    for included_file in [x[1] for x in include_statements]:
        included_file = os.sep.join(included_file.split('/'))

        if file_is_ignored(included_file):
            continue
        found = False
        for include_path in include_paths:
            ipath = os.path.join(include_path, included_file)
            if os.path.isfile(ipath):
                include_files.append(ipath)
                found = True
                break
        if not found:
            raise Exception('Could not find include file "' + included_file + '", included from file "' + fpath + '"')
    return ([x[0] for x in include_statements], include_files)

# recursively get all includes and write them
written_files = {}

def write_file(current_file, ignore_excluded = False):
    global linenumbers
    global written_files
    if current_file in written_files:
        # file is already written
        return ""
    written_files[current_file] = True

    # first read this file
    with open(current_file, 'r') as f:
        text = f.read()

    # remove include guards
    include_guard_pat = "#ifndef ([a-zA-Z0-9_]+_H)[_]*\n#define ([a-zA-Z0-9_]+_H)[_]*[^\n]*\n"
    match = re.search(include_guard_pat, text)
    if match != None:
        # remove include guards
        text = re.sub(include_guard_pat, "", text)
        # remove endif at end of file
        index = text.rfind("#endif")
        if index <= 0:
            print(current_file)
            exit(1)
        text = text[:index]


    if current_file.endswith(".h") and match == None:
        if current_file not in ['icu/common/bytesinkutil.h', 'icu/common/norm2_nfc_data.h', 'icu/common/propname_data.h', 'icu/common/ubidi_props_data.h', 'icu/common/ucase_props_data.h', 'icu/common/uchar_props_data.h', 'icu/common/unicode/urename.h']:
            print(current_file)
            exit(1)

    (statements, includes) = get_includes(current_file, text)
    # find the linenr of the final #include statement we parsed
    if len(statements) > 0:
        index = text.find(statements[-1])
        linenr = len(text[:index].split('\n'))

        # now write all the dependencies of this header first
        for i in range(len(includes)):
            # if 'utypes.h' in includes[i]:
            #     print(current_file)
            #     exit(1)
            include_text = write_file(includes[i])
            if linenumbers and i == len(includes) - 1:
                # for the last include statement, we also include a #line directive
                include_text += '\n#line %d "%s"\n' % (linenr, current_file)
            text = text.replace(statements[i], include_text)

    # add the initial line here
    if linenumbers:
        text = '\n#line 1 "%s"\n' % (current_file,) + text
    print(current_file)
    # now read the header and write it
    return text

def write_dir(dir, sfile):
    files = os.listdir(dir)
    files.sort()
    for fname in files:
        fpath = os.path.join(dir, fname)
        if os.path.isdir(fpath):
            write_dir(fpath, sfile)
        elif fname.endswith('.cpp') or fname.endswith('.c') or fname.endswith('.cc'):
            sfile.write(write_file(fpath))


# now construct duckdb.hpp from these headers
print("-----------------------")
print("-- Writing " + header_file + " --")
print("-----------------------")
with open(header_file, 'w+') as hfile:
    hfile.write("""#pragma once

#ifndef U_COMMON_IMPLEMENTATION
#define U_COMMON_IMPLEMENTATION
#endif

#ifndef U_STATIC_IMPLEMENTATION
#define U_STATIC_IMPLEMENTATION
#endif

#ifndef UCONFIG_NO_BREAK_ITERATION
#define UCONFIG_NO_BREAK_ITERATION 1
#endif

#ifndef UCONFIG_NO_IDNA
#define UCONFIG_NO_IDNA 1
#endif

#ifndef UCONFIG_NO_CONVERSION
#define UCONFIG_NO_CONVERSION 1
#endif

#ifndef UCONFIG_NO_FORMATTING
#define UCONFIG_NO_FORMATTING 1
#endif

#ifndef UCONFIG_NO_TRANSLITERATION
#define UCONFIG_NO_TRANSLITERATION 1
#endif

#ifndef UCONFIG_NO_REGULAR_EXPRESSIONS
#define UCONFIG_NO_REGULAR_EXPRESSIONS 1
#endif

#ifndef UCONFIG_NO_SERVICE
#define UCONFIG_NO_SERVICE 1
#endif

#ifndef U_SHOW_CPLUSPLUS_API
#define U_SHOW_CPLUSPLUS_API 1
#endif

""")
    for fpath in main_header_files:
        hfile.write(write_file(fpath))


# now construct duckdb.cpp
print("------------------------")
print("-- Writing " + source_file + " --")
print("------------------------")

# scan all the .cpp files
with open(source_file, 'w+') as sfile:
    header_file_name = header_file.split(os.sep)[-1]
    sfile.write('#include "' + header_file_name + '"\n\n')
    for compile_dir in compile_directories:
        write_dir(compile_dir, sfile)
