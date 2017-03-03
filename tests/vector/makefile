CXX=g++
CFLAGS= -std=c++14 -pthread -O2 -Wall -pedantic -c 
LDFLAGS= -lpthread -lglut -lGL -lGLU -lm

QMTHEADERS = ../../headers
QMTSRC = ../../src
BIN = ./obj


MAIN_PREFIX = main
MODE = vector

MKDIR_P = mkdir -p

$(MODE): obj $(BIN)/$(MAIN_PREFIX).o
	$(CXX) $(BIN)/$(MAIN_PREFIX).o $(LDFLAGS) -o $(MODE)
$(BIN)/$(MAIN_PREFIX).o: $(MAIN_PREFIX).cpp
	$(CXX) $(CFLAGS) $(MAIN_PREFIX).cpp -o $(BIN)/$(MAIN_PREFIX).o 

obj:
	${MKDIR_P} obj


clean:
	rm -rf $(MODE) $(BIN)

