#include "SamParser.h"
using namespace std;
SamParser::SamParser(){
}

SamParser::~SamParser(){
}

SamParser::SamParser(std::string path){
	samPath=path;
}

void SamParser::parser(HEADER & refInfor,std::vector<std::string> & qNames,
	ALIGNRECORDS &records,int minLength, float identity){
	if(samPath==""){
		cout<<"Sam path is invalid"<<endl;
		return;	
	}
	// open file
	ifstream inputHandle(samPath);
	if(!inputHandle.is_open()){
		cout<<"Can not open sam file"<<endl;
		return;
	}
	// Read file line by line
	string line;
	// int i=0;
	while(getline(inputHandle,line)){
		// read header, get reference infor
		if(line[0]=='@'){
			if(line.substr(0,3)=="@SQ"){
				std::vector<string> tokens=stringSplit(line,'\t');
				string scaffoldName=stringSplit(tokens[1],':')[1];
				int scaffoldLength=atoi(stringSplit(tokens[2],':')[1].c_str());
				refInfor[scaffoldName]=scaffoldLength;
			}
		}else{
			// if(i>100000) break;
			// i++;
			// Read the alignment records
			AlignmentRecord a(line);
			if ((a.getIdentity()> identity) && a.isValid(minLength)&& a.isMapped()){
				records[a.getQname()][a.getRname()].push_back(a);
			}
		}
	}	
	inputHandle.close();
	// read name
	for (ALIGNRECORD_ITER record=records.begin(); record!=records.end();record++){
		qNames.push_back(record->first);
	}
}

bool SamParser::writeLengthFile(const std::string & path){
	ofstream output_handle(path);
	if(!output_handle.is_open()){
		cout<<"Can not open file length"<<endl;
		return false;
	}
	for(HEAD_ITER ref=header.begin(); ref!=header.end();ref++){
		output_handle<<ref->first<<" "<<ref->second<<endl;
	}
	output_handle.close();
	return true;
}

std::vector<string> SamParser::stringSplit(const string & record, char split){
    vector<string> tokens;
    istringstream iss(record);
    string token;
    while(getline(iss, token, split)){
        tokens.push_back(token);
    }
    return tokens;
}

bool SamParser::writeFinalFile(ALIGNRECORDS &records,const std::string & path){
	ofstream output_handle(path);
	if(!output_handle.is_open()){
		cout<<"Can not open file length"<<endl;
		return false;
	}
	for(ALIGNRECORD_ITER record = records.begin(); record!=records.end(); record++){
		SCAFFOLDS & scaffolds=record->second;
		// count single map or multiple map
		map<int,int> strand_count;
		strand_count[1]=0;
		strand_count[2]=0;
		for(SCAFFOLD_ITER scaffold=scaffolds.begin();scaffold!=scaffolds.end();scaffold++){
			READS reads=scaffold->second;
			for(auto read:reads){
				if(read.isKeep()){
					strand_count[read.getStrand()]++;
				}
			}
		}
		// print output
		for(SCAFFOLD_ITER scaffold = scaffolds.begin(); scaffold!=scaffolds.end();scaffold++){
			READS & reads=scaffold->second;
			for(auto const& read:reads){
				if(read.isKeep()){
					output_handle<<read.toString()<<" "<<strand_count[read.getStrand()]<<endl;
				}
			}
		}
	}
	output_handle.close();
	return true;
}