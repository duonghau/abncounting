#ifndef SAMPARSER_H
#define SAMPARSER_H
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <exception>
#include "AlignmentRecord.h"

class SamParser
{
public:
    SamParser();
    //~SamParser();
    std::map<std::string,AlignmentRecord> parser(std::string read1_mapped, std::string read2_mapped);
};
#endif