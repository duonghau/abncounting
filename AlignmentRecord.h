#ifndef ALIGNMENT_H
#define ALIGNMENT_H
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <regex>
class AlignmentRecord{
private:
    std::string qname;
    std::string rname;
    int flag;
    int pos;
    int mapq;    
    std::string cigar;
    std::string rnext;
    int pnext;
    int tlen;
    std::string seq;
    std::string qual;
    std::string MN,XM;
    int rpair;//pair read 1 or 2
    std::vector<std::string> record_parser(std::string record);
    int str2int(std::string str);
public:
    AlignmentRecord();
    AlignmentRecord(std::string alignment_record,int rpair);
    int start(void);
    int end(void);
    int mapped_length(void);
    std::string to_string(void);
    bool is_valid(void);
};
#endif