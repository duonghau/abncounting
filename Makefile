all : Main.exe
Main.exe : Main.o AlignmentRecord.o SamParser.o
	g++ -std=c++11 -o Main.exe Main.o AlignmentRecord.o SamParser.o
Main.o: Main.cpp
	g++ -std=c++11 -c Main.cpp
AlignmentRecord.o: AlignmentRecord.cpp
	g++ -std=c++11 -c AlignmentRecord.cpp
SamParser.o:SamParser.cpp
	g++ -std=c++11 -c SamParser.cpp
Clean:
	rm Main.o AlignmentRecord.o SamParser.o