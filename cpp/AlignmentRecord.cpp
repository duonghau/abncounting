#include "AlignmentRecord.h"
using namespace std;
// isdigit()
AlignmentRecord::AlignmentRecord(){};
AlignmentRecord::~AlignmentRecord(){};
AlignmentRecord::AlignmentRecord(const string & record_raw, bool lite){
    std::vector<string> tokens=stringSplit(record_raw);
    qname=tokens[0];
    flag=atoi(tokens[1].c_str());
    rname=tokens[2];
    pos=atoi(tokens[3].c_str());
    cigar=tokens[5];
    end=pos+getMLength();//calculate mapped length
    if(lite==false){
        mapq=atoi(tokens[4].c_str());
        rnext=tokens[6];
        pnext=atoi(tokens[7].c_str());
        tlen=atoi(tokens[8].c_str());
        seq=tokens[9];
        qual=tokens[10];
    }
    for(int i=11; i<tokens.size();i++){
        string tmp=tokens[i];
        if(tmp!=""){
            tags[tmp.substr(0,2)]=tmp;
        }
    }
}

string AlignmentRecord::getQname(void) const{
    return qname;
}

int AlignmentRecord::getFlag(void) const{
    return flag;
}

string AlignmentRecord::getRname(void) const{
    return rname;
}

int AlignmentRecord::getPos(void) const{
    return pos;
}

int AlignmentRecord::getMapq(void) const{
    return mapq;
}

string AlignmentRecord::getCigar(void) const{
    return cigar;
}

string AlignmentRecord::getRnext(void) const{
    return rnext;
}

int AlignmentRecord::getPnext(void) const{
    return pnext;
}

int AlignmentRecord::getTlen(void) const{
    return tlen;
}

string AlignmentRecord::getSeq(void) const{
    return seq;
}

string AlignmentRecord::getQual(void) const{
    return qual;
}

// AS,XN, XM, XO, XG, NM, YS
int AlignmentRecord::getNumericTag(const string & tag) const{
    if (!(tag=="AS" || tag=="XN"||tag=="XM"||tag=="XO" ||tag=="XG"||tag=="NM"||tag=="YS")){
        return -1; //invalid tag
    }
    if ( tags.find(tag) == tags.end() ) {
        return -1;//not found
    }else {
        map<string,string>mytags=tags;
        vector<string> tokens=stringSplit(mytags[tag],':');
        if(tokens.size()>0){
            return atoi(tokens[2].c_str());
        }else{
            return -1;
        }
    }
}
// for all obtional tags
string AlignmentRecord::getStringTag(const string & tag) const{
    map<string,string>mytags=tags;
    vector<string> tokens=stringSplit(mytags[tag],':');
    if(tokens.size()>0){
        return tokens[2].c_str();
    }else{
        return "";
    }
}

bool AlignmentRecord::isPaired(void) const{
    return (flag & SAM_FPAIRED);
}

bool AlignmentRecord::isMapped(void) const{
    return !(flag & SAM_FUNMAP);
}

bool AlignmentRecord::isSecondary(void) const{
    return (flag & SAM_FSECONDARY);
}

bool AlignmentRecord::isReverse(void) const{
    return (flag & SAM_FREVERSE);
}

bool AlignmentRecord::isDuplicate(void) const{
    return (flag & SAM_FDUP);
}

bool AlignmentRecord::isRead1(void) const{
    return (flag & SAM_FREAD1);
}

bool AlignmentRecord::isRead2(void) const{
    return (flag & SAM_FREAD2);
}

int AlignmentRecord::getStart(void) const{
    return pos;
}

int AlignmentRecord::getEnd(void) const{
    return end;
}

int AlignmentRecord::getMLength(){
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

int AlignmentRecord::getMappedLength(void) const{
    return (end-pos);
}

bool AlignmentRecord::isValid(const int & minLength) const{
    return ((getEnd()-getStart()) > 0) && (getMappedLength()>minLength);
}

// identity= match/(maped length)= (M-XM)/M
float AlignmentRecord::getIdentity(void) const{
    int mappedLength=getMappedLength();
    return ((float)(mappedLength-getNumericTag("XM"))/mappedLength);
}

std::vector<string> AlignmentRecord::stringSplit(const string & record, char split) const{
    vector<string> tokens;
    istringstream iss(record);
    string token;
    while(getline(iss, token, split)){
        tokens.push_back(token);
    }
    return tokens;
}

int AlignmentRecord::getStrand(void) const{
    if(isRead1()){
        return 1;
    }else{
        return 2;
    }
}

void AlignmentRecord::setStart(const int & start){
    this->pos=start;
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

string AlignmentRecord::toString(void) const{
    stringstream ss;
    ss<<qname<<"_"<<getStrand()<<" "<<rname<<" "<<pos<<" "<<end;
    return ss.str();
}