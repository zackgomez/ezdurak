include ../Makefile.inc

CXXFLAGS = $(PRJCXXFLAGS) -I..
OBJLIBS  = ../libezduraknet.a
OBJS = NetworkListener.o kissnet.o NetworkProtocol.o NetworkGame.o NetworkPlayer.o \
       NetworkHost.o

.PHONY: all
all: $(OBJLIBS)

../libezduraknet.a: $(OBJS)
	echo $(ARFLAGS)
	$(AR) $(ARFLAGS) ../libezduraknet.a $?
	$(RANLIB) ../libezduraknet.a

clean:
	$(ECHO) clean up net
	-$(RM) -f $(OBJS) *.o

.PHONY: force_look
force_look: true
