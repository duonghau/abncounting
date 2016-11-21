#ifndef SAMPARSER_H
#define SAMPARSER_H
#include <string>
#include <map>
#include <fstream>
#include "AlignmentRecord.h"

typedef std::map<std::string,std::map<std::string,std::vector<AlignmentRecord>>> ALIGNRECORDS;
typedef std::map<std::string,std::map<std::string,std::vector<AlignmentRecord>>>::iterator ALIGNRECORD_ITER;
typedef std::map<std::string,std::vector<AlignmentRecord>> SCAFFOLDS;
typedef std::map<std::string,std::vector<AlignmentRecord>>::iterator SCAFFOLD_ITER;
typedef std::vector<AlignmentRecord> READS;
typedef std::vector<AlignmentRecord>::iterator READ_ITER;
typedef std::map<std::string, int> HEADER;
typedef std::map<std::string, int>::iterator HEAD_ITER;

class SamParser{
private:
    std::string samPath;
    std::vector<std::string> stringSplit(const std::string & record, char split);
    std::map<int,int> countStrand(SCAFFOLDS & scaffolds);
public:
    SamParser();
    ~SamParser();
    SamParser(std::string path);
    void parser(HEADER & refInfor,std::vector<std::string> & qNames, ALIGNRECORDS &records,
        int minLength=35, float identity=0.9);
    bool writeLengthFile(const std::string & path);
    bool writeFinalFile(ALIGNRECORDS &records,const std::string & path);
};
#endif