CC := gcc
LIB:= -lstdc++

vm: main.cpp
	${CC} $^ -o $@ ${LIB}
