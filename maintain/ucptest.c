/***************************************************
* A program for testing the Unicode property table *
***************************************************/

/* Copyright (c) University of Cambridge 2006 */

/* Compile thus:
   gcc -o ucptest ucptest.c ../pcre_ucp_searchfuncs.c
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../pcre_internal.h"
#include "../ucp.h"
#include "../ucpinternal.h"


/* -------------------------------------------------------------------*/

#define CS   (char *)
#define CCS  (const char *)
#define CSS  (char **)
#define US   (unsigned char *)
#define CUS  (const unsigned char *)
#define USS  (unsigned char **)

/* -------------------------------------------------------------------*/




/*************************************************
*      Print Unicode property info for a char    *
*************************************************/

static void
print_prop(int c)
{
int fulltype, script, othercase;
int type = _pcre_ucp_findprop(c, &fulltype, &script);

printf("%04x ", c);
if (type < 0) printf("not found\n"); else
  {
  uschar *fulltypename = US"??";
  uschar *typename = US"??";
  uschar *scriptname = US"??";
  switch (type)
    {
    case ucp_C: typename = US"Control"; break;
    case ucp_L: typename = US"Letter"; break;
    case ucp_M: typename = US"Mark"; break;
    case ucp_N: typename = US"Number"; break;
    case ucp_P: typename = US"Punctuation"; break;
    case ucp_S: typename = US"Symbol"; break;
    case ucp_Z: typename = US"Separator"; break;
    }
  switch (fulltype)
    {
    case ucp_Cc: fulltypename = US"Control"; break;
    case ucp_Cf: fulltypename = US"Format"; break;
    case ucp_Cn: fulltypename = US"Unassigned"; break;
    case ucp_Co: fulltypename = US"Private use"; break;
    case ucp_Cs: fulltypename = US"Surrogate"; break;
    case ucp_Ll: fulltypename = US"Lower case letter"; break;
    case ucp_Lm: fulltypename = US"Modifier letter"; break;
    case ucp_Lo: fulltypename = US"Other letter"; break;
    case ucp_Lt: fulltypename = US"Title case letter"; break;
    case ucp_Lu: fulltypename = US"Upper case letter"; break;
    case ucp_Mc: fulltypename = US"Spacing mark"; break;
    case ucp_Me: fulltypename = US"Enclosing mark"; break;
    case ucp_Mn: fulltypename = US"Non-spacing mark"; break;
    case ucp_Nd: fulltypename = US"Decimal number"; break;
    case ucp_Nl: fulltypename = US"Letter number"; break;
    case ucp_No: fulltypename = US"Other number"; break;
    case ucp_Pc: fulltypename = US"Connector punctuation"; break;
    case ucp_Pd: fulltypename = US"Dash punctuation"; break;
    case ucp_Pe: fulltypename = US"Close punctuation"; break;
    case ucp_Pf: fulltypename = US"Final punctuation"; break;
    case ucp_Pi: fulltypename = US"Initial punctuation"; break;
    case ucp_Po: fulltypename = US"Other punctuation"; break;
    case ucp_Ps: fulltypename = US"Open punctuation"; break;
    case ucp_Sc: fulltypename = US"Currency symbol"; break;
    case ucp_Sk: fulltypename = US"Modifier symbol"; break;
    case ucp_Sm: fulltypename = US"Mathematical symbol"; break;
    case ucp_So: fulltypename = US"Other symbol"; break;
    case ucp_Zl: fulltypename = US"Line separator"; break;
    case ucp_Zp: fulltypename = US"Paragraph separator"; break;
    case ucp_Zs: fulltypename = US"Space separator"; break;
    }
  switch(script)
    {
    case ucp_Arabic:      scriptname = US"Arabic"; break;
    case ucp_Armenian:    scriptname = US"Armenian"; break;
    case ucp_Balinese:    scriptname = US"Balinese"; break;
    case ucp_Bengali:     scriptname = US"Bengali"; break;
    case ucp_Bopomofo:    scriptname = US"Bopomofo"; break;
    case ucp_Braille:     scriptname = US"Braille"; break;
    case ucp_Buginese:    scriptname = US"Buginese"; break;
    case ucp_Buhid:       scriptname = US"Buhid"; break;
    case ucp_Canadian_Aboriginal: scriptname = US"Canadian_Aboriginal"; break;
    case ucp_Cherokee:    scriptname = US"Cherokee"; break;
    case ucp_Common:      scriptname = US"Common"; break;
    case ucp_Coptic:      scriptname = US"Coptic"; break;
    case ucp_Cuneiform:   scriptname = US"Cuneiform"; break;
    case ucp_Cypriot:     scriptname = US"Cypriot"; break;
    case ucp_Cyrillic:    scriptname = US"Cyrillic"; break;
    case ucp_Deseret:     scriptname = US"Deseret"; break;
    case ucp_Devanagari:  scriptname = US"Devanagari"; break;
    case ucp_Ethiopic:    scriptname = US"Ethiopic"; break;
    case ucp_Georgian:    scriptname = US"Georgian"; break;
    case ucp_Glagolitic:  scriptname = US"Glagolitic"; break;
    case ucp_Gothic:      scriptname = US"Gothic"; break;
    case ucp_Greek:       scriptname = US"Greek"; break;
    case ucp_Gujarati:    scriptname = US"Gujarati"; break;
    case ucp_Gurmukhi:    scriptname = US"Gurmukhi"; break;
    case ucp_Han:         scriptname = US"Han"; break;
    case ucp_Hangul:      scriptname = US"Hangul"; break;
    case ucp_Hanunoo:     scriptname = US"Hanunoo"; break;
    case ucp_Hebrew:      scriptname = US"Hebrew"; break;
    case ucp_Hiragana:    scriptname = US"Hiragana"; break;
    case ucp_Inherited:   scriptname = US"Inherited"; break;
    case ucp_Kannada:     scriptname = US"Kannada"; break;
    case ucp_Katakana:    scriptname = US"Katakana"; break;
    case ucp_Kharoshthi:  scriptname = US"Kharoshthi"; break;
    case ucp_Khmer:       scriptname = US"Khmer"; break;
    case ucp_Lao:         scriptname = US"Lao"; break;
    case ucp_Latin:       scriptname = US"Latin"; break;
    case ucp_Limbu:       scriptname = US"Limbu"; break;
    case ucp_Linear_B:    scriptname = US"Linear_B"; break;
    case ucp_Malayalam:   scriptname = US"Malayalam"; break;
    case ucp_Mongolian:   scriptname = US"Mongolian"; break;
    case ucp_Myanmar:     scriptname = US"Myanmar"; break;
    case ucp_New_Tai_Lue: scriptname = US"New_Tai_Lue"; break;
    case ucp_Nko:         scriptname = US"Nko"; break;
    case ucp_Ogham:       scriptname = US"Ogham"; break;
    case ucp_Old_Italic:  scriptname = US"Old_Italic"; break;
    case ucp_Old_Persian: scriptname = US"Old_Persian"; break;
    case ucp_Oriya:       scriptname = US"Oriya"; break;
    case ucp_Osmanya:     scriptname = US"Osmanya"; break;
    case ucp_Phags_Pa:    scriptname = US"Phags_Pa"; break;
    case ucp_Phoenician:  scriptname = US"Phoenician"; break;
    case ucp_Runic:       scriptname = US"Runic"; break;
    case ucp_Shavian:     scriptname = US"Shavian"; break;
    case ucp_Sinhala:     scriptname = US"Sinhala"; break;
    case ucp_Syloti_Nagri: scriptname = US"Syloti_Nagri"; break;
    case ucp_Syriac:      scriptname = US"Syriac"; break;
    case ucp_Tagalog:     scriptname = US"Tagalog"; break;
    case ucp_Tagbanwa:    scriptname = US"Tagbanwa"; break;
    case ucp_Tai_Le:      scriptname = US"Tai_Le"; break;
    case ucp_Tamil:       scriptname = US"Tamil"; break;
    case ucp_Telugu:      scriptname = US"Telugu"; break;
    case ucp_Thaana:      scriptname = US"Thaana"; break;
    case ucp_Thai:        scriptname = US"Thai"; break;
    case ucp_Tibetan:     scriptname = US"Tibetan"; break;
    case ucp_Tifinagh:    scriptname = US"Tifinagh"; break;
    case ucp_Ugaritic:    scriptname = US"Ugaritic"; break;
    case ucp_Yi:          scriptname = US"Yi"; break;
    }

  printf("%s: %s %s", typename, fulltypename, scriptname);
  othercase = _pcre_ucp_othercase(c);
  if (othercase >= 0) printf(" %04x", othercase);
  printf("\n");
  }
}



/*************************************************
*               Main program                     *
*************************************************/

int
main(void)
{
uschar buffer[1024];
while (fgets(CS buffer, sizeof(buffer), stdin) != NULL)
  {
  uschar name[24];
  uschar *s, *t;

  printf("%s", buffer);
  s = buffer;
  while (isspace(*s)) s++;
  if (*s == 0) continue;

  for (t = name; *s != 0 && !isspace(*s); s++) *t++ = *s;
  *t = 0;
  while (isspace(*s)) s++;

  if (strcmp(CS name, "findprop") == 0)
    {
    while (*s != 0)
      {
      uschar *endptr;
      int c = strtoul(CS s, CSS(&endptr), 16);
      print_prop(c);
      s = endptr;
      while (isspace(*s)) s++;
      }
    }

  else printf("Unknown test command %s\n", name);
  }

return 0;
}

/* End */
