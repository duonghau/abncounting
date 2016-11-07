#include "AlignmentRecord.h"
using namespace std;
AlignmentRecord::AlignmentRecord(){

}
AlignmentRecord::AlignmentRecord(string alignment_record,int rpair){
	std::vector<string> tokens=record_parser(alignment_record);
	this->qname=tokens[0];
	this->flag=str2int(tokens[1]);
	this->rname=tokens[2];
	this->pos=str2int(tokens[3]);
	this->mapq=str2int(tokens[4]);
	this->cigar=tokens[5];
	this->rnext=tokens[6];
	this->pnext=str2int(tokens[7]);
	this->tlen=str2int(tokens[8]);
	this->seq=tokens[9];
	this->qual=tokens[10];	
	this->XM=tokens[13];
	this->MN=tokens[16];
	this->rpair=rpair;
	cout<<this->cigar<<" "<<this->seq<<"\n";
}
int AlignmentRecord::str2int (string str) {
  stringstream ss(str);
  int num;
  ss >> num;
  return num;
}

string AlignmentRecord::to_string(void){
	stringstream ss;
	ss<<this->qname<<"\t";
	ss<<this->flag<<"\t";
	ss<<this->rname;
	return ss.str();
}
vector<string> AlignmentRecord::record_parser(string record){
	vector<string> tokens;
	istringstream iss(record);
	string token;
	while(getline(iss, token, '\t')){
        tokens.push_back(token);
	}
	return tokens;
}
int AlignmentRecord::start(void){
	return this->start;
}
int AlignmentRecord::end(void){
	
}
int AlignmentRecord::mapped_length(void){
	
}
bool AlignmentRecord::is_valid(void){

}