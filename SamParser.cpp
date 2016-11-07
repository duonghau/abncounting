#include "SamParser.h"
using namespace std;
SamParser::SamParser(){

}
std::map<std::string,AlignmentRecord> SamParser::parser(std::string read1_mapped, std::string read2_mapped){
	map<string,AlignmentRecord> records;
	ifstream file_handle1, file_handle2;
	try{
		file_handle1.open(read1_mapped.c_str());
		file_handle2.open(read2_mapped.c_str());			
	}catch(exception& e){
		throw;
	}
	if(file_handle1.is_open() && file_handle2.is_open()){
		//read file 1
		string line;
		while(getline(file_handle1,line)){
			//check if line is header or empty
			if(line[0]=='@' or line==""){
				continue;
			}else{
				size_t found=line.find('\t');
				string rname=line.substr(0,found);
				AlignmentRecord record = AlignmentRecord(line.c_str(),1);
				records[rname]=record;
			}
		}
		while(getline(file_handle2,line)){
			//check if line is header or empty
			if(line[0]=='@' or line==""){
				continue;
			}else{
				size_t found=line.find('\t');
				string rname=line.substr(0,found);
				AlignmentRecord record = AlignmentRecord(line,2);
				records[rname]=record;
			}
		}
	}else{
		throw "Can't not open file sam";
	}
	file_handle1.close();
	file_handle2.close();
	return records;
}