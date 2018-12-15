# Project: serv

CPP  = g++
CC   = gcc
WINDRES = windres.exe
RES  = 
OBJ  = main.o ../lib/platform.o ../lib/packet.o ../lib/socket.o ../lib/session.o userlist.o proc.o ../lib/list.o ../lib/encrypt.o  $(RES)
LINKOBJ  = main.o ../lib/platform.o ../lib/packet.o ../lib/socket.o ../lib/session.o userlist.o proc.o ../lib/list.o ../lib/encrypt.o $(RES)
LIBS =  
INCS =  
CXXINCS = 
BIN  = ../bin/serv.exe
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS)  
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before ../bin/serv.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o "../bin/serv.exe" -pthread $(LIBS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

../lib/platform.o: ../lib/platform.c
	$(CC) -c ../lib/platform.c -o ../lib/platform.o $(CFLAGS)

../lib/packet.o: ../lib/packet.c
	$(CC) -c ../lib/packet.c -o ../lib/packet.o $(CFLAGS)

../lib/socket.o: ../lib/socket.c
	$(CC) -c ../lib/socket.c -o ../lib/socket.o $(CFLAGS)

../lib/session.o: ../lib/session.c
	$(CC) -c ../lib/session.c -o ../lib/session.o $(CFLAGS)

userlist.o: userlist.c
	$(CC) -c userlist.c -o userlist.o $(CFLAGS)

proc.o: proc.c
	$(CC) -c proc.c -o proc.o $(CFLAGS)

../lib/list.o: ../lib/list.c
	$(CC) -c ../lib/list.c -o ../lib/list.o $(CFLAGS)

../lib/encrypt.o: ../lib/encrypt.c
	$(CC) -c ../lib/encrypt.c -o ../lib/encrypt.o $(CFLAGS)
