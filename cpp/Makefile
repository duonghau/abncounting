all : SamProcess
SamProcess: SamProcess.o AlignmentRecord.o SamParser.o
	g++ -std=c++11 -o SamProcess SamProcess.o AlignmentRecord.o SamParser.o -lboost_regex
SamProcess.o: SamProcess.cpp
	g++ -std=c++11 -c SamProcess.cpp
AlignmentRecord.o: AlignmentRecord.cpp
	g++ -std=c++11 -c AlignmentRecord.cpp
SamParser.o:SamParser.cpp
	g++ -std=c++11 -c SamParser.cpp
Clean:
	rm SamProcess.o AlignmentRecord.o SamParser.o
