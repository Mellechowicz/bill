CXX=g++
CCFLAGS= -std=c++14 -I${HOME}/freeglut-3.0.0/include -pthread -O2 -Wall -pedantic -c 
LDFLAGS= -lpthread -lGL -lGLU -lm
#LDFLAGS += -lglut
LDFLAGS += -L${HOME}/freeglut-3.0.0/lib -lX11 -lXxf86vm -lXrandr -lXi ${HOME}/freeglut-3.0.0/lib/libglut.a ${HOME}/freeglut-3.0.0/lib/libglut.so



QMTHEADERS = ../../headers
QMTSRC = ../../src
BIN = ./obj

MAIN_PREFIX = main
MODE = smallredball

MKDIR_P = mkdir -p

$(MODE): obj $(BIN)/$(MAIN_PREFIX).o $(BIN)/billwindow.o $(BIN)/billGLfunctions.o
	$(CXX) $(BIN)/billGLfunctions.o $(BIN)/billwindow.o $(BIN)/$(MAIN_PREFIX).o $(LDFLAGS) -o $(MODE)
$(BIN)/$(MAIN_PREFIX).o: $(MAIN_PREFIX).cpp
	$(CXX) $(CFLAGS) $(MAIN_PREFIX).cpp -o $(BIN)/$(MAIN_PREFIX).o 
$(BIN)/billwindow.o: $(QMTSRC)/billwindow.cpp 
	    $(CXX) $(CFLAGS) $(QMTSRC)/billwindow.cpp -o $(BIN)/billwindow.o
$(BIN)/billGLfunctions.o: $(QMTSRC)/billGLfunctions.cpp 
	    $(CXX) $(CFLAGS) $(QMTSRC)/billGLfunctions.cpp -o $(BIN)/billGLfunctions.o
obj:
	${MKDIR_P} obj


clean:
	rm -rf $(MODE) $(BIN)

