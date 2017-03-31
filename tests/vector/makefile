CXX=g++
CCFLAGS= -std=c++14 -I${HOME}/freeglut-3.0.0/include -pthread -O2 -Wall -pedantic -c 
LDFLAGS= -lpthread -lGL -lGLU -lm
#LDFLAGS += -lglut
LDFLAGS += -L${HOME}/freeglut-3.0.0/lib -lX11 -lXxf86vm -lXrandr -lXi ${HOME}/freeglut-3.0.0/lib/libglut.a ${HOME}/freeglut-3.0.0/lib/libglut.so



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

