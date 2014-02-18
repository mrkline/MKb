CXXFLAGS= -Wall -std=c++11 -Wextra -g -I../ -I../../
OBJ_FILES= IP.o TCPConnection.o TCPListener.o UDPClient.o

all: unit_tests

unit_tests: main.o $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o unit_tests main.o $(OBJ_FILES)

%.o: ../%.cpp
	g++ $(CXXFLAGS) -c $<

clean:
	rm -f *.o *.out unit_tests

.PHONY: clean
