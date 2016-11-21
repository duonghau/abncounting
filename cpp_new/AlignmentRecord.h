#ifndef ALIGNMENTRECORD_H
#define ALIGNMENTRECORD_H
#include <string>
#include <vector>
#include <sstream>
#include <ctype.h>
#include <sstream>
#include <boost/regex.hpp>

/*----------------------------
        Bitwise FLAG
------------------------------*/
/*! @abstract the read is paired in sequencing, no matter whether it is mapped in a pair */
#define SAM_FPAIRED        1
/*! @abstract the read itself is unmapped; conflictive with SAM_FPROPER_PAIR */
#define SAM_FUNMAP         4
/*! @abstract this is read1 */
#define SAM_FREAD1        64
/*! @abstract this is read2 */
#define SAM_FREAD2       128
/*! @abstract not primary alignment */
/*! @abstract not primary alignment */
#define SAM_FSECONDARY   256
std::vector<std::string> stringSplit(const std::string & record, char split='\t');

class AlignmentRecord{
private:
    std::string qname;    
    std::string rname;
    int flag;
    int start;
    int end;
    int xm;
    bool keep=true;    
    int getMappedLength(const std::string & cigar);
    bool isRead1(void) const;
    bool isRead2(void) const;

public:
    AlignmentRecord();
    ~AlignmentRecord();
    AlignmentRecord(const std::string & record_raw);
    std::string getQname(void) const;
    std::string getRname(void) const;
    bool isMapped(void) const;
    int getStart(void) const;
    int getEnd(void) const;
    bool isValid(const int & minlength) const;
    float getIdentity(void) const;
    int getRead(void) const;
    void setStart(const int & start);
    void setEnd(const int & end);
    bool isKeep(void) const;
    void unKeep(void);
    bool isPaired(void) const;
    bool isSecondary(void) const;
    std::string toString(void) const;
};
#endif