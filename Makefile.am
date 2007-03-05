## Process this file with automake to produce Makefile.in.

dist_doc_DATA = \
  doc/Tech.Notes \
  doc/pcre.txt \
  doc/pcregrep.txt \
  doc/pcretest.txt \
  doc/perltest.txt \
  AUTHORS \
  COPYING \
  ChangeLog \
  INSTALL \
  LICENCE \
  NEWS \
  NON-UNIX-USE \
  README

dist_html_DATA = \
  doc/html/index.html \
  doc/html/pcre.html \
  doc/html/pcre_compile.html \
  doc/html/pcre_compile2.html \
  doc/html/pcre_config.html \
  doc/html/pcre_copy_named_substring.html \
  doc/html/pcre_copy_substring.html \
  doc/html/pcre_dfa_exec.html \
  doc/html/pcre_exec.html \
  doc/html/pcre_free_substring.html \
  doc/html/pcre_free_substring_list.html \
  doc/html/pcre_fullinfo.html \
  doc/html/pcre_get_named_substring.html \
  doc/html/pcre_get_stringnumber.html \
  doc/html/pcre_get_stringtable_entries.html \
  doc/html/pcre_get_substring.html \
  doc/html/pcre_get_substring_list.html \
  doc/html/pcre_info.html \
  doc/html/pcre_maketables.html \
  doc/html/pcre_refcount.html \
  doc/html/pcre_study.html \
  doc/html/pcre_version.html \
  doc/html/pcreapi.html \
  doc/html/pcrebuild.html \
  doc/html/pcrecallout.html \
  doc/html/pcrecompat.html \
  doc/html/pcregrep.html \
  doc/html/pcrematching.html \
  doc/html/pcrepartial.html \
  doc/html/pcrepattern.html \
  doc/html/pcreperform.html \
  doc/html/pcreposix.html \
  doc/html/pcreprecompile.html \
  doc/html/pcresample.html \
  doc/html/pcrestack.html \
  doc/html/pcretest.html

pcrecpp_html = doc/html/pcrecpp.html
dist_noinst_DATA = $(pcrecpp_html)

if WITH_PCRE_CPP
html_DATA = $(pcrecpp_html)
endif

# The Libtool libraries to install.  We'll add to this later.
lib_LTLIBRARIES =

# Unit tests you want to run when people type 'make check'.
# TESTS is for binary unit tests, check_SCRIPTS for script-based tests
TESTS =
check_SCRIPTS =
noinst_SCRIPTS =

# Some of the binaries we make are to be installed, and others are
# (non-user-visible) helper programs needed to build libpcre.
bin_PROGRAMS =
noinst_PROGRAMS =

# Additional files to delete on 'make clean' and 'make maintainer-clean'.
CLEANFILES =
MAINTAINERCLEANFILES =

# Additional files to bundle with the distribution.
EXTRA_DIST =

# These are the header files we'll install.  Note that while pcre.h is
# generated from pcre.h.in, we want to include it in the distribution so
# that users building without Autotools support have an easier time of it.
include_HEADERS = \
  pcre.h \
  pcreposix.h

# These will be installed too, if C++ support is enabled. We don't
# distribute pcrecpparg.h nor pcre_stringpiece.h, as these are generated
# from corresponding .h.in files (which we do distribute).
if WITH_PCRE_CPP
nodist_include_HEADERS = \
  pcrecpparg.h \
  pcre_stringpiece.h
include_HEADERS += \
  pcrecpp.h \
  pcre_scanner.h
endif # WITH_PCRE_CPP

bin_SCRIPTS = pcre-config

## ---------------------------------------------------------------
## Some helper programs used to compile libpcre.  They aren't user-visible.
noinst_PROGRAMS += dftables
dftables_SOURCES = dftables.c

pcre_chartables.c: dftables$(EXEEXT)
	./dftables$(EXEEXT) $@

## The main pcre library
lib_LTLIBRARIES += libpcre.la
libpcre_la_SOURCES = \
  pcre_compile.c \
  pcre_config.c \
  pcre_dfa_exec.c \
  pcre_exec.c \
  pcre_fullinfo.c \
  pcre_get.c \
  pcre_globals.c \
  pcre_info.c \
  pcre_internal.h \
  pcre_maketables.c \
  pcre_newline.c \
  pcre_ord2utf8.c \
  pcre_refcount.c \
  pcre_study.c \
  pcre_tables.c \
  pcre_try_flipped.c \
  pcre_ucp_searchfuncs.c \
  pcre_valid_utf8.c \
  pcre_version.c \
  pcre_xclass.c \
  ucp.h \
  ucpinternal.h \
  ucptable.h
nodist_libpcre_la_SOURCES = \
  pcre_chartables.c
EXTRA_DIST += pcre_printint.src
libpcre_la_LDFLAGS = $(EXTRA_LIBPCRE_LDFLAGS)

CLEANFILES += pcre_chartables.c

## A version of the main pcre library that has a posix re API.
lib_LTLIBRARIES += libpcreposix.la
libpcreposix_la_SOURCES = \
  pcreposix.c
