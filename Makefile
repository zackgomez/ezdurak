include Makefile.inc

DIRS	= gui core cli
EXE	= ezdurak-gui ezdurak-cli
OBJS	= guimain.o
OBJLIBS = libezdurakgui.a libezdurakcore.a libezdurakcli.a
LIBS	= -L. -lezdurakgui
GUILIBS = `sdl-config --libs` -lSDL_ttf -lSDL_image -lGL

all: $(EXE)

ezdurak-gui: guimain.o libezdurakcore.a libezdurakgui.a
	$(ECHO) $(LD) -o $(EXE) $(OBJS) $(LIBS)
	$(LD) -o $@ $^ $(LIBS) $(GUILIBS)

ezdurak-cli: climain.o libezdurakcore.a libezdurakcli.a
	$(ECHO) $(LD) -o $(EXE) $(OBJS) $(LIBS)
	$(LD) -o $@ $^ $(LIBS)


libezdurakgui.a: force_look
	$(ECHO) looking into gui : $(MAKE) $(MFLAGS)
	cd gui; $(MAKE) $(MFLAGS)

libezdurakcore.a: force_look
	$(ECHO) looking into core : $(MAKE) $(MFLAGS)
	cd core; $(MAKE) $(MFLAGS)

libezdurakcli.a: force_look
	$(ECHO) looking into cli : $(MAKE) $(MFLAGS)
	cd cli; $(MAKE) $(MFLAGS)

clean:
	$(ECHO) $(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
	$(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: force_look
force_look:
	true
