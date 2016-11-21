#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <thread>
#include <ctime>
#include <math.h>
#include "SamParser.h"
#include "AlignmentRecord.h"
void ajustRecords(ALIGNRECORDS &records,const std::vector<std::string> & qNames,
	int begin, int end, int readLength, int minLength);
void ajustRecord(AlignmentRecord &record1, AlignmentRecord &record2, int readLength, int minLength);
void preProcess(ALIGNRECORDS &records,int threads);