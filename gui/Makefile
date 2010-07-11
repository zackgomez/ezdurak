include ../Makefile.inc

CXXFLAGS = $(PRJCXXFLAGS) `sdl-config --cflags` -I..
OBJLIBS  = ../libezdurakgui.a
OBJS = GUIListener.o GUIImpl.o GUIString.o GUIPlayer.o GUICard.o \
       GUIPlayerView.o GUIHumanView.o

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
