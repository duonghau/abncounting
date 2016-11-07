all : test.exe

test.exe : test_map.o test2.o
	g++ -o test.exe test_map.o test2.o
test_map.o: test_map.cpp
	g++ -c test_map.cpp
test2.o: test2.cpp
	g++ -c test2.cpp