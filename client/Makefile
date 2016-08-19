EXE:=./bin/main
INC_DIR:=./inc/ 
SRCS:=$(wildcard ./src/*.cpp) 
LIBS:= -lpthread
CXX:=g++
CXXFLAGS:= -Wall -g   $(addprefix -I,$(INC_DIR))


$(EXE) : $(SRCS)
	$(CXX) -o $(EXE)  $(SRCS) $(CXXFLAGS) $(LIBS) 
clean:
	rm $(EXE)
