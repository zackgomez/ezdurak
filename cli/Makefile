include ../Makefile.inc

CXXFLAGS = $(PRJCPPFLAGS) `sdl-config --cflags`
OBJLIBS  = ../libezdurakcli.a
OBJS = CLIListener.o CLIPlayer.o

.PHONY: all
all: $(OBJLIBS)

../libezdurakcli.a: $(OBJS)
	echo $(ARFLAGS)
	$(AR) $(ARFLAGS) ../libezdurakcli.a $?
	$(RANLIB) ../libezdurakcli.a

clean:
	$(ECHO) clean up cli
	-$(RM) -f $(OBJS)

.PHONY: force_look
force_look: true
