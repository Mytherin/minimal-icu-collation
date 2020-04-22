import os
import sys
import shutil
import re

icu_dir = '/Users/myth/Sources/icu/icu4c/source'
target_dir = os.path.join(os.getcwd(), 'icu')
directories = ['common', 'i18n']
extensions = ['.cpp', '.h']
include_paths = [os.path.join(icu_dir, 'i18n'), os.path.join(icu_dir, 'common')]
ignored_includes = ['uconfig_local.h', 'ucln_local_hook.c', 'stdio.h', 'unistrm.h', 'udbgutil.h']
always_included = ['locdspnm.cpp', 'rbnf.cpp', 'nfrs.cpp', 'nfsubs.cpp', 'decContext.cpp', 'decNumber.cpp']
extra_files = ['i18n/ucln_in.cpp', 'stubdata/stubdata.cpp']
ignored_files = []
# ignored_files = ['dictbe.cpp', 'ucnvsel.cpp', 'ucnv_set.cpp', 'ucnvscsu.cpp', 'wintz.cpp', 'dictionarydata.cpp', 'icuplug.cpp', 'ucnv_ext.cpp', 'ustr_cnv.cpp', 'ucnv_u32.cpp', 'punycode.cpp', 'rbbi.cpp', 'ucnvdisp.cpp', 'ucnv_err.cpp', 'unistr_titlecase_brkiter.cpp', 'rbbisetb.cpp', 'filteredbrk.cpp', 'ubrk.cpp', 'ucnv_bld.cpp', 'brkeng.cpp', 'ucnv_ct.cpp', 'ucnv_u8.cpp', 'ucnv_cb.cpp', 'ustr_titlecase_brkiter.cpp', 'static_unicode_sets.cpp', 'locdspnm.cpp', 'rbbi_cache.cpp', 'ucnv.cpp', 'ucnvlat1.cpp', 'uts46.cpp', 'ucnvmbcs.cpp', 'rbbistbl.cpp', 'messagepattern.cpp', 'cwchar.cpp', 'rbbiscan.cpp', 'ucnvbocu.cpp', 'ucnv_lmb.cpp', 'ucurr.cpp', 'restrace.cpp', 'ucnvhz.cpp', 'uidna.cpp', 'ucnv_u7.cpp', 'rbbidata.cpp', 'ucnvisci.cpp', 'unistr_cnv.cpp', 'rbbirb.cpp', 'ucasemap_titlecase_brkiter.cpp', 'usprep.cpp', 'rbbitblb.cpp', 'ucnv_u16.cpp', 'ucnv_io.cpp', 'ucnv_cnv.cpp', 'ucnv2022.cpp', 'rbbinode.cpp', 'brkiter.cpp', 'usearch.cpp', 'stsearch.cpp', 'search.cpp', 'rbnf.cpp', 'nfrule.cpp', 'dictbe.cpp', 'ucnvsel.cpp', 'ucnv_set.cpp', 'ucnvscsu.cpp', 'wintz.cpp', 'dictionarydata.cpp', 'icuplug.cpp', 'ucnv_ext.cpp', 'ustr_cnv.cpp', 'ucnv_u32.cpp', 'punycode.cpp', 'rbbi.cpp', 'ucnvdisp.cpp', 'ucnv_err.cpp', 'unistr_titlecase_brkiter.cpp', 'rbbisetb.cpp', 'filteredbrk.cpp', 'ubrk.cpp', 'ucnv_bld.cpp', 'brkeng.cpp', 'ucnv_ct.cpp', 'ucnv_u8.cpp', 'ucnv_cb.cpp', 'ustr_titlecase_brkiter.cpp', 'static_unicode_sets.cpp', 'locdspnm.cpp', 'rbbi_cache.cpp', 'ucnv.cpp', 'ucnvlat1.cpp', 'uts46.cpp', 'ucnvmbcs.cpp', 'rbbistbl.cpp', 'messagepattern.cpp', 'cwchar.cpp', 'rbbiscan.cpp', 'ucnvbocu.cpp', 'ucnv_lmb.cpp', 'ucurr.cpp', 'restrace.cpp', 'ucnvhz.cpp', 'uidna.cpp', 'ucnv_u7.cpp', 'rbbidata.cpp', 'ucnvisci.cpp', 'unistr_cnv.cpp', 'rbbirb.cpp', 'ucasemap_titlecase_brkiter.cpp', 'usprep.cpp', 'rbbitblb.cpp', 'ucnv_u16.cpp', 'ucnv_io.cpp', 'ucnv_cnv.cpp', 'ucnv2022.cpp', 'rbbinode.cpp', 'brkiter.cpp', 'usearch.cpp', 'stsearch.cpp', 'search.cpp', 'rbnf.cpp', 'nfrule.cpp', 'servnotf.cpp', 'locutil.cpp', 'servls.cpp', 'servlkf.cpp', 'servlk.cpp', 'serv.cpp', 'servrbf.cpp', 'servslkf.cpp', 'servnotf.cpp', 'locutil.cpp', 'servls.cpp', 'servlkf.cpp', 'servlk.cpp', 'serv.cpp', 'servrbf.cpp', 'servslkf.cpp']
# in/coll/ucadata-*.icu
# coll/*.txt

