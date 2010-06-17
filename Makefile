CPPFLAGS = -Wall -O0 -g
OBJS 	 = test.o card.o deck.o

test: $(OBJS)
	$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

run: test
	./test

clean:
	rm -f *.o ezdurak test
