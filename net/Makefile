include ../Makefile.inc

CXXFLAGS = $(PRJCXXFLAGS) -I..
OBJLIBS  = ../libezduraknet.a
OBJS = NetworkListener.o kissnet.o NetworkProtocol.o

.PHONY: all
all: $(OBJLIBS)

../libezduraknet.a: $(OBJS)
	echo $(ARFLAGS)
	$(AR) $(ARFLAGS) ../libezdurakcli.a $?
	$(RANLIB) ../libezdurakcli.a

clean:
	$(ECHO) clean up net
	-$(RM) -f $(OBJS)

.PHONY: force_look
force_look: true
