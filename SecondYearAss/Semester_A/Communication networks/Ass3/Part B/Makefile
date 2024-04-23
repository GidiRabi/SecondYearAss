.PHONY: all clean


all: RUDP_Receiver RUDP_Sender

RUDP_Receiver: RUDP_Receiver.o RUDP_API.o
	gcc -Wall -o RUDP_Receiver RUDP_Receiver.o RUDP_API.o

RUDP_Sender: RUDP_Sender.o RUDP_API.o
	gcc -Wall -o RUDP_Sender RUDP_Sender.o RUDP_API.o

%.o: %.c RUDP_API.h
	gcc -Wall -c $< -o $@

clean: 
	rm -f RUDP_Receiver RUDP_Sender *.o