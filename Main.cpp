#include "SamProcess.h"
#include "SamParser.h"
#include "AlignmentRecord.h"
#include <iostream>
#include <map>
using namespace std;
int main(int argc, char const *argv[])
{
    /* code */
    SamParser a=SamParser();
    map<string, AlignmentRecord> records=a.parser("data/MH0010.1.sam","data/MH0010.2.sam");
    return 0;
}