libpcreposix_la_LDFLAGS = $(EXTRA_LIBPCREPOSIX_LDFLAGS)
libpcreposix_la_LIBADD = libpcre.la

## There's a C++ library as well.
if WITH_PCRE_CPP

lib_LTLIBRARIES += libpcrecpp.la
libpcrecpp_la_SOURCES = \
  pcrecpp.cc \
  pcre_scanner.cc \
  pcre_stringpiece.cc
libpcrecpp_la_LDFLAGS = $(EXTRA_LIBPCRECPP_LDFLAGS)
libpcrecpp_la_LIBADD = libpcre.la

TESTS += pcrecpp_unittest
noinst_PROGRAMS += pcrecpp_unittest
pcrecpp_unittest_SOURCES = pcrecpp_unittest.cc
pcrecpp_unittest_LDADD = libpcrecpp.la

TESTS += pcre_scanner_unittest
noinst_PROGRAMS += pcre_scanner_unittest
pcre_scanner_unittest_SOURCES = pcre_scanner_unittest.cc
pcre_scanner_unittest_LDADD = libpcrecpp.la

TESTS += pcre_stringpiece_unittest
noinst_PROGRAMS += pcre_stringpiece_unittest
pcre_stringpiece_unittest_SOURCES = pcre_stringpiece_unittest.cc
pcre_stringpiece_unittest_LDADD = libpcrecpp.la

endif # WITH_PCRE_CPP

## The main unit tests

# Each unit test is a binary plus a script that runs that binary in various
# ways. We install these test binaries in case folks find it helpful.

TESTS += RunTest
noinst_SCRIPTS += RunTest
EXTRA_DIST += RunTest.bat
bin_PROGRAMS += pcretest
pcretest_SOURCES = pcretest.c
pcretest_LDADD = libpcreposix.la

TESTS += RunGrepTest
noinst_SCRIPTS += RunGrepTest
bin_PROGRAMS += pcregrep
pcregrep_SOURCES = pcregrep.c
pcregrep_LDADD = libpcreposix.la

EXTRA_DIST += \
  testdata/grepinput \
  testdata/grepinput8 \
  testdata/grepinputx \
  testdata/greplist \
  testdata/grepoutput \
  testdata/grepoutput8 \
  testdata/testinput1 \
  testdata/testinput2 \
  testdata/testinput3 \
  testdata/testinput4 \
  testdata/testinput5 \
  testdata/testinput6 \
  testdata/testinput7 \
  testdata/testinput8 \
  testdata/testinput9 \
  testdata/testoutput1 \
  testdata/testoutput2 \
  testdata/testoutput3 \
  testdata/testoutput4 \
  testdata/testoutput5 \
  testdata/testoutput6 \
  testdata/testoutput7 \
  testdata/testoutput8 \
  testdata/testoutput9 \
  perltest.pl

CLEANFILES += \
	testsavedregex \
	teststderr \
	testtry

# PCRE demonstration program
noinst_PROGRAMS += pcredemo
pcredemo_SOURCES = pcredemo.c
pcredemo_LDADD = libpcre.la

## Utility rules, documentation, etc.

# A compatibility line, the old build system worked with 'make test'
test: check ;

# We have .pc files for pkg-config users.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libpcre.pc
if WITH_PCRE_CPP
pkgconfig_DATA += libpcrecpp.pc
endif

dist_man_MANS = \
  doc/pcre.3 \
  doc/pcre_compile.3 \
  doc/pcre_compile2.3 \
  doc/pcre_config.3 \
  doc/pcre_copy_named_substring.3 \
  doc/pcre_copy_substring.3 \
  doc/pcre_dfa_exec.3 \
  doc/pcre_exec.3 \
  doc/pcre_free_substring.3 \
  doc/pcre_free_substring_list.3 \
  doc/pcre_fullinfo.3 \
  doc/pcre_get_named_substring.3 \
  doc/pcre_get_stringnumber.3 \
  doc/pcre_get_stringtable_entries.3 \
  doc/pcre_get_substring.3 \
  doc/pcre_get_substring_list.3 \
  doc/pcre_info.3 \
  doc/pcre_maketables.3 \
  doc/pcre_refcount.3 \
  doc/pcre_study.3 \
  doc/pcre_version.3 \
  doc/pcreapi.3 \
  doc/pcrebuild.3 \
  doc/pcrecallout.3 \
  doc/pcrecompat.3 \
  doc/pcregrep.1 \
  doc/pcrematching.3 \
  doc/pcrepartial.3 \
  doc/pcrepattern.3 \
  doc/pcreperform.3 \
  doc/pcreposix.3 \
  doc/pcreprecompile.3 \
  doc/pcresample.3 \
  doc/pcrestack.3 \
  doc/pcretest.1

pcrecpp_man = doc/pcrecpp.3
EXTRA_DIST += $(pcrecpp_man)

if WITH_PCRE_CPP
man_MANS = $(pcrecpp_man)
endif

## CMake support

EXTRA_DIST += \
  CMakeLists.txt \
  config-cmake.h.in

config-cmake.h.in: config.h.in
	sed 's/#undef/#cmakedefine/g' config.h.in >$@

MAINTAINERCLEANFILES += config-cmake.h.in

## end Makefile.am
