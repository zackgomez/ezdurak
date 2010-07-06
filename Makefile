CPPFLAGS   = -Wall -O0 -g -Wno-sign-compare -std=c++0x
COMMONOBJS = obj/Card.o obj/Deck.o obj/Player.o obj/Game.o obj/CLIPlayer.o \
	   obj/CLIListener.o obj/AIPlayer.o obj/ScoreKeeper.o
GUIOBJS    = obj/GUIListener.o obj/guimain.o
GUILDFLAGS = -lSDL -lGL -lSDL_image

all: ezdurak ezdurak-gui

ezdurak: $(COMMONOBJS) obj/climain.o
	$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

ezdurak-gui: $(COMMONOBJS) $(GUIOBJS)
	$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) $(GUILDFLAGS) -o $@

obj/%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) -o $@ $<

clean:
	rm -rf obj/* ezdurak ezdurak-gui

obj:
	mkdir -p obj
