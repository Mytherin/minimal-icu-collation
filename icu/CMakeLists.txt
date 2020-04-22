
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

include_directories(common)
include_directories(i18n)

add_library(minimal_icu_collation STATIC i18n/ucln_in.cpp stubdata/stubdata.cpp common/uniset.cpp common/chariter.cpp common/utf_impl.cpp common/uhash.cpp common/ushape.cpp common/uarrsort.cpp common/uniset_props.cpp common/caniter.cpp common/ubidiwrt.cpp common/cstr.cpp common/ubiditransform.cpp common/loclikelysubtags.cpp common/stringtriebuilder.cpp common/util.cpp common/cstring.cpp common/localeprioritylist.cpp common/uinit.cpp common/ucharstrieiterator.cpp common/unisetspan.cpp common/locid.cpp common/unames.cpp common/filterednormalizer2.cpp common/loclikely.cpp common/uscript_props.cpp common/bytestrieiterator.cpp common/propsvec.cpp common/ubidiln.cpp common/putil.cpp common/bytestrie.cpp common/resource.cpp common/uvector.cpp common/ucase.cpp common/uloc_tag.cpp common/icudataver.cpp common/uenum.cpp common/charstr.cpp common/uloc_keytype.cpp common/bytestriebuilder.cpp common/ubidi.cpp common/locdispnames.cpp common/locdistance.cpp common/bmpset.cpp common/uniset_closure.cpp common/bytesinkutil.cpp common/ucharstriebuilder.cpp common/utrie2.cpp common/unifiedcache.cpp common/schriter.cpp common/locbased.cpp common/udatamem.cpp common/umutablecptrie.cpp common/patternprops.cpp common/localebuilder.cpp common/unistr_props.cpp common/stringpiece.cpp common/ulist.cpp common/ustrtrns.cpp common/uiter.cpp common/cmemory.cpp common/resbund_cnv.cpp common/uvectr64.cpp common/unistr_case.cpp common/ustr_wcs.cpp common/simpleformatter.cpp common/locresdata.cpp common/pluralmap.cpp common/uprops.cpp common/usetiter.cpp common/propname.cpp common/ucat.cpp common/unifilt.cpp common/utrie_swap.cpp common/uresdata.cpp common/umapfile.cpp common/locavailable.cpp common/ustrcase.cpp common/umath.cpp common/uset.cpp common/ucln_cmn.cpp common/ures_cnv.cpp common/normlzr.cpp common/ucmndata.cpp common/edits.cpp common/ustrfmt.cpp common/parsepos.cpp common/loadednormalizer2impl.cpp common/udata.cpp common/errorcode.cpp common/uchriter.cpp common/udataswp.cpp common/ustrcase_locale.cpp common/localematcher.cpp common/unistr.cpp common/normalizer2.cpp common/sharedobject.cpp common/resbund.cpp common/uhash_us.cpp common/utrie.cpp common/umutex.cpp common/unorm.cpp common/ruleiter.cpp common/uset_props.cpp common/appendable.cpp common/lsr.cpp common/utypes.cpp common/ucasemap.cpp common/usc_impl.cpp common/uresbund.cpp common/unormcmp.cpp common/ucptrie.cpp common/ubidi_props.cpp common/uchar.cpp common/normalizer2impl.cpp common/unifunct.cpp common/characterproperties.cpp common/ustack.cpp common/uinvchar.cpp common/bytestream.cpp common/ustring.cpp common/util_props.cpp common/uloc.cpp common/ustrenum.cpp common/unistr_case_locale.cpp common/uobject.cpp common/ucol_swp.cpp common/dtintrv.cpp common/ucharstrie.cpp common/uscript.cpp common/utext.cpp common/locmap.cpp common/utrie2_builder.cpp common/uvectr32.cpp common/utrace.cpp i18n/collationweights.cpp i18n/collationruleparser.cpp i18n/collation.cpp i18n/collationsets.cpp i18n/ucoleitr.cpp i18n/collationdatawriter.cpp i18n/coleitr.cpp i18n/uitercollationiterator.cpp i18n/collationfcd.cpp i18n/sortkey.cpp i18n/alphaindex.cpp i18n/collationkeys.cpp i18n/collationfastlatin.cpp i18n/utf16collationiterator.cpp i18n/collationsettings.cpp i18n/collationdatareader.cpp i18n/collationbuilder.cpp i18n/collationtailoring.cpp i18n/collationfastlatinbuilder.cpp i18n/bocsu.cpp i18n/rulebasedcollator.cpp i18n/coll.cpp i18n/collationrootelements.cpp i18n/collationiterator.cpp i18n/utf8collationiterator.cpp i18n/collationdata.cpp i18n/ucol_sit.cpp i18n/ucol.cpp i18n/collationroot.cpp i18n/ucol_res.cpp i18n/collationdatabuilder.cpp i18n/collationcompare.cpp)
add_executable(minimal_icu_example sort.cpp)
target_link_libraries(minimal_icu_example minimal_icu_collation)


