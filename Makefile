all:	P1.cpp P1.hpp VectorIntHash.hpp
		g++ -o P1 P1.cpp
debug:	P1.cpp P1.hpp VectorIntHash.hpp
		g++ -g -o P1 P1.cpp
clean:
		rm -rf *.o *~ P1 core