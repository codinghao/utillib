PROGRAM = RoseDB
SRCDIRS = .
SRCEXTS = .cpp

CXXFLAGS += -g -Wall -I../include
LDFLAGS += 

SRCNAMES = $(foreach dir, $(SRCDIRS), $(wildcard $(addprefix *,$(SRCEXTS))))
OBJS = $(patsubst %.cpp,%.o,$(SRCNAMES))

.PHONY : all

all : $(PROGRAM)

$(PROGRAM) : $(OBJS)
	g++ -o $(PROGRAM) $(OBJS) $(LDFLAGS)
%.o : %.cpp
	g++ -c $(CXXFLAGS) $< -o $@
clean:
	rm -f $(OBJS)
	rm -f $(PROGRAM)

