#ifndef PCRECPP_INTERNAL_H
#define PCRECPP_INTERNAL_H

/* When compiling a DLL for Windows, the exported symbols have to be declared
using some MS magic. I found some useful information on this web page:
http://msdn2.microsoft.com/en-us/library/y4h7bcy6(VS.80).aspx. According to the
information there, using __declspec(dllexport) without "extern" we have a
definition; with "extern" we have a declaration. The settings here override the
setting in pcrecpp.h; it defines only PCRECPP_EXP_DECL, which is all that is
needed for applications (they just import the symbols). We use:

  PCRECPP_EXP_DECL       for declarations
  PCRECPP_EXP_DEFN       for definitions of exported functions
  PCRECPP_EXP_DATA_DEFN  for definitions of exported variables

The reason for the two DEFN macros is that in non-Windows environments, one
does not want to have "extern" before variable definitions because it leads to
compiler warnings. So we distinguish between functions and variables. In
Windows, the two should always be the same.

The reason for wrapping this in #ifndef PCRECPP_EXP_DECL is so that pcretest,
which is an application, but needs to import this file in order to "peek" at
internals, can #include pcre.h first to get an application's-eye view.

In principle, people compiling for non-Windows, non-Unix-like (i.e. uncommon,
special-purpose environments) might want to stick other stuff in front of
exported symbols. That's why, in the non-Windows case, we set PCRECPP_EXP_DEFN and
PCRECPP_EXP_DATA_DEFN only if they are not already set. */

#ifndef PCRECPP_EXP_DECL
#  ifdef _WIN32
#    ifndef PCRECPP_STATIC
#      define PCRECPP_EXP_DECL       extern __declspec(dllexport)
#      define PCRECPP_EXP_DEFN       __declspec(dllexport)
#      define PCRECPP_EXP_DATA_DEFN  __declspec(dllexport)
#    else
#      define PCRECPP_EXP_DECL       extern
#      define PCRECPP_EXP_DEFN
#      define PCRECPP_EXP_DATA_DEFN
#    endif
#  else
#    define PCRECPP_EXP_DECL
#    define PCRECPP_EXP_DEFN
#    define PCRECPP_EXP_DATA_DEFN
#  endif
#endif

#endif

/* End of pcrecpp_internal.h */
