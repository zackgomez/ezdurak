include ../Makefile.inc

CXXFLAGS = $(PRJCXXFLAGS) -I..
OBJLIBS  = ../libezdurakcore.a
OBJS = Game.o Card.o Deck.o ScoreKeeper.o PlayerImpl.o

.PHONY: all
all: $(OBJLIBS)

../libezdurakcore.a: $(OBJS)
	$(ECHO) $(AR) $(ARFLAGS) ../libezdurakcore.a $^
	$(AR) $(ARFLAGS) ../libezdurakcore.a $^
	$(ECHO) $(RANLIB) ../libezdurakcore.a
	$(RANLIB) ../libezdurakcore.a

clean:
	$(ECHO) clean up core
	-$(RM) -f $(OBJS)

.PHONY: force_look
force_look:
	true
