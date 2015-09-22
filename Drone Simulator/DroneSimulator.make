#Taken and modified from http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

CC=gcc

all: 
	$(CC) -o dronesimulator -lm -pthread dronesim.c
