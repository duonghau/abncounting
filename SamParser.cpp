#include "SamParser.h"
using namespace std;
SamParser::SamParser(){

}
std::map<std::string,AlignmentRecord> SamParser::parser(std::string read1_mapped, std::string read2_mapped){
	std::map<std::string,AlignmentRecord> records;
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
			if(line[0]=='@'){
				continue; //check if line is header
			}else{
				
				AlignmentRecord record = AlignmentRecord(line,1);
				records[]=record;
			}
		}
	}else{
		throw;
	}
	file_handle1.close();
	file_handle2.close();
	return records;
}