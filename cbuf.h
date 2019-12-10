/**
 *  @file cbuf.h
 *  @version 0.1.3-dev0
 *  @date Tue Dec 10 10:16:11 CST 2019
 *  @copyright %COPYRIGHT%
 *  @brief Equivalents of fgetc() and ungetc() using a string.
 *  @details Provides the equivalents of fgetc() and ungetc() using a string
 *  as input rather than a stream.
 */

#ifndef _CBUF_H_
#define _CBUF_H_

#ifdef  _PACKAGE_NAME
#undef  _PACKAGE_NAME
#endif
#define _PACKAGE_NAME "cbuf"

struct cbuf;                                /* intentionally opaque */

/**
 *  @brief Constructor for a cbuf object.
 *  @details Creates a cbuf object. Free later with cbuf_free().
 *  @returns On success returns a pointer to a new cbuf object, NULL otherwise.
 */
struct cbuf *cbuf_new(void);

/**
 *  @brief Clean up and free a cbuf structure.
 */
void        cbuf_free(struct cbuf **pp);

/**
 *  @brief Return the version of this package.
 */
const char *cbuf_version(void);

/**
 *  @brief Get the next character from a cbuf object.
 *  @details Effectively fgetc() on the cbuf object.
 *  @param p[in,out] Pointer to a cbuf object
 *  @returns Value of the next character in the cbuf (char cast as an int
 *  similar to fgetc()) or EOF at the end of the cbuf.
 */
int         cbuf_get(struct cbuf *p);

/**
 *
 *  @brief Initialize a cbuf object.
 *  @details Initialize a cbuf object with the value of the character string
 *  to use.
 *  @param p[in,out] Pointer to a cbuf object.
 *  @param x[in] Character string. An internal copy will be made and freed
 *  by cbuf_free().
 *  @returns On success returns nonzero, zero otherwise.
 */
int         cbuf_init(struct cbuf *p, const char *x);

/**
 *  @brief Unget a character from a cbuf object.
 *  @details Effectively ungetc() on the cbuf object. Cbuf_unget(p, c) attempts
 *  to put c as the next character in the stream. Multiple cbuf_ungets can be
 *  made, but never in a way to exceed the original buffer size.
 *  @param p[in,out] Pointer to a cbuf object.
 *  @param c[in] Character to position as the next character in the stream.
 *  @returns On success returns the character positioned as the next 
 *  character for cbuf_get(), EOF otherwise.
 */
int         cbuf_unget(struct cbuf *p, const char c);

#endif
