/* A program for converting characters to UTF-8 and vice versa */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* The valid ranges for UTF-8 characters are:

0000 0000  to  0000 007f   1 byte (ascii)
0000 0080  to  0000 07ff   2 bytes
0000 0800  to  0000 ffff   3 bytes
0001 0000  to  001f ffff   4 bytes
0020 0000  to  03ff ffff   5 bytes
0400 0000  to  7fff ffff   6 bytes
*/


static const int utf8_table1[] = {
  0x0000007f, 0x000007ff, 0x0000ffff, 0x001fffff, 0x03ffffff, 0x7fffffff};  

static const int utf8_table2[] = {
  0, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc};  
  
static const int utf8_table3[] = {
  0xff, 0x1f, 0x0f, 0x07, 0x03, 0x01};  
  
static const unsigned char utf8_table4[] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,4,4,4,4,5,5,6,6 };


/*************************************************
*       Convert character value to UTF-8         *
*************************************************/

/* This function takes an integer value in the range 0 - 0x7fffffff
and encodes it as a UTF-8 character in 1 to 6 bytes.

Arguments:   
  cvalue     the character value 
  buffer     pointer to buffer for result - at least 6 bytes long
  
Returns:     number of characters placed in the buffer
             -1 if input character is negative  
             0 if input character is positive but too big (only when
             int is longer than 32 bits) 
*/

int
ord2utf8(int cvalue, unsigned char *buffer)
{
register int i, j;
for (i = 0; i < sizeof(utf8_table1)/sizeof(int); i++)
  if (cvalue <= utf8_table1[i]) break;
if (i >= sizeof(utf8_table1)/sizeof(int)) return 0;
if (cvalue < 0) return -1;
buffer += i;
for (j = i; j > 0; j--)
 {
 *buffer-- = 0x80 | (cvalue & 0x3f);
 cvalue >>= 6;
 }
*buffer = utf8_table2[i] | cvalue;
return i + 1;
}



/*************************************************
*            Convert UTF-8 string to value       *
*************************************************/

/* This function takes one or more bytes that represents a UTF-8 character,
and returns the value of the character.

Argument:  
  buffer   a pointer to the byte vector
  vptr     a pointer to an int to receive the value 

Returns:   >  0 => the number of bytes consumed
           -6 to 0 => malformed UTF-8 character at offset = (-return)
*/

int
utf82ord(unsigned char *buffer, int *vptr)
{
int c = *buffer++;
int d = c;
int i, j, s;

for (i = -1; i < 6; i++)               /* i is number of additional bytes */
  {
  if ((d & 0x80) == 0) break;
  d <<= 1;
  }

if (i == -1) { *vptr = c; return 1; }  /* ascii character */
if (i == 0 || i == 6) return 0;        /* invalid UTF-8 */

/* i now has a value in the range 1-5 */

s = 6*i;
d = (c & utf8_table3[i]) << s;

for (j = 0; j < i; j++)
  {
  c = *buffer++;
  if ((c & 0xc0) != 0x80) return -(j+1);
  s -= 6;
  d |= (c & 0x3f) << s;
  }

/* Check that encoding was the correct unique one */

for (j = 0; j < sizeof(utf8_table1)/sizeof(int); j++)
  if (d <= utf8_table1[j]) break;
if (j != i) return -(i+1);

/* Valid value */

*vptr = d;
return i+1;
}




/*************************************************
*                 Main Program                   *
*************************************************/


int
main(int argc, char **argv)
{
int i = 1;
int show = 0;
unsigned char buffer[8];

if (strcmp(argv[1], "-s") == 0)
  {
  show = 1;
  i = 2;
  }   

for (; i < argc; i++)
  {
  unsigned char *x = argv[i];
  if (strncmp(x, "0x", 2) == 0)
    {
    int j; 
    int d = strtol(x+2, NULL, 16);
    int rc = ord2utf8(d, buffer);
    printf("0x%08x => ", d); 
    if (rc <= 0) printf("*** Error %d ***", rc); else 
      {
      for (j = 0; j < rc; j++) printf("%02x ", buffer[j]);
      if (show)
        {
        printf(">");
        for (j = 0; j < rc; j++) printf("%c", buffer[j]);
        printf("<"); 
        }  
      } 
    printf("\n");   
    }
  else
    {
    int d, rc; 
    int j = 0;
    int y = 0; 
    int z = 0; 
    for (;;) 
      { 
      while (*x == ' ') x++; 
      if (*x == 0 && !z) break;
      if (!isxdigit(*x)) 
        {
        printf("Malformed hex string: %s\n", argv[i]);
        j = -1;
        break;    
        } 
      y = y * 16 + tolower(*x) - ((isdigit(*x))? '0' : 'W');
      x++; 
      if (z)
        { 
        buffer[j++] = y;
        y = 0;
        }
      z ^= 1;     
      } 
    if (j < 0) continue;     
    buffer[j] = 0;   
    rc = utf82ord(buffer, &d);
    if (rc > 0) printf("0x%08x <= %s\n", d, argv[i]); 
      else printf("Error %d <= %s\n", rc, argv[i]); 
    }       
  } 
return 0;
}

/* End */
