CPPFLAGS = -Wall -O0 -g -Wno-sign-compare
OBJS 	 = Card.o Deck.o GamePlayer.o Game.o

all: ezdurak test

ezdurak: $(OBJS) main.o
	$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

test: $(OBJS) test.o
	$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

run: test
	./test

clean:
	rm -f *.o ezdurak test
