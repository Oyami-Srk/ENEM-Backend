CC = g++
CC_FLAG = "-g"
CC_LIB="-lcurl"


default:
	make clean
	make main.out

Player.o:
	$(CC) $(CC_FLAG) -c Player.cpp Player.h

Netease.o:
	$(CC) $(CC_FALG) -c Netease.cpp Netease.h $(CC_LIB)

main.out: Netease.o
	$(CC) $(CC_FLAG) -o main.out main.cpp Netease.o $(CC_LIB)

run:
	make default
	./main.out

clean:
	rm *.o
	rm *.gch
	rm *.out
