# Makefile for CS 360 example code

CXX=	g++ -g
COBJS=	client.o
SOBJS= 	select.o
POBJS=	poll.o
FOBJS=	fork.o
TOBJS=	thread.o
OBJS =	$(SOBJS) $(POBJS) $(FOBJS) $(COBJS) $(TOBJS)

LIBS=
TLIBS=	-pthread

CCFLAGS= -g

all:	select poll fork thread client

select:	$(SOBJS)
	$(CXX) -o select $(SOBJS) $(LIBS)

poll:	$(POBJS)
	$(CXX) -o poll $(POBJS) $(LIBS)

fork:	$(FOBJS)
	$(CXX) -o fork $(FOBJS) $(LIBS)

thread:	$(TOBJS)
	$(CXX) -o thread $(TOBJS) $(TLIBS)

client:	$(COBJS)
	$(CXX) -o client $(COBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.dd)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.dd) select poll fork thread client


# These lines ensure that dependencies are handled automatically.
%.dd:	%.cc
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.dd)
