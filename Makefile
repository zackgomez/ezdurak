CPPFLAGS = -Wall -O0 -g
OBJS 	 = test.o Card.o Deck.o GamePlayer.o

test: $(OBJS)
	$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

run: test
	./test

clean:
	rm -f *.o ezdurak test
