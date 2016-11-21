#include "SamProcess.h"

GROUPRECORDS records;
GROUPCOUNTS recordsTrans;
REFERENCES refsLength;
READIDS readIDs;
// MAPCOUNTS mapCounts;
using namespace std;
mutex m;
int getRecordPerPart(const int &nRecord, const int &nThread){
	return floor(nRecord/nThread);
}

void parserHeader(const std::vector<string> &header, const int &start, const int &end){
	for(int i=start;i<end;i++){
		std::vector<string> tokens=stringSplit(header[i],'\t');
		string scaffoldName=stringSplit(tokens[1],':')[1];
		int scaffoldLength=atoi(stringSplit(tokens[2],':')[1].c_str());
		m.lock();
		refsLength[scaffoldName]=scaffoldLength;
		m.unlock();
	}
}

void parserRecords(const std::vector<string> & mapRecords,const int &start,const int &end,const int &minLength, const float & identity){
	for(int i=start;i<end;i++){
		AlignmentRecord a(mapRecords[i]);
		if ((a.getIdentity()> identity) && a.isValid(minLength)&& a.isMapped()){
			m.lock();
			records[a.getQname()][a.getRname()].push_back(a);
			m.unlock();
		}

	}
}

void parser(const string & filePath,const float &minLength,const float &identity, const int &nThreads){
	std::vector<string> header;
	std::vector<string> mapRecords;
	if(filePath==""){
		throw "File path is empty";
	}

	ifstream inputHandle(filePath);
	if(!inputHandle.is_open()){		
		throw "Can't not open file SAM";
	}
	//read entire file
	string line;
	// int i=0;
	while(getline(inputHandle,line)){
		if(line[0]=='@'){
			if(line.substr(0,3)=="@SQ"){
				header.push_back(line);
			}			
		}else{
			// if(i>10000){
			// 	break;
			// }
			// i++;
			mapRecords.push_back(line);
		}
	}
	// parser header
	thread threads[nThreads];
	int nRecord=header.size();
	int rowsPerThread=getRecordPerPart(nRecord,nThreads);
	// cout<<nThreads<<endl;
	if(nThreads==1){
		parserHeader(header,0,nRecord);		
	}else{
		int begin=0;
		int end=0;
		for(int i=0;i<nThreads-1;i++){
			begin = i*rowsPerThread;
			end = (i+1)*(rowsPerThread);
			threads[i]=thread(parserHeader,std::ref(header),begin,end);
		}
		threads[nThreads-1]=thread(parserHeader,std::ref(header),end,nRecord);
		for(int i=0;i<nThreads;i++){
			if(threads[i].joinable()){
				threads[i].join();
			}
		}
	}
	// parser records content	
	nRecord=mapRecords.size();
	rowsPerThread=getRecordPerPart(nRecord,nThreads);
	if(nThreads==1){
		parserRecords(mapRecords, 0,nRecord, minLength, identity);
	}else{
		int begin=0;
		int end=0;
		for(int i=0;i<nThreads-1;i++){
			begin = i*rowsPerThread;
			end = (i+1)*(rowsPerThread);
			threads[i]=thread(parserRecords,std::ref(mapRecords),begin,end,minLength,identity);
		}
		threads[nThreads-1]=thread(parserRecords,std::ref(mapRecords),end,nRecord,minLength,identity);
		for(int i=0;i<nThreads;i++){
			if(threads[i].joinable()){
				threads[i].join();
			}
		}
	}
	// get all query name
	for(GROUPRECORD_ITER record = records.begin(); record!=records.end();record++){
		readIDs.push_back(record->first);
	}
	inputHandle.close();
}

void ajustRecords(const int &start, const int &end, const int &readLength, const int &minLength){
	
	// for(ALIGNRECORD_ITER record = records.begin(); record!=records.end(); record++){
	for(int i=start;i<end;i++){		
		SCAFFOLDS & scaffolds=records[readIDs[i]];
		for(SCAFFOLD_ITER scaffold = scaffolds.begin(); scaffold!=scaffolds.end();scaffold++){
			RECORDS & reads=scaffold->second;
			//  if there are only one record in a group, do nothing
   			//  else ajust position
			if(reads.size()<=1) continue;
			for(auto & read1:reads){
				for(auto & read2:reads){
					if(&read1==&read2){
						continue;
					}					
					ajustRecord(read1, read2, readLength, minLength);
				}
			}
		}
	}
}

