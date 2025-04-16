CXX=g++
CXXFLAGS=-Wall -g

SRCS=$(wildcard src/*.cpp)
OBJS=$(SRCS:src/%.cpp=objs/%.o)

run: all
	@echo
	./exe

all: $(OBJS) 
	@echo "Compiling executable"
	$(CXX) $(CXXFLAGS) -o exe $^

objs/%.o: src/%.cpp
	@echo "Compiling object $@"
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	@echo ""

clean:
	$(RM) $(RF) objs/*
