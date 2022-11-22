a4s1: sequenceTest.o
	g++ sequenceTest.o -o a4s1
sequenceTest.o: sequenceTest.cpp sequence.template sequence.h
	g++ -Wall -ansi -pedantic -std=c++11 -c sequenceTest.cpp
test:
	./a4s1 auto < a4test.in > a4test11.out
clean:
	@rm -rf sequenceTest.o
cleanall:
	@rm -rf sequenceTest.o a4s1

