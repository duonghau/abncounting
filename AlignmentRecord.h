#ifndef ALIGNMENT_H
#define ALIGNMENT_H
#include <string>
class AlignmentRecord{
private:
    string qname;
    string rname;
    int flag;
    int pos;
    int mapq;    
    string cigar;
    string rnext;
    int pnext;
    int tlen;
    string seq;
    string qual;
    string optional;
public:
    AlignmentRecord(string line,int rpair);
    int start(void);
    int end(void);
};
#endif