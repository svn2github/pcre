# CMakeLists.txt
#
# This file allows building PCRE with the CMake configuration and build
# tool. Download CMake in source or binary form from http://www.cmake.org/
#
# Original listfile by Christian Ehrlicher <Ch.Ehrlicher@gmx.de>
# Refined and expanded by Daniel Richard G. <skunk@iSKUNK.ORG>
#

PROJECT(PCRE C CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.6)

# Configuration checks

INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckTypeSize)

CHECK_INCLUDE_FILE(dirent.h	HAVE_DIRENT_H)
CHECK_INCLUDE_FILE(unistd.h	HAVE_UNISTD_H)
CHECK_INCLUDE_FILE(sys/stat.h	HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(sys/types.h	HAVE_SYS_TYPES_H)

CHECK_INCLUDE_FILE_CXX(type_traits.h		HAVE_TYPE_TRAITS_H)
CHECK_INCLUDE_FILE_CXX(bits/type_traits.h	HAVE_BITS_TYPE_TRAITS_H)

CHECK_FUNCTION_EXISTS(bcopy	HAVE_BCOPY)
CHECK_FUNCTION_EXISTS(memmove	HAVE_MEMMOVE)
CHECK_FUNCTION_EXISTS(strerror	HAVE_STRERROR)

CHECK_TYPE_SIZE("long long"		LONG_LONG)
CHECK_TYPE_SIZE("unsigned long long"	UNSIGNED_LONG_LONG)

# User-configurable options
#
# (Note: CMakeSetup displays these in alphabetical order, regardless of
# the order we use here)

SET(BUILD_SHARED_LIBS "SHARED" CACHE STRING
    "What type of libraries to build. Set to SHARED or STATIC.")

OPTION(PCRE_BUILD_PCRECPP "Build the PCRE C++ library (pcrecpp)." ON)

SET(PCRE_EBCDIC FALSE CACHE BOOL
    "Use EBCDIC coding instead of ASCII. (This is rarely used outside of mainframe systems)")

SET(PCRE_LINK_SIZE "2" CACHE STRING
    "Internal link size (2, 3 or 4 allowed). See LINK_SIZE in config.h.in for details.")

SET(PCRE_MATCH_LIMIT "10000000" CACHE STRING
    "Default limit on internal looping. See MATCH_LIMIT in config.h.in for details.")

SET(PCRE_MATCH_LIMIT_RECURSION "MATCH_LIMIT" CACHE STRING
    "Default limit on internal recursion. See MATCH_LIMIT_RECURSION in config.h.in for details.")

SET(PCRE_NEWLINE "LF" CACHE STRING
    "What to recognize as a newline (one of CR, LF, CRLF, ANY).")

SET(PCRE_NO_RECURSE TRUE CACHE BOOL
    "If ON, then don't use stack recursion when matching. See NO_RECURSE in config.h.in for details.")

SET(PCRE_POSIX_MALLOC_THRESHOLD "10" CACHE STRING
    "Threshold for malloc() usage. See POSIX_MALLOC_THRESHOLD in config.h.in for details.")

SET(PCRE_SUPPORT_UNICODE_PROPERTIES FALSE CACHE BOOL
    "Enable support for Unicode properties. (If set, UTF-8 support will be enabled as well)")

SET(PCRE_SUPPORT_UTF8 FALSE CACHE BOOL
    "Enable support for the Unicode UTF-8 encoding.")

# Prepare build configuration

SET(pcre_have_type_traits 0)
SET(pcre_have_bits_type_traits 0)

IF(HAVE_TYPE_TRAITS_H)
	SET(pcre_have_type_traits 1)
ENDIF(HAVE_TYPE_TRAITS_H)

IF(HAVE_BITS_TYPE_TRAITS_H)
	SET(pcre_have_bits_type_traits 1)
ENDIF(HAVE_BITS_TYPE_TRAITS_H)

SET(pcre_have_long_long 0)
SET(pcre_have_ulong_long 0)

IF(HAVE_LONG_LONG)
	SET(pcre_have_long_long 1)
ENDIF(HAVE_LONG_LONG)

IF(HAVE_UNSIGNED_LONG_LONG)
	SET(pcre_have_ulong_long 1)
ENDIF(HAVE_UNSIGNED_LONG_LONG)

IF(PCRE_SUPPORT_UTF8 OR PCRE_SUPPORT_UNICODE_PROPERTIES)
	SET(SUPPORT_UTF8 1)
ENDIF(PCRE_SUPPORT_UTF8 OR PCRE_SUPPORT_UNICODE_PROPERTIES)

IF(PCRE_SUPPORT_UNICODE_PROPERTIES)
	SET(SUPPORT_UCP 1)
ENDIF(PCRE_SUPPORT_UNICODE_PROPERTIES)

IF(PCRE_NEWLINE STREQUAL "LF")
  SET(NEWLINE "10")
ELSE(PCRE_NEWLINE STREQUAL "LF")
  IF(PCRE_NEWLINE STREQUAL "CR")
    SET(NEWLINE "13")
  ELSE(PCRE_NEWLINE STREQUAL "CR")
    IF(PCRE_NEWLINE STREQUAL "CRLF")
      SET(NEWLINE "3338")
    ELSE(PCRE_NEWLINE STREQUAL "CRLF")
      IF(PCRE_NEWLINE STREQUAL "ANY")
	SET(NEWLINE "-1")
      ELSE(PCRE_NEWLINE STREQUAL "ANY")
	MESSAGE(FATAL_ERROR "The PCRE_NEWLINE variable must be set to one of the following values: \"LF\", \"CR\", \"CRLF\", \"ANY\".")
      ENDIF(PCRE_NEWLINE STREQUAL "ANY")
    ENDIF(PCRE_NEWLINE STREQUAL "CRLF")
  ENDIF(PCRE_NEWLINE STREQUAL "CR")
ENDIF(PCRE_NEWLINE STREQUAL "LF")

IF(PCRE_EBCDIC)
	SET(EBCDIC 1)
ENDIF(PCRE_EBCDIC)

IF(PCRE_NO_RECURSE)
	SET(NO_RECURSE 1)
ENDIF(PCRE_NO_RECURSE)

# Output files

CONFIGURE_FILE(config-cmake.h.in
               ${CMAKE_BINARY_DIR}/config.h
               @ONLY)

CONFIGURE_FILE(pcre.h.generic
               ${CMAKE_BINARY_DIR}/pcre.h
               COPYONLY)

# What about pcre-config and libpcre.pc?

IF(PCRE_BUILD_PCRECPP)
	CONFIGURE_FILE(pcre_stringpiece.h.in
		       ${CMAKE_BINARY_DIR}/pcre_stringpiece.h
		       @ONLY)

	CONFIGURE_FILE(pcrecpparg.h.in
		       ${CMAKE_BINARY_DIR}/pcrecpparg.h
		       @ONLY)
ENDIF(PCRE_BUILD_PCRECPP)

# Character table generation

ADD_EXECUTABLE(dftables dftables.c)

GET_TARGET_PROPERTY(DFTABLES_EXE dftables LOCATION)

ADD_CUSTOM_COMMAND(
	COMMENT "Generating character tables (pcre_chartables.c) for current locale"
	DEPENDS dftables
	COMMAND ${DFTABLES_EXE}
	ARGS	${CMAKE_BINARY_DIR}/pcre_chartables.c
	OUTPUT	${CMAKE_BINARY_DIR}/pcre_chartables.c
)

# Source code

SET(PCRE_HEADERS ${CMAKE_BINARY_DIR}/pcre.h)

SET(PCRE_SOURCES
	${CMAKE_BINARY_DIR}/pcre_chartables.c
	pcre_compile.c
	pcre_config.c
	pcre_dfa_exec.c
	pcre_exec.c
	pcre_fullinfo.c
	pcre_get.c
	pcre_globals.c
	pcre_info.c
	pcre_newline.c
	pcre_maketables.c
	pcre_ord2utf8.c
	pcre_refcount.c
	pcre_study.c
	pcre_tables.c
	pcre_try_flipped.c
	pcre_ucp_searchfuncs.c
	pcre_valid_utf8.c
	pcre_version.c
	pcre_xclass.c
)

SET(PCREPOSIX_HEADERS pcreposix.h)

SET(PCREPOSIX_SOURCES pcreposix.c)

SET(PCRECPP_HEADERS
	pcrecpp.h
	pcre_scanner.h
	${CMAKE_BINARY_DIR}/pcrecpparg.h
	${CMAKE_BINARY_DIR}/pcre_stringpiece.h
)

SET(PCRECPP_SOURCES
	pcrecpp.cc
	pcre_scanner.cc
	pcre_stringpiece.cc
)

# Build setup

ADD_DEFINITIONS(-DHAVE_CONFIG_H)

IF(WIN32)
	# What about -DDLL_EXPORT?
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ENDIF(WIN32)

SET(CMAKE_INCLUDE_CURRENT_DIR 1)

#SET(CMAKE_DEBUG_POSTFIX "d")

# Libraries

ADD_LIBRARY(pcre ${PCRE_HEADERS} ${PCRE_SOURCES})

ADD_LIBRARY(pcreposix ${PCREPOSIX_HEADERS} ${PCREPOSIX_SOURCES})
TARGET_LINK_LIBRARIES(pcreposix pcre)

IF(PCRE_BUILD_PCRECPP)
	ADD_LIBRARY(pcrecpp ${PCRECPP_HEADERS} ${PCRECPP_SOURCES})
	TARGET_LINK_LIBRARIES(pcrecpp pcre)
	IF(MINGW)
		SET_TARGET_PROPERTIES(pcrecpp PROPERTIES PREFIX "mingw-")
	ENDIF(MINGW)
ENDIF(PCRE_BUILD_PCRECPP)

# Executables

ADD_EXECUTABLE(pcretest pcretest.c)
TARGET_LINK_LIBRARIES(pcretest pcreposix)

ADD_EXECUTABLE(pcregrep pcregrep.c)
TARGET_LINK_LIBRARIES(pcregrep pcreposix)

# Testing

ENABLE_TESTING()

IF(UNIX)
	ADD_TEST(test1 sh -c "srcdir=${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/RunTest")
	ADD_TEST(test2 sh -c "srcdir=${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/RunGrepTest")
ELSE(UNIX)
  IF(WIN32)
	ADD_TEST(test1 ${CMAKE_SOURCE_DIR}/RunTest.bat ${CMAKE_SOURCE_DIR})
  ENDIF(WIN32)       
ENDIF(UNIX)

# Installation

SET(CMAKE_INSTALL_ALWAYS 1)

INSTALL(TARGETS pcre pcreposix pcregrep pcretest
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

INSTALL(FILES ${PCRE_HEADERS} ${PCREPOSIX_HEADERS} DESTINATION include)

FILE(GLOB html ${CMAKE_SOURCE_DIR}/doc/html/*.html)
FILE(GLOB man1 ${CMAKE_SOURCE_DIR}/doc/*.1)
FILE(GLOB man3 ${CMAKE_SOURCE_DIR}/doc/*.3)

IF(PCRE_BUILD_PCRECPP)
	INSTALL(TARGETS pcrecpp DESTINATION lib)
	INSTALL(FILES ${PCRECPP_HEADERS} DESTINATION include)
ELSE(PCRE_BUILD_PCRECPP)
	# Remove pcrecpp.3
	FOREACH(man ${man3})
		GET_FILENAME_COMPONENT(man_tmp ${man} NAME)
		IF(NOT man_tmp STREQUAL "pcrecpp.3")
			SET(man3_new ${man3} ${man})
		ENDIF(NOT man_tmp STREQUAL "pcrecpp.3")
	ENDFOREACH(man ${man3})
	SET(man3 ${man3_new})
ENDIF(PCRE_BUILD_PCRECPP)

INSTALL(FILES ${man1} DESTINATION man/man1)
INSTALL(FILES ${man3} DESTINATION man/man3)
INSTALL(FILES ${html} DESTINATION doc/html)

# end CMakeLists.txt
