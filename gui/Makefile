include ../Makefile.inc

CXXFLAGS = $(PRJCXXFLAGS) `sdl-config --cflags` -I..
OBJLIBS  = ../libezdurakgui.a
OBJS = GUIApp.o GUIString.o GUIPlayer.o GUICard.o InGameState.o GameOverState.o\
       GUIPlayerView.o GUIHumanView.o HumanCardHolder.o PlayedCardsView.o\
       Animation.o

.PHONY: all
all: $(OBJLIBS)

../libezdurakgui.a: $(OBJS)
	$(ECHO) $(AR) $(ARFLAGS) ../libezdurakgui.a $?
	$(AR) $(ARFLAGS) ../libezdurakgui.a $?
	$(ECHO) $(RANLIB) ../libezdurakgui.a
	$(RANLIB) ../libezdurakgui.a

clean:
	$(ECHO) clean up gui
	-$(RM) -f $(OBJS)

.PHONY: force_look
force_look: true
