#include "SamProcess.h"
using namespace std;

void ajustRecords(ALIGNRECORDS &records,const std::vector<std::string> & qNames,int begin, int end, int readLength, int minLength){
	
	// for(ALIGNRECORD_ITER record = records.begin(); record!=records.end(); record++){
	for(int i=begin;i<end;i++){		
		SCAFFOLDS & scaffolds=records[qNames[i]];
		for(SCAFFOLD_ITER scaffold = scaffolds.begin(); scaffold!=scaffolds.end();scaffold++){
			READS & reads=scaffold->second;
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
void ajustRecord(AlignmentRecord &record1, AlignmentRecord &record2, int readLength,
	int minLength){
	// if one read is unkeep, return
	if (!record1.isKeep()||!record2.isKeep()){
		return;
	}
	// check if one read mapped multiple time to a scaffold	
	if(record1.getStrand()==record2.getStrand()){
		int distance=min(abs(record2.getStart()-record1.getEnd()),
			abs(record1.getStart()-record2.getEnd()));
		if (distance < readLength){
			if (record1.getMappedLength() < record2.getMappedLength()){
                record1.unKeep();
            }else{
                record2.unKeep();
			}
		}
	}else{
		 // record1 is in the left of record2
        if ((record1.getEnd() - record2.getStart())>=0 && (record2.getEnd() - record1.getStart())>=0
        	&& record2.getStart()> record1.getStart()){
             // determine which record is read_1
            if (record1.getStrand()==1){
            	record2.setStart(record1.getEnd()+1);
            }
            else{
            	record2.setEnd(record1.getStart()-1);
            }
        }
        // record1 is in the right of record2
        else if ((record2.getEnd() - record1.getStart())>=0 && (record1.getEnd() - record2.getStart())>=0 
        	&& record1.getStart()>record2.getStart()){
            if (record1.getStrand()==1){
            	record2.setEnd(record1.getStart()-1);
            }else{
            	record2.setStart(record1.getEnd()+1);
            }
        }
        //  record2 is in record1
        else if ((record2.getStart() - record1.getStart())>=0 && (record2.getEnd() - record1.getEnd())<=0){
            record2.unKeep();
        }
        // record1 in record2
        else if ((record1.getStart()-record2.getStart())>=0 && (record1.getEnd() - record2.getEnd())<=0){
            record1.unKeep();
        }
	}

	if (record1.isKeep()){
        if (!record1.isValid(minLength)){
            record1.unKeep();
        }
	}
    if (record2.isKeep()){
        if (!record2.isValid(minLength)){
            record2.unKeep();
        }
    }
}
void preProcess(ALIGNRECORDS &records,const std::vector<std::string> & qNames, int nthreads){
	int nRead=qNames.size();
	int readsPerPath=floor(nRead/nthreads);
	int begin=0;
	int end=nRead;
	thread threads[nthreads-1];
	if(readsPerPath>0 && nthreads>1){
		for(int i=0;i<nthreads-1;i++){
			begin=(readsPerPath*i);
			end=(readsPerPath*(i+1)-1);
			threads[i]=thread(ajustRecords,std::ref(records),std::ref(qNames), begin,end,75,35);
		}
	}
	if(begin!=0){
		begin=end+1;
	}
	ajustRecords(std::ref(records),std::ref(qNames), begin,nRead,75,35);
	for(int i=0;i<nthreads-1;i++){
		if(threads[i].joinable()){
			threads[i].join();
		}
	}
}

int main(int argc, char const *argv[])
{   
	int nRead=qNames.size();
	int readsPerPath=floor(nRead/numthread);
	int begin=0;
	int end=nRead;
	clock_t beginTime = clock();
	thread threads[numthread-1];
	if(readsPerPath>0 && numthread>1){
		for(int i=0;i<numthread-1;i++){
			begin=(readsPerPath*i);
			end=(readsPerPath*(i+1)-1);
			threads[i]=thread(ajustRecords,qNames, begin,end,75,35);
		}
	}
	cout<<"end"<<end<<endl;
	if(begin!=0){
		begin=end+1;
	}
	ajustRecords(qNames, begin,nRead,75,35);
	for(int i=0;i<numthread-1;i++){
		if(threads[i].joinable()){
			threads[i].join();
		}
	}
	clock_t endTime = clock();
	double elapsed = double(endTime - beginTime) / CLOCKS_PER_SEC;
	cout<<"Elapsed time:"<<elapsed<<"(s)"<<endl;
	// clock_t beginTime = clock();
	// ajustRecords(qNames, 0,nRead,75,35);
	// clock_t endTime = clock();
	// double elapsed = double(endTime - beginTime) / CLOCKS_PER_SEC;
	// sam.writeLengthFile(filelen);
	// sam.writeFinalFile(records,filefinal);
	// cout<<"Done";
	// cout<<"Elapsed time:"<<elapsed<<"(s)"<<endl;
	return 0;
}
