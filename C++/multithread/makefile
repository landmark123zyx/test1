#include ${PRE_RULES_MK}
#SUBSYSTEM=exercises

MYCODETARGET = multithread
MYCODETARGET1 = thrQ3
MYCODETARGET2 = thrQ4
MYCODETARGET3 = thrQ5
MYCODETARGET4 = thrQ6
MYCODETARGET5 = thrQ7_0
MYCODETARGET6 = thrQ7_1
MYCODETARGET7 = thrQ7_2
#MYCODETARGET8 = class_noinherited

# list of project libraries that are being linked

# list of source files that are needed to create module 
MYCODESOURCE1 = thrQ3.cpp
MYCODESOURCE2 = thrQ4.cpp
MYCODESOURCE3 = thrQ5.cpp
MYCODESOURCE4 = thrQ6.cpp
MYCODESOURCE5 = thrQ7_0.cpp
MYCODESOURCE6 = thrQ7_1.cpp
MYCODESOURCE7 = thrQ7_2.cpp
#MYCODESOURCE8 = class_noinherited.cpp

# list of other files (data files, man pages, etc...)
MYCODEMAN3 = my_thread.3


#OPTIONS = -D_GLIBCXX_HAS_GTHREADS -D_GLIBCXX_BEGIN_NAMESPACE_VERSION
#OPTIONS = -D_GLIBCXX_HAS_GTHREADS -D_GLIBCXX_USE_C99_STDINT_TR1 -D_GLIBCXX_USE_NANOSLEEP
#OPTIONS = -D_GLIBCXX_USE_NANOSLEEP
#OPTIONS = -Dnamespace__gnu_cxx
#OPTIONS = -DHIDE_EXPORTS
#OPTIONS = -fvisibility=hidden
#OPTIONS = -fvisibility=default
#INCLUDE = . /usr/local/include /usr/local/include/c++/5.2.0 
INCLUDE = . /usr/local/include/c++/5.3.0 
CFLAGS = $(OPTIONS) -g -DSVR4 -DPTHREAD_SAFE --std=c++14 $(INCLUDE:%=-I%)
LNKFLAGS = $(OPTIONS) -g -DSVR4 -DPTHREAD_SAFE

LIBDIRS = /usr/local/lib
LIBS = pthread

CC=g++-5.3.0

OBJECTS1  = $(MYCODESOURCE1:.cpp=.o)
OBJECTS2  = $(MYCODESOURCE2:.cpp=.o)
OBJECTS3  = $(MYCODESOURCE3:.cpp=.o)
OBJECTS4  = $(MYCODESOURCE4:.cpp=.o)
OBJECTS5  = $(MYCODESOURCE5:.cpp=.o)
OBJECTS6  = $(MYCODESOURCE6:.cpp=.o)
OBJECTS7  = $(MYCODESOURCE7:.cpp=.o)
OBJECTS8  = $(MYCODESOURCE8:.cpp=.o)

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
		$(CC) $(LNKFLAGS) $(OBJECTS1) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-L%) $(LIBS:%=-l%)
$(MYCODETARGET2):	$(OBJECTS2)
		$(CC) $(LNKFLAGS) $(OBJECTS2) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-L%) $(LIBS:%=-l%)
$(MYCODETARGET3):	$(OBJECTS3)
		$(CC) $(LNKFLAGS) $(OBJECTS3) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-L%) $(LIBS:%=-l%)
$(MYCODETARGET4):	$(OBJECTS4)
		$(CC) $(LNKFLAGS) $(OBJECTS4) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-L%) $(LIBS:%=-l%)
$(MYCODETARGET5):	$(OBJECTS5)
		$(CC) $(LNKFLAGS) $(OBJECTS5) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-L%) $(LIBS:%=-l%)
$(MYCODETARGET6):	$(OBJECTS6)
		$(CC) $(LNKFLAGS) $(OBJECTS6) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-L%) $(LIBS:%=-l%)
$(MYCODETARGET7):	$(OBJECTS7)
		$(CC) $(LNKFLAGS) $(OBJECTS7) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-L%) $(LIBS:%=-l%)
$(MYCODETARGET8):	$(OBJECTS8)
		$(CC) $(LNKFLAGS) $(OBJECTS8) -o $@ $(LIBDIRS:%=-L%) $(MYCODELIBS:%=-L%) $(LIBS:%=-l%)

ALL:	LIBRARIES OTHERS
LIBRARIES:	;	@echo $@ n/a
OTHERS:	$(MYCODETARGET1) $(MYCODETARGET2) $(MYCODETARGET3) $(MYCODETARGET4) $(MYCODETARGET5) $(MYCODETARGET6) $(MYCODETARGET7) \
$(MYCODETARGET8)




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
	rm -f *.o \,* core $(MYCODETARGET1)_sl.c .nMake* .make*

clobber: clean
	rm -f $(MYCODETARGET1)
