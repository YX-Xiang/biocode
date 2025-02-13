SRCS := ${wildcard *.cpp}
OBJS := $(SRCS:.cpp=.o)

PROG := seq2graph.exe
CXX := g++
CFLAGS := -Wall -Ofast -std=c++1z -fopenmp -fPIE 
LDFLAGS := -pie

$(PROG): $(OBJS)
	$(CXX) ${CFLAGS} -o $@ $^

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)
	rm -f $(PROG)