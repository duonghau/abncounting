#include <iostream>
#include <stdlib.h>
#include "SamParser.h"
#include "SamProcess.h"

ALIGNRECORDS records;
HEADER refInfor;
std::vector<std::string> qNames;

int main(int argc, char const *argv[])
{	
	/* code */
	string filename=argv[1];
	string filelen=argv[2];
	string filefinal=argv[3];
	int numthread=atoi(argv[4]);
	SamParser sam(filename);
	sam.parser(refInfor,qNames,records,35,0.9);
	
	return 0;
}