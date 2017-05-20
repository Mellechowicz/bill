CXX=g++
CFLAGS= -std=c++14 -I${HOME}/freeglut-3.0.0/include -pthread -O2 -Wall -pedantic -c 
LDFLAGS= -lpthread -lGL -lGLU -lm
#LDFLAGS += -lglut
LDFLAGS += -L${HOME}/freeglut-3.0.0/lib -lX11 -lXxf86vm -lXrandr -lXi ${HOME}/freeglut-3.0.0/lib/libglut.a ${HOME}/freeglut-3.0.0/lib/libglut.so

QMTHEADERS = ../../headers
QMTSRC = ../../src
BIN = ./obj


MAIN_PREFIX = main
MODE = boxes

MKDIR_P = mkdir -p

$(MODE): obj $(BIN)/$(MAIN_PREFIX).o $(BIN)/billwindow.o  $(BIN)/billGLfunctions.o $(BIN)/billRBengine.o $(BIN)/billmaterialpoint.o $(BIN)/billrigidbody.o $(BIN)/box.o
	$(CXX) $(BIN)/box.o $(BIN)/billrigidbody.o $(BIN)/billmaterialpoint.o $(BIN)/billRBengine.o $(BIN)/billGLfunctions.o $(BIN)/billwindow.o $(BIN)/$(MAIN_PREFIX).o $(LDFLAGS) -o $(MODE)
$(BIN)/$(MAIN_PREFIX).o: $(MAIN_PREFIX).cpp
	$(CXX) $(CFLAGS) $(MAIN_PREFIX).cpp -o $(BIN)/$(MAIN_PREFIX).o 
$(BIN)/billwindow.o: $(QMTSRC)/billwindow.cpp 
	    $(CXX) $(CFLAGS) $(QMTSRC)/billwindow.cpp -o $(BIN)/billwindow.o
$(BIN)/billGLfunctions.o: $(QMTSRC)/billGLfunctions.cpp 
	    $(CXX) $(CFLAGS) $(QMTSRC)/billGLfunctions.cpp -o $(BIN)/billGLfunctions.o
$(BIN)/billRBengine.o: $(QMTSRC)/billRBengine.cpp 
	    $(CXX) $(CFLAGS) $(QMTSRC)/billRBengine.cpp -o $(BIN)/billRBengine.o
$(BIN)/billmaterialpoint.o: $(QMTSRC)/billmaterialpoint.cpp 
	    $(CXX) $(CFLAGS) $(QMTSRC)/billmaterialpoint.cpp -o $(BIN)/billmaterialpoint.o
$(BIN)/billrigidbody.o: $(QMTSRC)/billrigidbody.cpp 
	    $(CXX) $(CFLAGS) $(QMTSRC)/billrigidbody.cpp -o $(BIN)/billrigidbody.o
$(BIN)/box.o: box.cpp 
	    $(CXX) $(CFLAGS) box.cpp -o $(BIN)/box.o
obj:
	${MKDIR_P} obj


clean:
	rm -rf $(MODE) $(BIN)

