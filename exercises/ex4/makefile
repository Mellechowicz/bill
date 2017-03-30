CXX=g++
CFLAGS= -std=c++14 -pthread -O2 -Wall -pedantic -c 
LDFLAGS= -lpthread -lglut -lGL -lGLU -lm

QMTHEADERS = ../../headers
QMTSRC = ../../src
BIN = ./obj


MAIN_PREFIX = main
MODE = grass

MKDIR_P = mkdir -p

$(MODE): obj $(BIN)/$(MAIN_PREFIX).o $(BIN)/billwindow.o  $(BIN)/billGLfunctions.o $(BIN)/billengine.o $(BIN)/billmaterialpoint.o $(BIN)/oscillator.o
	$(CXX) $(BIN)/oscillator.o $(BIN)/billmaterialpoint.o  $(BIN)/billengine.o $(BIN)/billGLfunctions.o $(BIN)/billwindow.o $(BIN)/$(MAIN_PREFIX).o $(LDFLAGS) -o $(MODE)
$(BIN)/$(MAIN_PREFIX).o: $(MAIN_PREFIX).cpp
	$(CXX) $(CFLAGS) $(MAIN_PREFIX).cpp -o $(BIN)/$(MAIN_PREFIX).o 
$(BIN)/billwindow.o: $(QMTSRC)/billwindow.cpp 
	    $(CXX) $(CFLAGS) $(QMTSRC)/billwindow.cpp -o $(BIN)/billwindow.o
$(BIN)/billGLfunctions.o: $(QMTSRC)/billGLfunctions.cpp 
	    $(CXX) $(CFLAGS) $(QMTSRC)/billGLfunctions.cpp -o $(BIN)/billGLfunctions.o
$(BIN)/billengine.o: $(QMTSRC)/billengine.cpp 
	    $(CXX) $(CFLAGS) $(QMTSRC)/billengine.cpp -o $(BIN)/billengine.o
$(BIN)/billmaterialpoint.o: $(QMTSRC)/billmaterialpoint.cpp 
	    $(CXX) $(CFLAGS) $(QMTSRC)/billmaterialpoint.cpp -o $(BIN)/billmaterialpoint.o
$(BIN)/oscillator.o: oscillator.cpp
	    $(CXX) $(CFLAGS) oscillator.cpp -o $(BIN)/oscillator.o
obj:
	${MKDIR_P} obj


clean:
	rm -rf $(MODE) $(BIN)

