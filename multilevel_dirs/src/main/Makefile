vpath %.so $(TOPDIR)lib

TARGET:=make_test
EXEPATH:=$(TOPDIR)

TARGET_LIBS:=libmylib.so

CPPFLAGS:= -I $(TOPDIR)inc
CFLAGS:= -Wl,-rpath=$(TOPDIR)lib
LDFLAGS:= -L $(TOPDIR)lib 
LDFLAGS+= -lmylib

include $(TOPDIR)Makefile.env
