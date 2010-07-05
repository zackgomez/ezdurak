CPPFLAGS   = -Wall -O0 -g -Wno-sign-compare -std=c++0x
COMMONOBJS = obj/Card.o obj/Deck.o obj/Player.o obj/Game.o obj/CLIPlayer.o \
	   obj/CLIListener.o obj/AIPlayer.o obj/ScoreKeeper.o
GUIOBJS    = obj/GUIListener.o obj/guimain.o

all: ezdurak test ezdurak-gui

ezdurak: $(OBJS) obj/main.o
	$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

ezdurak-gui: $(OBJS) $(GUIOBJS)
	$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) $(GUILDFLAYS) -o $@

obj/%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) -o $@ $<


run: test
	./test

clean:
	rm -rf obj/* ezdurak test

obj:
	mkdir -p obj
