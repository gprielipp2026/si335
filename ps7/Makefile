CXX=g++
CXXFLAGS=-Wall -g -std=c++20

SRCS=$(wildcard src/*.cpp)
OBJS=$(SRCS:src/%.cpp=objs/%.o)

run: exe
	@echo
	./exe junk.txt

all: run 
	
exe: $(OBJS)
	@echo "Compiling executable"
	$(CXX) $(CXXFLAGS) -o exe $^

objs/%.o: src/%.cpp
	@echo "Compiling object $@"
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	@echo ""

clean:
	$(RM) $(RF) objs/*
