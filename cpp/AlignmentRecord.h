#ifndef ALIGNMENTRECORD_H
#define ALIGNMENTRECORD_H
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <ctype.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
/*----------------------------
        AligmentRecord
------------------------------*/
/*
Class: AligmentRecord
Store informations principal of a alignment.
- qname: Query template NAME
- flag: bitwise FLAG
- rname: Reference sequence NAME
- pos: 1-based leftmost mapping POSition
- mapq: MAPping Quality
- cigar: CIGAR string
- rnext: Ref. name of the mate/next read
- pnext: Position of the mate/next read
- tlen: observed Template LENgth
- seq: segment SEQuence
- qual: ASCII of Phred-scaled base QUALity+33
*/

/*----------------------------
        Bitwise FLAG
------------------------------*/
/*! @abstract the read is paired in sequencing, no matter whether it is mapped in a pair */
#define SAM_FPAIRED        1
/*! @abstract the read is mapped in a proper pair */
#define SAM_FPROPER_PAIR   2
/*! @abstract the read itself is unmapped; conflictive with SAM_FPROPER_PAIR */
#define SAM_FUNMAP         4
/*! @abstract the mate is unmapped */
#define SAM_FMUNMAP        8
/*! @abstract the read is mapped to the reverse strand */
#define SAM_FREVERSE      16
/*! @abstract the mate is mapped to the reverse strand */
#define SAM_FMREVERSE     32
/*! @abstract this is read1 */
#define SAM_FREAD1        64
/*! @abstract this is read2 */
#define SAM_FREAD2       128
/*! @abstract not primary alignment */
#define SAM_FSECONDARY   256
/*! @abstract QC failure */
#define SAM_FQCFAIL      512
/*! @abstract optical or PCR duplicate */
#define SAM_FDUP        1024
/*! @abstract supplementary alignment */
#define SAM_FSUPPLEMENTARY 2048

class AlignmentRecord{
private:
    std::string qname;
    int flag;
    std::string rname;
    int pos;
    int mapq;
    std::string cigar;
    std::string rnext;
    int pnext;
    int tlen;
    std::string seq;
    std::string qual;
    std::map<std::string,std::string> tags;
    std::vector<std::string> stringSplit(const std::string & record, char split='\t') const;
    int getMLength(void);
    int end;
    bool keep=true;
    
public:
    AlignmentRecord();
    ~AlignmentRecord();
    AlignmentRecord(const std::string & record_raw, bool lite=true);
    std::string getQname(void) const;
    int getFlag(void) const;
    std::string getRname(void) const;
    int getPos(void) const;
    int getMapq(void) const;
    std::string getCigar(void) const;
    std::string getRnext(void) const;
    int getPnext(void) const;
    int getTlen(void) const;
    std::string getSeq(void) const;
    std::string getQual(void) const;
    // only for 
    int getNumericTag(const std::string & tag) const;
    std::string getStringTag(const std::string & tag) const;
    bool isPaired(void) const;
    bool isMapped(void) const;
    bool isSecondary(void) const;
    bool isReverse(void) const;
    bool isDuplicate(void) const;
    bool isRead1(void) const;
    bool isRead2(void) const;
    int getStart(void) const;
    int getEnd(void) const;
    int getMappedLength(void) const;
    bool isValid(const int & minlength) const;
    float getIdentity(void) const;
    int getStrand(void) const;
    void setStart(const int & start);
    void setEnd(const int & end);
    bool isKeep(void) const;
    void unKeep(void);
    std::string toString(void) const;
};
#endif