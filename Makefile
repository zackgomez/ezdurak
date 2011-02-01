include Makefile.inc

DIRS	= gui core cli ai www net
EXE	= ezdurak-gui ezdurak-cli clientmain servermain
OBJS	= guimain.o climain.o clientmain.o servermain.o
OBJLIBS = libezdurakgui.a libezdurakcore.a libezdurakcli.a libezdurakai.a libezduraknet.a
LIBS	= -L. -lpthread
GUILIBS = `sdl-config --libs` -lSDL_ttf -lSDL_image -lGL
CXXFLAGS = -I. $(PRJCXXFLAGS)

all: $(EXE)

ezdurak-gui: guimain.o libezduraknet.a libezdurakcore.a libezdurakgui.a libezdurakai.a
	$(LD) -o $@ $^ $(LIBS) $(GUILIBS) -lezdurakcore -lezduraknet

ezdurak-cli: climain.o libezdurakcore.a libezdurakcli.a libezdurakai.a
	$(LD) -o $@ $^ $(LIBS) core/PlayerImpl.o

clientmain: clientmain.o libezduraknet.a libezdurakcore.a libezdurakai.a libezdurakcli.a
	$(LD) -o $@ $^ $(LIBS) core/PlayerImpl.o

servermain: servermain.o libezduraknet.a libezdurakcore.a libezdurakai.a libezdurakcli.a
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

libezduraknet.a: force_look
	$(ECHO) looking into net : $(MAKE) $(MFLAGS)
	cd net; $(MAKE) $(MFLAGS)

clean:
	$(ECHO) $(RM) -rf $(EXE) $(OBJS) $(OBJLIBS) ezdurak-linux
	$(RM) -rf $(EXE) $(OBJS) $(OBJLIBS) ezdurak-linux
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

dist: ezdurak-gui
	mkdir -p ezdurak-linux
	mkdir -p ezdurak-linux/resources
	cp resources/* ezdurak-linux/resources
	cp ezdurak-gui ezdurak-linux/ezdurak
	mkdir -p dist
	tar -czvf dist/ezdurak-linux.tar.gz ezdurak-linux

deploy: dist force_look
	cd www; $(MAKE) deploy

docs: Doxyfile
	doxygen

.PHONY: force_look
force_look:
	true
