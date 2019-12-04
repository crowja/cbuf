#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cbuf.h"
#include "t/tinytest.h"

#ifdef  _COLOR_CODE
#undef  _COLOR_CODE
#endif
#define _COLOR_CODE       0x1B

#ifdef  _COLOR_RED
#undef  _COLOR_RED
#endif
#define _COLOR_RED        "[1;31m"

#ifdef  _COLOR_GREEN
#undef  _COLOR_GREEN
#endif
#define _COLOR_GREEN      "[1;32m"

#ifdef  _COLOR_YELLOW
#undef  _COLOR_YELLOW
#endif
#define _COLOR_YELLOW     "[1;33m"

#ifdef  _COLOR_RESET
#undef  _COLOR_RESET
#endif
#define _COLOR_RESET      "[0m"


static void
_printf_test_name(char *name, char *info)
{
   printf("%c%s%s%c%s", _COLOR_CODE, _COLOR_YELLOW, name, _COLOR_CODE, _COLOR_RESET);

   if (NULL != info)
      printf(" [%s]\n", info);
   else
      printf("\n");
}


static void
test_constr(void)
{
   struct cbuf *z;

   _printf_test_name("test_constr()", NULL);

   z = cbuf_new();
   ASSERT("Constructor test", z);
   cbuf_free(z);
}


static void
test_get(void)
{
   struct cbuf *z;
   char        test_str[] = "Now is the time for all old dogs ...";
   char        c;
   unsigned    count = 0;

   _printf_test_name("test_get()", NULL);

   z = cbuf_new();
   cbuf_init(z, test_str);

   while (EOF != (c = cbuf_get(z))) {
#if 0
      printf("char is %c\n", c);
#endif
      count += 1;
   }

   ASSERT_EQUALS(count, strlen(test_str));

   cbuf_free(z);
}


static void
test_unget(void)
{
   struct cbuf *z;
   char        test_str1[] = "Now is the time for all old dogs ...";
   char        test_str2[] = "... sgod dlo lla rof emit eht si woN";
   char       *test_tmp = (char *) malloc((1 + strlen(test_str2)) * sizeof(char));
   char        c;
   unsigned    count;

   _printf_test_name("test_unget()", NULL);

   z = cbuf_new();
   cbuf_init(z, test_str1);

   /* First exhaust the initial buffer */
   while (EOF != (c = cbuf_get(z))) {
   }

   /* Now unget the initial text starting from the end of the cbuf */
   count = 0;
   while (EOF != (c = cbuf_unget(z, test_str1[count]))) {
      count++;
   }

   /* Read it again, this time saving the results for comparison */
   count = 0;
   while (EOF != (c = cbuf_get(z))) {
      test_tmp[count++] = c;
   }
   test_tmp[count] = '\0';

   ASSERT_STRING_EQUALS(test_tmp, test_str2);

#if 0
   /* And print it out */
   while (EOF != (c = cbuf_get(z))) {
      printf("char is %c\n", c);
   }

   printf("TMP ***%s***\n", test_tmp);
   printf("2   ***%s***\n", test_str2);

   printf("char is %c\n", c);
   count += 1;
#endif

   free(test_tmp);
   cbuf_free(z);
}


static void
test_null_input(void)
{
   struct cbuf *z;
   char        c;

   _printf_test_name("test_null_input()", NULL);

   z = cbuf_new();

   c = cbuf_get(z);
   ASSERT_EQUALS(EOF, c);

   cbuf_free(z);
}


static void
test_init_init(void)
{
   struct cbuf *z;
   int         c;

   _printf_test_name("test_init_init()", NULL);

   z = cbuf_new();
   cbuf_init(z, "ABCDE FGHIJ KLMNO PQRST UVWXY Z");
   cbuf_init(z, NULL);
   c = cbuf_unget(z, '1');
   ASSERT_EQUALS(EOF, c);
   c = cbuf_unget(z, '2');
   ASSERT_EQUALS(EOF, c);
   c = cbuf_get(z);
   ASSERT_EQUALS(EOF, c);
   c = cbuf_get(z);
   ASSERT_EQUALS(EOF, c);
   c = cbuf_unget(z, '2');
   ASSERT_EQUALS(EOF, c);

   cbuf_free(z);
}

static void
test_stress_1(void)
{
   struct cbuf *z;
   int         i;

   _printf_test_name("test_stress_1()", NULL);

   for (i = 0; i < 50000; i++) {
      z = cbuf_new();
      ASSERT_EQUALS(0, (NULL == z));
      cbuf_init(z, "Now is the time for all good dogs ...");
      ASSERT_EQUALS('N', cbuf_get(z));
      ASSERT_EQUALS('o', cbuf_get(z));
      ASSERT_EQUALS('w', cbuf_unget(z, cbuf_get(z)));
      ASSERT_EQUALS('w', cbuf_unget(z, cbuf_get(z)));
      cbuf_free(z);
   }
}


static void
test_empty(void)
{
   struct cbuf *z;

   _printf_test_name("test_empty()", NULL);

   z = cbuf_new();
   ASSERT_EQUALS(1, cbuf_init(z, ""));
   ASSERT_EQUALS(EOF, cbuf_get(z));
   ASSERT_EQUALS(EOF, cbuf_unget(z, 'A'));
   cbuf_free(z);
}


static void
test_stub(void)
{
   struct cbuf *z;

   _printf_test_name("test_stub()", NULL);

   z = cbuf_new();
   ASSERT("Test stub", z);
   cbuf_free(z);
}


int
main(void)
{
   printf("%s\n", cbuf_version());

   test_stub();
   RUN(test_constr);
   RUN(test_get);
   RUN(test_unget);
   RUN(test_null_input);
   RUN(test_init_init);
   RUN(test_stress_1);
   RUN(test_empty);

   return TEST_REPORT();
}
