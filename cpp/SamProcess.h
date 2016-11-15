#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include "SamParser.h"
#include "AlignmentRecord.h"
void ajustRecords(ALIGNRECORDS & records, int readLength, int minLength);
void ajustRecord(AlignmentRecord &record1, AlignmentRecord &record2, int readLength, int minLength);