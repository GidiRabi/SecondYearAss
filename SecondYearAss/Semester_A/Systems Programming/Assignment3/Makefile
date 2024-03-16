.PHONY: all clean

all: StrList

StrList: Main.o StrLib.a
	gcc -Wall -o StrList Main.o ./StrLib.a

Main.o: Main.c StrList.h
	gcc -Wall -c Main.c -o Main.o

StrLib.a: StrList.o
	ar rc StrLib.a StrList.o
	ranlib StrLib.a

StrList.o: StrList.c StrList.h
	gcc -Wall -c StrList.c -o StrList.o


clean:
	rm -f *.o StrList StrLib.a