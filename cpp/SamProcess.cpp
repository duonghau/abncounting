#include "SamProcess.h"
using namespace std;

void ajustRecords(ALIGNRECORDS & records, int readLength, int minLength){
	for(ALIGNRECORD_ITER record = records.begin(); record!=records.end(); record++){
		SCAFFOLDS & scaffolds=record->second;
		for(SCAFFOLD_ITER scaffold = scaffolds.begin(); scaffold!=scaffolds.end();scaffold++){
			READS & reads=scaffold->second;
			//  if there are only one record in a group, do nothing
   			//  else ajust position
			if(reads.size()<=1) continue;
			for(auto & read1:reads){
				for(auto & read2:reads){
					if(&read1==&read2) continue;
					ajustRecord(read1, read2, readLength, minLength);
				}
			}
		}
	}
}
void ajustRecord(AlignmentRecord &record1, AlignmentRecord &record2, int readLength, int minLength){
	// if one read is unkeep, return
	if (!record1.isKeep()||!record2.isKeep()){
		return;
	}
	// check if one read mapped multiple time to a scaffold	
	if(record1.getStrand()==record2.getStrand()){
		int distance=min(abs(record2.getStart()-record1.getEnd()),abs(record1.getStart()-record2.getEnd()));
		if (distance < readLength){
			if (record1.getMappedLength() < record2.getMappedLength()){
                record1.unKeep();
            }else{
                record2.unKeep();
			}           
		}            
	}else{
		 // record1 is in the left of record2
        if ((record1.getEnd() - record2.getStart())>=0 && (record2.getEnd() - record1.getStart())>=0 && record2.getStart()> record1.getStart()){
             // determine which record is read_1
            if (record1.getStrand()==1){
            	record1.setStart(record1.getEnd()+1);
            }
            else{
            	record2.setEnd(record1.getStart()-1);
            }
        }
        // record1 is in the right of record2
        else if ((record2.getEnd() - record1.getStart())>=0 && (record1.getEnd() - record2.getStart())>=0 && record1.getStart()>record2.getStart()){
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

int main(int argc, char const *argv[])
{   string filename="../data/MH0015.sam";
	string filelen="../data/MH0015.len";
	string filefinal="../data/MH0015.final";
	SamParser sam(filename);
	ALIGNRECORDS records=sam.parser(35,0.9);
	ajustRecords(records,75,35);
	sam.writeLengthFile(filelen);
	sam.writeFinalFile(records,filefinal);
	cout<<"Done";
	return 0;
}
