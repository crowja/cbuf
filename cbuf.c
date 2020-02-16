/**
 *  @file cbuf.c
 *  @version 0.2.0-dev0
 *  @date Sun Feb 16, 2020 03:56:21 PM CST
 *  @copyright 2018-2020 John A. Crow <crowja@gmail.com>
 *  @license Unlicense <http://unlicense.org/>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cbuf.h"

#ifdef  IS_NULL
#undef  IS_NULL
#endif
#define IS_NULL(p)   ((NULL == (p)) ? (1) : (0))

#ifdef  FREE
#undef  FREE
#endif
#define FREE(p)      ((NULL == (p)) ? (0) : (free((p)), (p) = NULL))

struct cbuf {
   char       *x;
   unsigned    pos;
};

struct cbuf *
cbuf_new(void)
{
   struct cbuf *tp;

   tp = (struct cbuf *) malloc(sizeof(struct cbuf));
   if (IS_NULL(tp))
      return NULL;

   tp->x = NULL;
   tp->pos = EOF;

   return tp;
}

void
cbuf_free(struct cbuf **pp)
{
   FREE((*pp)->x);
   FREE((*pp));
   *pp = NULL;
}

const char *
cbuf_version(void)
{
   return "0.2.0-dev0";
}

int
cbuf_init(struct cbuf *p, const char *str)
{
   if (IS_NULL(str)) {
      FREE(p->x);
      p->x = NULL;
      return 0;
   }

   p->x = realloc(p->x, sizeof(char) * (1 + strlen(str)));

   if (!IS_NULL(p->x)) {
      strcpy(p->x, str);
      p->pos = 0;
      return 1;
   }

   else
      return 0;
}

int
cbuf_get(struct cbuf *p)
{
   if (IS_NULL(p->x))                            /* TODO combine these checks later */
      return EOF;

   if (p->pos == strlen(p->x))
      return EOF;

   return (int) (p->x)[p->pos++];
}

int
cbuf_unget(struct cbuf *p, const char c)
{
   if (IS_NULL(p->x))                            /* TODO combine these checks later */
      return EOF;

   if (p->pos == 0)
      return EOF;

   (p->x)[--p->pos] = c;
   return c;
}

#undef IS_NULL
#undef FREE
