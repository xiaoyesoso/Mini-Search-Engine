#test: ../../../Src/ICTCLAS2013/example-c/Example-C.cpp ../../../Src/ICTCLAS2013/include/NLPIR.h
#	 g++ ../../../Src/ICTCLAS2013/example-c/Example-C.cpp -L. -lpthread -L../../../bin/ICTCLAS2013 -lNLPIR -Wall -Wunused -O3 -DOS_LINUX  -o ../../../bin/ICTCLAS2013/example

INC_DIR:= ../include/
LIBS:= -lNLPIR 
CXXFLAGS:= -Wall -g -Wunused -DOS_LINUX $(addprefix -I, $(INC_DIR)) $(addprefix -L, $(LINK_DIR)) $(LIBS)
main:main.cpp
	g++ -o $@ $^ $(CXXFLAGS) 
clean:
	rm $@
