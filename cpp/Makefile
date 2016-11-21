all : SamProcess
SamProcess: SamProcess.o AlignmentRecord.o SamParser.o
	g++ -std=c++11 -L/home/duongtienhau/bin/boost/lib -o SamProcess SamProcess.o AlignmentRecord.o SamParser.o -lboost_regex
SamProcess.o: SamProcess.cpp
	g++ -I/home/duongtienhau/bin/boost/include -std=c++11 -c SamProcess.cpp -pthread
AlignmentRecord.o: AlignmentRecord.cpp
	g++ -I/home/duongtienhau/bin/boost/include -std=c++11 -c AlignmentRecord.cpp
SamParser.o:SamParser.cpp
	g++ -I/home/duongtienhau/bin/boost/include -std=c++11 -c SamParser.cpp
Clean:
	rm SamProcess.o AlignmentRecord.o SamParser.o
