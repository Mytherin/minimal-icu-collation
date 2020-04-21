
cmake_minimum_required(VERSION 2.8.7)

project(MinimalICUCollation)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DU_COMMON_IMPLEMENTATION -DU_STATIC_IMPLEMENTATION -DUCONFIG_NO_BREAK_ITERATION=1 -DUCONFIG_NO_IDNA=1 -DUCONFIG_NO_CONVERSION=1 -DUCONFIG_NO_FORMATTING=1 -DUCONFIG_NO_TRANSLITERATION=1 -DUCONFIG_NO_REGULAR_EXPRESSIONS=1 -DUCONFIG_NO_SERVICE=1)

option(FORCE_COLORED_OUTPUT
       "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if(${FORCE_COLORED_OUTPUT})
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
  endif()
endif()

include_directories(icu/common)
include_directories(icu/i18n)

add_library(minimal_icu_collation STATIC icu/i18n/ucln_in.cpp icu/stubdata/stubdata.cpp icu/common/uniset.cpp icu/common/chariter.cpp icu/common/utf_impl.cpp icu/common/uhash.cpp icu/common/ushape.cpp icu/common/uarrsort.cpp icu/common/uniset_props.cpp icu/common/caniter.cpp icu/common/ubidiwrt.cpp icu/common/cstr.cpp icu/common/ubiditransform.cpp icu/common/loclikelysubtags.cpp icu/common/stringtriebuilder.cpp icu/common/util.cpp icu/common/cstring.cpp icu/common/localeprioritylist.cpp icu/common/uinit.cpp icu/common/ucharstrieiterator.cpp icu/common/unisetspan.cpp icu/common/locid.cpp icu/common/unames.cpp icu/common/filterednormalizer2.cpp icu/common/loclikely.cpp icu/common/uscript_props.cpp icu/common/bytestrieiterator.cpp icu/common/propsvec.cpp icu/common/ubidiln.cpp icu/common/putil.cpp icu/common/bytestrie.cpp icu/common/resource.cpp icu/common/uvector.cpp icu/common/ucase.cpp icu/common/uloc_tag.cpp icu/common/icudataver.cpp icu/common/uenum.cpp icu/common/charstr.cpp icu/common/uloc_keytype.cpp icu/common/bytestriebuilder.cpp icu/common/ubidi.cpp icu/common/locdispnames.cpp icu/common/locdistance.cpp icu/common/bmpset.cpp icu/common/uniset_closure.cpp icu/common/bytesinkutil.cpp icu/common/ucharstriebuilder.cpp icu/common/utrie2.cpp icu/common/unifiedcache.cpp icu/common/schriter.cpp icu/common/locbased.cpp icu/common/udatamem.cpp icu/common/umutablecptrie.cpp icu/common/patternprops.cpp icu/common/localebuilder.cpp icu/common/unistr_props.cpp icu/common/stringpiece.cpp icu/common/ulist.cpp icu/common/ustrtrns.cpp icu/common/uiter.cpp icu/common/cmemory.cpp icu/common/resbund_cnv.cpp icu/common/uvectr64.cpp icu/common/unistr_case.cpp icu/common/ustr_wcs.cpp icu/common/simpleformatter.cpp icu/common/locresdata.cpp icu/common/pluralmap.cpp icu/common/uprops.cpp icu/common/usetiter.cpp icu/common/propname.cpp icu/common/ucat.cpp icu/common/unifilt.cpp icu/common/utrie_swap.cpp icu/common/uresdata.cpp icu/common/umapfile.cpp icu/common/locavailable.cpp icu/common/ustrcase.cpp icu/common/umath.cpp icu/common/uset.cpp icu/common/ucln_cmn.cpp icu/common/ures_cnv.cpp icu/common/normlzr.cpp icu/common/ucmndata.cpp icu/common/edits.cpp icu/common/ustrfmt.cpp icu/common/parsepos.cpp icu/common/loadednormalizer2impl.cpp icu/common/udata.cpp icu/common/errorcode.cpp icu/common/uchriter.cpp icu/common/udataswp.cpp icu/common/ustrcase_locale.cpp icu/common/localematcher.cpp icu/common/unistr.cpp icu/common/normalizer2.cpp icu/common/sharedobject.cpp icu/common/resbund.cpp icu/common/uhash_us.cpp icu/common/utrie.cpp icu/common/umutex.cpp icu/common/unorm.cpp icu/common/ruleiter.cpp icu/common/uset_props.cpp icu/common/appendable.cpp icu/common/lsr.cpp icu/common/utypes.cpp icu/common/ucasemap.cpp icu/common/usc_impl.cpp icu/common/uresbund.cpp icu/common/unormcmp.cpp icu/common/ucptrie.cpp icu/common/ubidi_props.cpp icu/common/uchar.cpp icu/common/normalizer2impl.cpp icu/common/unifunct.cpp icu/common/characterproperties.cpp icu/common/ustack.cpp icu/common/uinvchar.cpp icu/common/bytestream.cpp icu/common/ustring.cpp icu/common/util_props.cpp icu/common/uloc.cpp icu/common/ustrenum.cpp icu/common/unistr_case_locale.cpp icu/common/uobject.cpp icu/common/ucol_swp.cpp icu/common/dtintrv.cpp icu/common/ucharstrie.cpp icu/common/uscript.cpp icu/common/utext.cpp icu/common/locmap.cpp icu/common/utrie2_builder.cpp icu/common/uvectr32.cpp icu/common/utrace.cpp icu/i18n/collationweights.cpp icu/i18n/collationruleparser.cpp icu/i18n/collation.cpp icu/i18n/collationsets.cpp icu/i18n/ucoleitr.cpp icu/i18n/collationdatawriter.cpp icu/i18n/coleitr.cpp icu/i18n/uitercollationiterator.cpp icu/i18n/collationfcd.cpp icu/i18n/sortkey.cpp icu/i18n/alphaindex.cpp icu/i18n/collationkeys.cpp icu/i18n/collationfastlatin.cpp icu/i18n/utf16collationiterator.cpp icu/i18n/collationsettings.cpp icu/i18n/collationdatareader.cpp icu/i18n/collationbuilder.cpp icu/i18n/collationtailoring.cpp icu/i18n/collationfastlatinbuilder.cpp icu/i18n/bocsu.cpp icu/i18n/rulebasedcollator.cpp icu/i18n/coll.cpp icu/i18n/collationrootelements.cpp icu/i18n/collationiterator.cpp icu/i18n/utf8collationiterator.cpp icu/i18n/collationdata.cpp icu/i18n/ucol_sit.cpp icu/i18n/ucol.cpp icu/i18n/collationroot.cpp icu/i18n/ucol_res.cpp icu/i18n/collationdatabuilder.cpp icu/i18n/collationcompare.cpp)
add_executable(minimal_icu_example example.cpp)
target_link_libraries(minimal_icu_example minimal_icu_collation)


