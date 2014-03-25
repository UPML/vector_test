main: clean main.o source
	g++ ./main.o -o main -pg -lgtest
clean:
	rm -f ./main
	rm -rf ./*.o
	rm -f ./gmon.out
main.o:
	g++ -ggdb -c ./main.cpp -pg -std=c++11
edit:
	vim ./main.cpp
run:
	./main
