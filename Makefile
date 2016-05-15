database : main.o basic.o algorithm.o
	g++ -std=c++11 -O2 -o database main.o basic.o algorithm.o

main.o : main.cpp datastructure.h basic.h
	g++ -std=c++11 -c main.cpp

basic.o : basic.cpp datastructure.h basic.h
	g++ -std=c++11 -c basic.cpp

algorithm.o : algorithm.cpp datastructure.h basic.h
	g++ -std=c++11 -c algorithm.cpp

.PHONY : clean
clean :
	rm -f database *.o