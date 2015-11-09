CXX = g++
CXXFLAG = -g -Wall -O2 -lmysqlclient -L/usr/lib/mysql
TARGET = test

all : $(TARGET)

$(TARGET) : test.o
	$(CXX) $(CXXFLAG) -o $(TARGET) test.o

test.o : test.cpp DBPool.h DBAgent.h 
	$(CXX) $(CXXFLAG) -c test.cpp

clean:
	rm -rf *.o test