file_content_list = []

copied_file_set = {}

def get_includes(text, include_dir):
    # find all the includes referred to in the directory
    include_statements = re.findall("(^[#][ \t]*include[\t ]+[\"]([^\"]+)[\"])", text, flags=re.MULTILINE)
    # figure out where they are located
    for included_file in [x[1] for x in include_statements]:
        if included_file in ignored_includes:
            continue
        found_file = False
        file_already_exists = False
        for dirname in directories:
            full_path = os.path.join(icu_dir, dirname, included_file)
            if full_path in copied_file_set:
                file_already_exists = True
                break
            if os.path.isfile(full_path):
                found_file = True
                break
        if file_already_exists:
            continue
        if not found_file:
            raise Exception('Could not find include file "' + full_path)

        copied_file_set[full_path] = True
        get_includes(open(full_path).read(), include_dir)

def file_always_included(full_path):
    for fname in always_included:
        if fname in full_path:
            return True
    return False

def get_files_to_copy(dirname):
    source_path = os.path.join(icu_dir, dirname)
    file_list = os.listdir(source_path)
    for fname in file_list:
        full_path = os.path.join(source_path, fname)
        if os.path.isdir(full_path):
            get_files_to_copy(os.path.join(dirname, fname))
        else:
            found = False
            for ext in extensions:
                if fname.endswith(ext):
                    found = True
                    break
            if not found:
                continue
            if fname in ignored_files:
                continue
            with open(full_path, 'r') as f:
                contents = f.read()
            if dirname == 'common' or file_always_included(full_path) or '!UCONFIG_NO_FORMATTING' in contents or '!UCONFIG_NO_COLLATION' in contents or 'coll' in fname.lower():
                copied_file_set[full_path] = True
                get_includes(contents, source_path)

for extra_file in extra_files:
    full_path = os.path.join(icu_dir, extra_file)
    copied_file_set[full_path] = True
    with open(full_path, 'r') as f:
        contents = f.read()
    get_includes(contents, full_path.rsplit(os.path.sep, 1)[0])

# first pass: copy all files that get compiled for collation support (i.e. have #if !UCONFIG_NO_COLLATION in them)
for dirname in directories:
    get_files_to_copy(dirname)

# for every file that is a header file, we also include the .cpp if they are there
for fname in copied_file_set:
    if fname.endswith('.h'):
        cpp_file = fname[:-1] + '.cpp'
        if os.path.isfile(cpp_file):
            copied_file_set[cpp_file] = True

# make some base dirs
makedirs = ['common', 'common/unicode', 'i18n', 'i18n/unicode', 'stubdata']
try:
    os.mkdir('icu')
except:
    pass
for dirname in makedirs:
    try:
        os.mkdir(os.path.join(target_dir, dirname))
    except:
        pass

to_be_compiled = []

# final pass, copy all the files
for fname in copied_file_set:
    # copy the file
    base_path = fname.replace(icu_dir + '/', '')
    target_path = os.path.join(target_dir, base_path)
    if not os.path.isfile(target_path):
        print(target_path)
        shutil.copy(fname, target_path)
    if fname.endswith('.cpp'):
        to_be_compiled.append(base_path)

# create the cmake file
cmake_file = """
cmake_minimum_required(VERSION 2.8.7)

project(MinimalICUCollation)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DU_COMMON_IMPLEMENTATION -DU_STATIC_IMPLEMENTATION -DUCONFIG_NO_BREAK_ITERATION=1 -DUCONFIG_NO_IDNA=1 -DUCONFIG_NO_CONVERSION=1 -DUCONFIG_NO_TRANSLITERATION=1 -DUCONFIG_NO_REGULAR_EXPRESSIONS=1 -DUCONFIG_NO_SERVICE=1)

option(FORCE_COLORED_OUTPUT
       "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if(${FORCE_COLORED_OUTPUT})
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
  endif()
endif()

include_directories(common)
include_directories(i18n)

add_library(minimal_icu_collation STATIC %s)
add_executable(minimal_icu_example example.cpp)
target_link_libraries(minimal_icu_example minimal_icu_collation)


""" % (' '.join(to_be_compiled))


with open('icu/CMakeLists.txt', 'w+') as f:
    f.write(cmake_file)

# coll_ucadata
# coll_tree
