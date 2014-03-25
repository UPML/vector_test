main: clean vector.o source
	g++ ./vector.o -o vector -pg -lgtest
clean:
	rm -f ./vector
	rm -rf ./*.o
	rm -f ./gmon.out
vector.o:
	g++ -ggdb -c ./vector.cpp -pg -std=c++11
edit:
	vim ./vector.cpp
run:
	./vector
