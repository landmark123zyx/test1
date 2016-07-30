#include ${PRE_RULES_MK}
#SUBSYSTEM=libThreadpool

TARGET1 = libThreadpool
#TARGET2 = libalgohelper
TESTTARGET=testThreadpool

# list of project libraries that are being linked

# list of source files that are needed to create module 
SOURCE1 = thrpool1.cpp # other1.cpp other2.cpp
#SOURCE2 = algoHelper.cpp 
TESTSOURCES = thrpoolTest.cpp

# list of other files (data files, man pages, etc...)
MAN3 = 


#OPTIONS = $(COMPATMODE) -xildoff -template=no%wholeclass -library=rwtools7_std
OPTIONS = 
INCLUDE = . /usr/local/include 
CFLAGS = $(OPTIONS) -g -DSVR4 -DPTHREAD_SAFE -std=c++17 $(INCLUDE:%=-I%)
LNKSTATICFLAGS = $(OPTIONS) -g -DSVR4 -DPTHREAD_SAFE  # for linking static lib (.a)
BUILDDYNAMICFLAGS = $(OPTIONS) -g -DSVR4 -DPTHREAD_SAFE -Wall -shared -fPIC  # for building dynamic lib (.so)
LNKDYNAMICFLAGS = -Wall # for linking dynamic lib (.so)
LIBDIRS = .
LIBS =

CC=g++

OBJECTS1  = $(SOURCE1:.cpp=.o)
#OBJECTS2  = $(SOURCE2:.cpp=.o)
TESTOBJS = $(TESTSOURCES:.cpp=.o)

.cpp.o:
	$(CC) -c $(CFLAGS) $<

.h.cpp :
	@touch $*.cpp

all:	ALL
#
# use this rule if creating a static library (.a)
#
################################################x
$(TARGET1).a: $(OBJECTS1)
	ar cr $(TARGET1).a $(OBJECTS1)
##$(TARGET2).a: $(OBJECTS2)
##	ar cr $(TARGET2).a $(OBJECTS2)

#
# use this rule if creating a dynamic library (.so)
#
################################################x
#$(TARGET1).so: $(OBJECTS1)
#	$(CC) $(BUILDDYNAMICFLAGS) $(OBJECTS1) -o $(TARGET1).so

#
# do not delete $(LIBDIRS) and $(LIBS) from the compile line
#
$(TESTTARGET):	$(TESTOBJS)
# link to static lib (.a)
	$(CC) $(LNKSTATICFLAGS) $(TESTOBJS) -o $@ $(LIBDIRS:%=-L%) $(LIBS:%=-l%) -lThreadpool
# link to dynamic lib (.so)
#	$(CC)  $(LNKDYNAMICFLAGS) $(TESTOBJS) -o $@ $(LIBDIRS:%=-L%) -l:libThreadpool.so

#include ${POST_RULES_MK}

#
# Install in development execution environment
#
ALL:		LIBRARIES OTHERS
#OTHERS:         ;	@echo $@ n/a 
#LIBRARIES:	$(TARGET1).a $(TARGET2).a
OTHERS:		$(TESTTARGET)
LIBRARIES:	$(TARGET1).a 
#LIBRARIES:	$(TARGET1).so


#old_install:	$(TARGET0).a $(TARGET).a
#		install -c $(TARGET0).a $(_BUILDDIR)/lib/$(TARGET0).a
#		install -c $(TARGET).a $(_BUILDDIR)/lib/$(TARGET).a
#		install -c -m 444 $(SHAREHEADERS) $(_BUILDDIR)/include
#		install -c -m 444 $(MAN3) $(_BUILDDIR)/man/man3

#
# Clean up all files (file beginning with ',' are created by the sccs cmd)
#
clean:
	rm -f *.o \,* core *.a *.so *.exe .nMake* .make*

clobber: clean
	rm -f $(TARGET)*.a
