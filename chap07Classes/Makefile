CXX = g++
FLAGS = -std=c++11
SRC = $(wildcard *.cpp)
TARGETS = $(patsubst %.cpp, %.x, $(SRC))

all: ${TARGETS}

%.x : %.cpp
	${CXX} ${FLAGS} -o $@ $^

clean:
	rm -rf *.x
