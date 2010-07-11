include ../Makefile.inc

CXXFLAGS = $(PRJCPPFLAGS) -I..
OBJLIBS  = ../libezdurakcore.a
OBJS = Game.o Card.o Player.o Deck.o ScoreKeeper.o

.PHONY: all
all: $(OBJLIBS)

../libezdurakcore.a: $(OBJS)
	echo $(ARFLAGS)
	$(AR) $(ARFLAGS) ../libezdurakcore.a $?
	$(RANLIB) ../libezdurakcore.a

clean:
	$(ECHO) clean up core
	-$(RM) -f $(OBJS)

.PHONY: force_look
force_look:
	true
