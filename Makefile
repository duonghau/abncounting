all : Main.exe
Main.exe : Main.o AlignmentRecord.o SamParser.o
	g++ -o Main.exe Main.o AlignmentRecord.o SamParser.o
Main.o: Main.cpp
	g++ -c Main.cpp
AlignmentRecord.o: AlignmentRecord.cpp
	g++ -c AlignmentRecord.cpp
SamParser.o:SamParser.cpp
	g++ -c SamParser.cpp
Clean:
	rm Main.o AlignmentRecord.o SamParser.o