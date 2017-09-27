CC = g++
CC_FLAG = "-g"


default:
	make main.out

Player.o:
	$(CC) $(CC_FLAG) -c Player.cpp Player.h

main.out: Player.o
	$(CC) $(CC_FLAG) -o main.out main.cpp Player.o

run: main.out
	./main.out

clean:
	rm *.o
	rm *.gch
	rm *.out
