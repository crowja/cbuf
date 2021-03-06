#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cbuf.h"
#include "t/tinytest.h"
#include "t/tinyhelp.h"

static void
test_constr(void)
{
   struct cbuf *z;

   fprintf_test_info(stdout, "test_constr", NULL);
   z = cbuf_new();
   ASSERT("Constructor test", z);
   cbuf_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_get(void)
{
   struct cbuf *z;
   char        test_str[] = "Now is the time for all old dogs ...";
   char        c;
   unsigned    count = 0;

   fprintf_test_info(stdout, "test_get", NULL);
   z = cbuf_new();
   cbuf_init(z, test_str);

   while (EOF != (c = cbuf_get(z))) {
#if 0
      printf("char is %c\n", c);
#endif
      count += 1;
   }

   ASSERT_EQUALS(count, strlen(test_str));
   cbuf_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_unget1(void)
{
   struct cbuf *z;
   char        test_str1[] = "Now is the time for all old dogs ...";
   char        test_str2[] = "... sgod dlo lla rof emit eht si woN";
   char       *test_tmp = (char *) malloc((1 + strlen(test_str2)) * sizeof(char));
   char        c;
   unsigned    count;

   fprintf_test_info(stdout, "test_unget1", "cbuf_get, cbuf_unget");
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
   cbuf_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_unget2(void)
{
   struct cbuf *z;
   char        test_str[] = "ABCDEFG";
   char        c1, c2;

   fprintf_test_info(stdout, "test_unget2", "cbuf_get, cbuf_unget");
   z = cbuf_new();
   cbuf_init(z, test_str);
   c1 = cbuf_get(z);                             /* removes A */
   c2 = cbuf_get(z);                             /* removes B */
   cbuf_get(z);                                  /* removes C */
   cbuf_unget(z, 'J');                           /* prepends J */
   c1 = cbuf_get(z);                             /* removes J */
   c2 = cbuf_get(z);                             /* removes D */
   ASSERT_EQUALS(c1, 'J');
   ASSERT_EQUALS(c2, test_str[3]);
   cbuf_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_null_input(void)
{
   struct cbuf *z;
   char        c;

   fprintf_test_info(stdout, "test_null_input", NULL);
   z = cbuf_new();
   c = cbuf_get(z);
   ASSERT_EQUALS(EOF, c);
   cbuf_free(&z);
   ASSERT_EQUALS(NULL, z);
}


static void
test_init_init(void)
{
   struct cbuf *z;
   int         c;

   fprintf_test_info(stdout, "test_init_init", NULL);
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
   cbuf_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_stress_1(void)
{
   struct cbuf *z;
   int         i;

   fprintf_test_info(stdout, "test_stress_1", NULL);

   for (i = 0; i < 50000; i++) {
      z = cbuf_new();
      ASSERT_EQUALS(0, (NULL == z));
      cbuf_init(z, "Now is the time for all good dogs ...");
      ASSERT_EQUALS('N', cbuf_get(z));
      ASSERT_EQUALS('o', cbuf_get(z));
      ASSERT_EQUALS('w', cbuf_unget(z, cbuf_get(z)));
      ASSERT_EQUALS('w', cbuf_unget(z, cbuf_get(z)));
      cbuf_free(&z);
      ASSERT_EQUALS(NULL, z);
   }
}


static void
test_empty(void)
{
   struct cbuf *z;

   fprintf_test_info(stdout, "test_empty", NULL);
   z = cbuf_new();
   ASSERT_EQUALS(1, cbuf_init(z, ""));
   ASSERT_EQUALS(EOF, cbuf_get(z));
   ASSERT_EQUALS(EOF, cbuf_unget(z, 'A'));
   cbuf_free(&z);
   ASSERT_EQUALS(NULL, z);
}


static void
test_stub(void)
{
   struct cbuf *z;

   fprintf_test_info(stdout, "test_stub", NULL);
   z = cbuf_new();
   ASSERT("Test stub", z);
   cbuf_free(&z);
   ASSERT_EQUALS(NULL, z);
}


int
main(void)
{
   printf("%s\n", cbuf_version());

   test_stub();
   RUN(test_constr);
   RUN(test_get);
   RUN(test_unget1);
   RUN(test_unget2);
   RUN(test_null_input);
   RUN(test_init_init);
   RUN(test_stress_1);
   RUN(test_empty);
   return TEST_REPORT();
}
