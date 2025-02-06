CXX = g++
CXXFLAGS = -Wall -g -std=c++20

SRCS = $(wildcard *.cpp)
TARGETS = $(SRCS:%.cpp=%)

FILE =


all: $(TARGETS)


%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@


test: rooms checker
	./rooms <$(FILE) | ./checker $(FILE)

