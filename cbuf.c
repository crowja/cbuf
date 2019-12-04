/**
 *  @file cbuf.c
 *  @version 0.1.0-dev0
 *  @date %TODAY%
 *  @copyright %COPYRIGHT%
 *  @brief FIXME
 *  @details FIXME
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cbuf.h"

#ifdef  _IS_NULL
#undef  _IS_NULL
#endif
#define _IS_NULL(p)   ((NULL == (p)) ? (1) : (0))

#ifdef  _FREE
#undef  _FREE
#endif
#define _FREE(p)      ((NULL == (p)) ? (0) : (free((p)), (p) = NULL))

struct cbuf {
   char       *x;
   unsigned    pos;
};

/*** cbuf_new() ***/

struct cbuf *
cbuf_new(void)
{
   struct cbuf *tp;

   tp = (struct cbuf *) malloc(sizeof(struct cbuf));
   if (_IS_NULL(tp))
      return NULL;

   tp->x = NULL;
   tp->pos = EOF;

   return tp;
}

/*** cbuf_free() ***/

void
cbuf_free(struct cbuf *p)
{
   _FREE(p->x);
   _FREE(p);
}

/*** cbuf_version() ***/

const char *
cbuf_version(void)
{
   return "0.1.0-dev0";
}

/*** cbuf_init() ***/

int
cbuf_init(struct cbuf *p, const char *str)
{
   if (_IS_NULL(str)) {
      _FREE(p->x);
      p->x = NULL;
      return 0;
   }

   p->x = realloc(p->x, sizeof(char) * (1 + strlen(str)));

   if (!_IS_NULL(p->x)) {
      strcpy(p->x, str);
      p->pos = 0;
      return 1;
   }

   else
      return 0;
}

/*** cbuf_get() ***/

int
cbuf_get(struct cbuf *p)
{
   if (_IS_NULL(p->x))                           /* TODO combine these checks later */
      return EOF;

   if (p->pos == strlen(p->x))
      return EOF;

   return (int) (p->x)[p->pos++];

}

/*** cbuf_unget() ***/

int
cbuf_unget(struct cbuf *p, char c)
{
   if (_IS_NULL(p->x))                           /* TODO combine these checks later */
      return EOF;

   if (p->pos == 0)
      return EOF;

   (p->x)[--p->pos] = c;
   return c;
}

#undef _IS_NULL
#undef _FREE
