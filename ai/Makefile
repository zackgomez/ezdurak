include ../Makefile.inc

CXXFLAGS = $(PRJCPPFLAGS) -I..
OBJLIBS  = ../libezdurakai.a
OBJS = AIPlayer.o

.PHONY: all
all: $(OBJLIBS)

../libezdurakai.a: $(OBJS)
	echo $(ARFLAGS)
	$(AR) $(ARFLAGS) ../libezdurakai.a $?
	$(RANLIB) ../libezdurakai.a

clean:
	$(ECHO) clean up ai
	-$(RM) -f $(OBJS)

.PHONY: force_look
force_look: true
