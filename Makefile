SHELL = /bin/sh

GCC_STRICT_FLAGS = -pedantic -ansi -W -Wall -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -O2
GCC_SANITIZE_FLAGS = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
OTHER_SOURCE =
OTHER_INCLUDE = -I.
CPPFLAGS = -I. $(OTHER_INCLUDE)
CFLAGS = $(GCC_STRICT_FLAGS) 
LDFLAGS =
LDFLAGS_EFENCE = -L/usr/local/lib -lefence $(LDFLAGS)
#VALGRIND_FLAGS = --verbose --leak-check=full --undef-value-errors=yes --track-origins=yes
VALGRIND_FLAGS =  --leak-check=summary --undef-value-errors=yes --track-origins=yes

INDENT_FLAGS = -TFILE -Tsize_t -Tuint8_t -Tuint16_t -Tuint32_t -Tuint64_t

.PHONY: check vcheck scheck echeck indent stamp clean

TESTS = t/test

cbuf.o: cbuf.c cbuf.h
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ cbuf.c

check:
	@for i in $(TESTS); \
	do \
	  echo "--------------------"; \
	  echo "Running test $$i ..."; \
	  ( $(CC)    $(CPPFLAGS) $(OTHER_INCLUDE) $(CFLAGS) $(OTHER_SOURCE) \
		-o t/a.out $$i.c cbuf.c $(LDFLAGS) ) \
	  && ( t/a.out ); \
	done 

vcheck:
	@for i in $(TESTS); \
	do \
	  echo "--------------------"; \
	  echo "Running test $$i ..."; \
	  ( $(CC) -g $(CPPFLAGS) $(OTHER_INCLUDE) $(CFLAGS) $(OTHER_SOURCE) \
		-o t/a.out $$i.c cbuf.c $(LDFLAGS) ) \
	  && ( valgrind $(VALGRIND_FLAGS) t/a.out ); \
	done 

scheck:
	@for i in $(TESTS); \
	do \
	  echo "--------------------"; \
	  echo "Running test $$i ..."; \
	  ( $(CC) -g $(CPPFLAGS) $(OTHER_INCLUDE) $(CFLAGS) $(GCC_SANITIZE_FLAGS) $(OTHER_SOURCE) \
		-o t/a.out $$i.c cbuf.c $(LDFLAGS) ) \
	  && ( t/a.out ); \
	done 

echeck:
	@for i in $(TESTS); \
	do \
	  echo "--------------------"; \
	  echo "Running test $$i ..."; \
	  ( $(CC)    $(CPPFLAGS) $(OTHER_INCLUDE) $(CFLAGS) $(OTHER_SOURCE) \
		-o t/a.out $$i.c cbuf.c $(LDFLAGS_EFENCE) ) \
	  && ( LD_PRELOAD=libefence.so ./t/a.out ); \
	done 

indent: stamp
	@indent $(INDENT_FLAGS) cbuf.c
	@indent $(INDENT_FLAGS) cbuf.h
	@indent $(INDENT_FLAGS) t/test.c

stamp:
	@bash stamper.bash cbuf.c
	@bash stamper.bash cbuf.h

clean:
	@/bin/rm -f *.o *~ *.BAK *.bak core.*
	@/bin/rm -f t/*.o t/*~ t/*.BAK t/*.bak t/core.* t/a.out
