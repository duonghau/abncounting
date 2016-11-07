#ifndef ALIGNMENT_H
#define ALIGNMENT_H
#include <string>
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
    std::string optional;
    int rpair;//pair read 1 or 2
public:
    AlignmentRecord(std::string alignment_record,int rpair);
    int start(void);
    int end(void);
    int mapped_length(void);
};
#endif