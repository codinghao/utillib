SRCDIRS = .
SRCEXTS = .cpp

CXXFLAGS += -g -Wall -I../include
LDFLAGS += 

SRCNAMES = $(foreach dir, $(SRCDIRS), $(wildcard $(addprefix *,$(SRCEXTS))))
OBJS = $(patsubst %.cpp,%.o,$(SRCNAMES))

.PHONY : all

all : ServerTest ClientTest

ServerTest : ServerTest.o
	g++ -o ServerTest ServerTest.o  $(LDFLAGS)
ClientTest : ClientTest.o
	g++ -o ClientTest ClientTest.o  $(LDFLAGS)
%.o : %.cpp
	g++ -c $(CXXFLAGS) $< -o $@
clean:
	rm -f $(OBJS)
	rm -f ServerTest
	rm -f ClientTest

