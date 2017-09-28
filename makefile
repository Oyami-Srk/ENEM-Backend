CC = g++
CC_FLAG = "-g"


default:
	make clean
	make main.out

Player.o:
	$(CC) $(CC_FLAG) -c Player.cpp Player.h

main.out: Player.o
	$(CC) $(CC_FLAG) -o main.out main.cpp Player.o

run: 
	make default
	./main.out

clean:
	rm *.o
	rm *.gch
	rm *.out
