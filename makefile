CC = g++
LDFLAGSRUN = -pthread
LDFLAGSTEST = -pthread -lgtest

all: compile test-compile

compile: main.cpp thread_pool.cpp staticstics.cpp display.cpp timer.cpp
	${CC} ${LDFLAGSRUN} $^ -o main

test-compile: test.cpp thread_pool.cpp staticstics.cpp display.cpp
	${CC} ${LDFLAGSTEST} $^ -o test