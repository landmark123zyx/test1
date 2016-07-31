#include ${PRE_RULES_MK}
#SUBSYSTEM=proxy_server_threadpool

MYCODETARGET = proxy_server_threadpool
MYCODETARGET1 = mainserver_thrpool
MYCODETARGET2 = proxyserver_thrpool
MYCODETARGET3 = client_thrpool
#MYCODETARGET4 = lambda
#MYCODETARGET5 = ctor_dtor
#MYCODETARGET6 = t_bind
#MYCODETARGET7 = init
#MYCODETARGET8 = func_overload
#MYCODETARGET9 = integer_sequence

# list of project libraries that are being linked

# list of source files that are needed to create module 
MYCODESOURCE1 = mainserver_thrpool.cpp
MYCODESOURCE2 = proxyserver_thrpool.cpp
MYCODESOURCE3 = client_thrpool.cpp
#MYCODESOURCE4 = lambda.cpp
#MYCODESOURCE5 = ctor_dtor.cpp
#MYCODESOURCE6 = t_bind.cpp
#MYCODESOURCE7 = init.cpp
#MYCODESOURCE8 = func_overload.cpp
#MYCODESOURCE9 = integer_sequence.cpp

# list of other files (data files, man pages, etc...)
MYCODEMAN3 = reverse_words.3

# remove RVO (Return Value Optimization)
#OPTIONS = -fno-elide-constructors
OPTIONS =
INCLUDE = . /usr/local/include ../libThreadpool
CFLAGS = $(OPTIONS) -g -DSVR4 -DPTHREAD_SAFE --std=c++17 $(INCLUDE:%=-I%)
LNKSTATICFLAGS = $(OPTIONS) -g -DSVR4 -DPTHREAD_SAFE  # for static lib (.a)
LNKDYNAMICFLAGS = -Wall # for linking dynamic lib (.so)

LIBDIRS = /usr/local/lib ../libThreadpool
#LIBS = pthread 
LIBS = 
MYCODELIBS = Threadpool

CC=g++

OBJECTS1  = $(MYCODESOURCE1:.cpp=.o)
OBJECTS2  = $(MYCODESOURCE2:.cpp=.o)
OBJECTS3  = $(MYCODESOURCE3:.cpp=.o)
OBJECTS4  = $(MYCODESOURCE4:.cpp=.o)
OBJECTS5  = $(MYCODESOURCE5:.cpp=.o)
OBJECTS6  = $(MYCODESOURCE6:.cpp=.o)
OBJECTS7  = $(MYCODESOURCE7:.cpp=.o)
OBJECTS8  = $(MYCODESOURCE8:.cpp=.o)
OBJECTS9  = $(MYCODESOURCE9:.cpp=.o)

#.c.o:
#	$(CC) -c $(CFLAGS) $<
.cpp.o:
	$(CC) -c $(CFLAGS) $<

#
# do not delete $(LIBDIRS) and $(MYCODELIBS) from the compile line
#
all:	ALL
$(MYCODETARGET):	$(MYCODETARGET1) $(MYCODETARGET2) $(MYCODETARGET3) $(MYCODETARGET4) $(MYCODETARGET5)
$(MYCODETARGET1):	$(OBJECTS1)
## link static lib (.a)
		$(CC) $(LNKSTATICFLAGS) $(OBJECTS1) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-l%) $(LIBS:%=-l%)
## link dynamic lib (.so)
#		$(CC) $(LNKDYNAMICFLAGS) $(OBJECTS1) -o $@ $(LIBDIRS:%=-L%) $(LIBS:%=-l:lib%.so) 

$(MYCODETARGET2):	$(OBJECTS2)
## link static lib (.a)
		$(CC) $(LNKSTATICFLAGS) $(OBJECTS2) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-l%) $(LIBS:%=-l%)
## link dynamic lib (.so)
#		$(CC) $(LNKDYNAMICFLAGS) $(OBJECTS2) -o $@ $(LIBDIRS:%=-L%) -l:libThreadpool.so 
#		$(CC) $(LNKDYNAMICFLAGS) $(OBJECTS2) -o $@ $(LIBDIRS:%=-L%) $(LIBS:%=-l:lib%.so) 

$(MYCODETARGET3):	$(OBJECTS3)
## link static lib (.a)
		$(CC) $(LNKSTATICFLAGS) $(OBJECTS3) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-l%) $(LIBS:%=-l%)
## link dynamic lib (.so)
#		$(CC) $(LNKDYNAMICFLAGS) $(OBJECTS3) -o $@ $(LIBDIRS:%=-L%) $(LIBS:%=-l:lib%.so) 

$(MYCODETARGET4):	$(OBJECTS4)
		$(CC) $(LNKSTATICFLAGS) $(OBJECTS4) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-l%) $(LIBS:%=-l%)
$(MYCODETARGET5):	$(OBJECTS5)
		$(CC) $(LNKSTATICFLAGS) $(OBJECTS5) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-l%) $(LIBS:%=-l%)
$(MYCODETARGET6):	$(OBJECTS6)
		$(CC) $(LNKSTATICFLAGS) $(OBJECTS6) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-l%) $(LIBS:%=-l%)
$(MYCODETARGET7):	$(OBJECTS7)
		$(CC) $(LNKSTATICFLAGS) $(OBJECTS7) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-l%) $(LIBS:%=-l%)
$(MYCODETARGET8):	$(OBJECTS8)
		$(CC) $(LNKSTATICFLAGS) $(OBJECTS8) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-l%) $(LIBS:%=-l%)
$(MYCODETARGET9):	$(OBJECTS9)
		$(CC) $(LNKSTATICFLAGS) $(OBJECTS9) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-l%) $(LIBS:%=-l%)

ALL:	LIBRARIES OTHERS
LIBRARIES:	;	@echo $@ n/a
OTHERS:	$(MYCODETARGET1) $(MYCODETARGET2) $(MYCODETARGET3) $(MYCODETARGET4) $(MYCODETARGET5) $(MYCODETARGET6) $(MYCODETARGET7) \
$(MYCODETARGET8) $(MYCODETARGET9)




#
# use this rule if creating a library
#
################################################x
#all:	ALL
#$(MYCODETARGET1).a: $(OBJECTS1)
#	ar cr $(MYCODETARGET1).a $(OBJECTS1)

#$(MYCODETARGET2).a: $(OBJECTS2)
#	ar cr $(MYCODETARGET2).a $(OBJECTS2)

#include ${POST_RULES_MK}

#
# Install in development execution environment
#
#ALL:		LIBRARIES OTHERS
#OTHERS:         ;	@echo $@ n/a 
#LIBRARIES:	$(MYCODETARGET1).a $(MYCODETARGET2).a


#old_install:	$(MYCODETARGET10).a $(MYCODETARGET1).a
#		install -c $(MYCODETARGET10).a $(MYCODE_BUILDDIR)/lib/$(MYCODETARGET1).a
#		install -c $(MYCODETARGET1).a $(MYCODE_BUILDDIR)/lib/$(MYCODETARGET2).a
#		install -c -m 444 $(MYCODESHAREHEADERS) $(MYCODE_BUILDDIR)/include
#		install -c -m 444 $(MYCODEMAN3) $(MYCODE_BUILDDIR)/man/man3

#
# Clean up all files (file beginning with ',' are created by the sccs cmd)
#
clean:
	rm -f *.o \,* core *.exe $(MYCODETARGET1)_sl.c .nMake* .make*

clobber: clean
	rm -f $(MYCODETARGET1)
