#include "AlignmentRecord.h"
using namespace std;
// isdigit()
AlignmentRecord::AlignmentRecord(){};
AlignmentRecord::~AlignmentRecord(){};
AlignmentRecord::AlignmentRecord(const string & record_raw){
    std::vector<string> tokens=stringSplit(record_raw);
    qname=tokens[0];
    flag=atoi(tokens[1].c_str());
    rname=tokens[2];
    start=atoi(tokens[3].c_str())-1;
    end=start+getMappedLength(tokens[5]);
    xm=atoi(stringSplit(tokens[13],':')[2].c_str());
}

string AlignmentRecord::getQname(void) const{
    return qname;
}

string AlignmentRecord::getRname(void) const{
    return rname;
}

bool AlignmentRecord::isPaired(void) const{
    return (flag & SAM_FPAIRED);
}

bool AlignmentRecord::isMapped(void) const{
    return !(flag & SAM_FUNMAP);
}

bool AlignmentRecord::isRead1(void) const{
    return (flag & SAM_FREAD1);
}

bool AlignmentRecord::isRead2(void) const{
    return (flag & SAM_FREAD2);
}

int AlignmentRecord::getStart(void) const{
    return start;
}

int AlignmentRecord::getEnd(void) const{
    return end;
}

int AlignmentRecord::getMappedLength(const string & cigar){
    // mapped length is count from CIGAR chars [MX=]
    // all other CIGAR chars do not affect end position
    string regex_string = "([[:digit:]]+)[MX]";
    if(cigar==""){
        return 0;
    }else{
        // user regular expression for calculate the mapped length
        int length=0;
        boost::regex expr{regex_string};
        string::const_iterator begin = cigar.begin();
        string::const_iterator end = cigar.end();
        boost::match_results<string::const_iterator> matchs;
        while (regex_search(begin, end, matchs, expr)) {
            string match= string(matchs[1].first, matchs[2].second-1);
            length+=atoi(match.c_str());
            begin = matchs[0].second;
        }
        return length;
    }
}

bool AlignmentRecord::isValid(const int & minLength) const{
    return ((end-start)>minLength);
}

// identity= match/(maped length)= (M-XM)/M
float AlignmentRecord::getIdentity(void) const{
    int mappedLength=(end-start);
    return ((float)(mappedLength-xm)/mappedLength);
}

std::vector<string> stringSplit(const string & record, char split){
    vector<string> tokens;
    istringstream iss(record);
    string token;
    while(getline(iss, token, split)){
        tokens.push_back(token);
    }
    return tokens;
}

int AlignmentRecord::getRead(void) const{
    if(isRead2()){
        return 2;
    }else{
        return 1;
    }
}

void AlignmentRecord::setStart(const int & start){
    this->start=start;
}

void AlignmentRecord::setEnd(const int & end){
    this->end=end;
}

bool AlignmentRecord::isKeep(void) const{
    return keep;
}

void AlignmentRecord::unKeep(void){
    keep=false;
}

bool AlignmentRecord::isSecondary(void) const{
    return (flag & SAM_FSECONDARY);
}

string AlignmentRecord::toString(void) const{
    stringstream ss;
    ss<<qname<<"_"<<getRead()<<" "<<rname<<" "<<start<<" "<<end;
    return ss.str();
}