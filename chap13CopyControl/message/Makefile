CXX = g++
FLAGS = -std=c++11
SRC = $(wildcard *.cpp)
INC = "./"

msg.x: ${SRC}
		${CXX} ${FLAGS} -I${INC} -o $@ $^

clean:
		rm -rf *.x
