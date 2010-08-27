include ../Makefile.inc

CXXFLAGS = $(PRJCXXFLAGS) -I..
OBJLIBS  = ../libezduraknet.a
OBJS = NetworkListener.o kissnet.o NetworkProtocol.o NetworkGame.o

.PHONY: all
all: $(OBJLIBS)

clientmain: $(OBJLIBS) clientmain.o
	g++ -L../ -lezduraknet -lezdurakcore -lezdurakcli

../libezduraknet.a: $(OBJS)
	echo $(ARFLAGS)
	$(AR) $(ARFLAGS) ../libezduraknet.a $?
	$(RANLIB) ../libezduraknet.a

clean:
	$(ECHO) clean up net
	-$(RM) -f $(OBJS) *.o

.PHONY: force_look
force_look: true
