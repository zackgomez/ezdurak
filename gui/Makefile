include ../Makefile.inc

CXXFLAGS = $(PRJCPPFLAGS) `sdl-config --cflags`
OBJLIBS  = ../libezdurakgui.a
OBJS = GUIListener.o GUIImpl.o GUIString.o GUIPlayer.o GUICard.o \
       GUIPlayerView.o GUIHumanView.o

.PHONY: all
all: $(OBJLIBS)

../libezdurakgui.a: $(OBJS)
	echo $(ARFLAGS)
	$(AR) $(ARFLAGS) ../libezdurakgui.a $?
	$(RANLIB) ../libezdurakgui.a

clean:
	$(ECHO) clean up gui
	-$(RM) -f $(OBJS)

.PHONY: force_look
force_look: true
