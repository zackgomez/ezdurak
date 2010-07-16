include Makefile.inc

DIRS	= gui core cli ai
EXE	= ezdurak-gui ezdurak-cli
OBJS	= guimain.o climain.o
OBJLIBS = libezdurakgui.a libezdurakcore.a libezdurakcli.a libezdurakai.a
LIBS	= -L.
GUILIBS = `sdl-config --libs` -lSDL_ttf -lSDL_image -lGL
CXXFLAGS = -I. $(PRJCXXFLAGS)

all: $(EXE)

ezdurak-gui: guimain.o libezdurakcore.a libezdurakgui.a libezdurakai.a
	g++ -o ezdurak-gui guimain.o libezdurakgui.a libezdurakcore.a -lSDL -lSDL_ttf -lSDL_image -lGL libezdurakai.a

ezdurak-cli: climain.o libezdurakcore.a libezdurakcli.a libezdurakai.a
	$(ECHO) $(LD) -o $@ $^ $(LIBS) core/PlayerImpl.o
	$(LD) -o $@ $^ $(LIBS) core/PlayerImpl.o


libezdurakgui.a: force_look
	$(ECHO) looking into gui : $(MAKE) $(MFLAGS)
	cd gui; $(MAKE) $(MFLAGS)

libezdurakcore.a: force_look
	$(ECHO) looking into core : $(MAKE) $(MFLAGS)
	cd core; $(MAKE) $(MFLAGS)

libezdurakcli.a: force_look
	$(ECHO) looking into cli : $(MAKE) $(MFLAGS)
	cd cli; $(MAKE) $(MFLAGS)

libezdurakai.a: force_look
	$(ECHO) looking into ai : $(MAKE) $(MFLAGS)
	cd ai; $(MAKE) $(MFLAGS)

clean:
	$(ECHO) $(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
	$(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: force_look
force_look:
	true
