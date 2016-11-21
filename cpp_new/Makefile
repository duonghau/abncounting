all : AbnCounting
AbnCounting: AbnCounting.o SamProcess.o AlignmentRecord.o
	g++ -std=c++11 -L/home/duongtienhau/bin/boost/lib -o AbnCounting AbnCounting.o SamProcess.o AlignmentRecord.o -lboost_regex -pthread
AbnCounting.o:AbnCounting.cpp
	g++ -std=c++11   -I/home/duongtienhau/bin/boost/include -c AbnCounting.cpp
SamProcess.o: SamProcess.cpp
	g++ -std=c++11  -I/home/duongtienhau/bin/boost/include -c SamProcess.cpp -pthread
AlignmentRecord.o: AlignmentRecord.cpp
	g++ -std=c++11  -I/home/duongtienhau/bin/boost/include  -c AlignmentRecord.cpp -lboost_regex

Clean:
	rm SamProcess.o AlignmentRecord.o AbnCounting.o
