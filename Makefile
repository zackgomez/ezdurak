CPPFLAGS = -Wall -O0 -g -Wno-sign-compare -std=c++0x
OBJS 	 = obj/Card.o obj/Deck.o obj/Player.o obj/Game.o obj/CLIPlayer.o \
	   obj/CLIListener.o obj/AIPlayer.o obj/EHPlayer.o

all: ezdurak test

ezdurak: $(OBJS) obj/main.o
	$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

test: $(OBJS) obj/test.o
	$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

obj/%.o: %.cpp 
	$(CXX) -c $(CPPFLAGS) -o $@ $<


run: test
	./test

clean:
	rm -rf ezdurak test

obj:
	mkdir -p obj