void ajustRecord(AlignmentRecord &record1, AlignmentRecord &record2, const int &readLength,
	const int &minLength){
	// if one read is unkeep, return
	if (!record1.isKeep()||!record2.isKeep()){
		return;
	}
	// check if one read mapped multiple time to a scaffold	
	if(record1.getRead()==record2.getRead()){
		int distance=min(abs(record2.getStart()-record1.getEnd()),
			abs(record1.getStart()-record2.getEnd()));
		if (distance < readLength){
			m.lock();
			if ((record1.getEnd()-record1.getStart()) < (record2.getEnd()-record2.getStart())){
                record1.unKeep();
            }else{
                record2.unKeep();
			}
			m.unlock();
		}
	}else{
		 // record1 is in the left of record2
        if ((record1.getEnd() - record2.getStart())>=0 && (record2.getEnd() - record1.getStart())>=0
        	&& record2.getStart()> record1.getStart()){
             // determine which record is read_1
        	m.lock();
            if (record1.getRead()==1){
            	record2.setStart(record1.getEnd()+1);
            }
            else{
            	record2.setEnd(record1.getStart()-1);
            }
            m.unlock();
        }
        // record1 is in the right of record2
        else if ((record2.getEnd() - record1.getStart())>=0 && (record1.getEnd() - record2.getStart())>=0 
        	&& record1.getStart()>record2.getStart()){
        	m.lock();
            if (record1.getRead()==1){
            	record2.setEnd(record1.getStart()-1);
            }else{
            	record2.setStart(record1.getEnd()+1);
            }
            m.unlock();
        }
        //  record2 is in record1
        else if ((record2.getStart() - record1.getStart())>=0 && (record2.getEnd() - record1.getEnd())<=0){
            m.lock();
            record2.unKeep();
            m.unlock();
        }
        // record1 in record2
        else if ((record1.getStart()-record2.getStart())>=0 && (record1.getEnd() - record2.getEnd())<=0){
            m.lock();
            record1.unKeep();
            m.unlock();
        }
	}

	if (record1.isKeep()){
        if (!record1.isValid(minLength)){
            m.lock();
            record1.unKeep();
            m.unlock();
        }
	}
    if (record2.isKeep()){
        if (!record2.isValid(minLength)){
        	m.lock();
            record2.unKeep();
            m.unlock();
        }
    }
}

void preProcessData(const int &readLength,const int &minLength,const int &nThreads){
	// ajust position
	thread threads[nThreads];
	int nRecord=readIDs.size();
	int rowsPerThread=getRecordPerPart(nRecord,nThreads);	
	if(nThreads==1){
		ajustRecords(0,nRecord,readLength, minLength);
	}else{
		int begin=0;
		int end=0;
		for(int i=0;i<nThreads-1;i++){
			begin = i*rowsPerThread;
			end = (i+1)*(rowsPerThread);
			threads[i]=thread(ajustRecords,begin,end,readLength, minLength);
		}
		threads[nThreads-1]=thread(ajustRecords,end,nRecord,readLength, minLength);
		for(int i=0;i<nThreads;i++){
			if(threads[i].joinable()){
				threads[i].join();
			}
		}
	}
	transformData();//create new map for counting
}



void transformData(){
	for(GROUPRECORD_ITER record = records.begin(); record!=records.end(); record++){
		SCAFFOLDS & scaffolds=record->second;
		for(SCAFFOLD_ITER scaffold = scaffolds.begin(); scaffold!=scaffolds.end();scaffold++){
			RECORDS & reads=scaffold->second;
			for(auto const& read:reads){
				if(read.isKeep()){
					// output_handle<<read.toString()<<" "<<strand_count[read.getStrand()]<<endl;
					recordsTrans[read.getQname()][read.getRead()].push_back(read);
				}
			}
		}
	}
}

void writeLengthFile(const string & path){
	ofstream output_handle(path);
	if(!output_handle.is_open()){
		throw "Can't open length file";
	}
	for(REFERENCE_ITER ref=refsLength.begin(); ref!=refsLength.end();ref++){
		output_handle<<ref->first<<" "<<ref->second<<endl;
	}
	output_handle.close();
}