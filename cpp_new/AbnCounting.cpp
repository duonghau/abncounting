#include "AbnCounting.h"
using namespace std;

map<string,std::vector<float>> abnVectors;
mutex mu;
void initAbnVector(){
	// refsLength;
	for(REFERENCE_ITER reference=refsLength.begin();reference!=refsLength.end();reference++){
		abnVectors[reference->first]=vector<float>(reference->second);
	}
}
//start, end is rezo-base
void addVector(const std::string &refName,const int &start, const int &end,const float & value){
	mu.lock();
	std::vector<float> &vec=abnVectors[refName];
	if(vec.size()>0){
		for(int i=start;i<end;i++){
			vec[i]+=value;
		}
	}
	mu.unlock();
}

void countUnique(const int &start, const int &end){
	for(int i=start;i<end;i++){
		RECORDS &reads1=recordsTrans[readIDs[i]][1];
		RECORDS &reads2=recordsTrans[readIDs[i]][2];	
		if(reads1.size()==1){
			addVector(reads1[0].getRname(),reads1[0].getStart(),reads1[0].getEnd(),1);
		}
		if(reads2.size()==1){
			addVector(reads2[0].getRname(),reads2[0].getStart(),reads2[0].getEnd(),1);
		}
	}
}

void countMultiple(const int &start, const int &end){
	for(int i =start;i<end;i++){
		RECORDS &reads1=recordsTrans[readIDs[i]][1];
		RECORDS &reads2=recordsTrans[readIDs[i]][2];
		if(reads1.size()>1){
			vector<float> hits=getHitRatio(reads1);
			for(int j=0;j<reads1.size();j++){
				addVector(reads1[j].getRname(),reads1[j].getStart(),reads1[j].getEnd(),hits[j]);
			}
		}
		if(reads2.size()>1){
			vector<float> hits=getHitRatio(reads2);
			for(int j=0;j<reads2.size();j++){
				addVector(reads2[j].getRname(),reads2[j].getStart(),reads2[j].getEnd(),hits[j]);
			}
		}
	}
}

vector<float> getHitRatio(const RECORDS &reads){
	vector<float> ratios=vector<float>(reads.size());
	vector<float> meanAbn=vector<float>(reads.size());
	for(int i=0;i<reads.size();i++){
		meanAbn[i]=getMeanAbn(reads[i].getRname(),reads[i].getStart(),reads[i].getEnd());
	}
	float sum=sumFloat(meanAbn);
	if(sum==0){
		float ratio=(float)1/reads.size();
		for(int i=0;i<reads.size();i++){
			ratios[i]=ratio;
		}
	}else{
		for(int i=0;i<reads.size();i++){
			ratios[i]=(float)meanAbn[i]/sum;
		}
	}
	return ratios;
}

float sumFloat(const std::vector<float> &vec){
	float sum=0;
	for(int i=0;i<vec.size();i++){
		sum+=vec[i];
	}
	return sum;
}

float getMeanAbn(const std::string &refName,const int &start, const int &end){
	std::vector<float> &vec=abnVectors[refName];
	int sum=0;	
	for(int i=start;i<end;i++){
		sum+=vec[i];
	}
	return ((float)sum/(end-start));
}

void countAbn(const int &nThreads){
	// count unique
	thread threads[nThreads];
	int nRecord=readIDs.size();
	int rowsPerThread=getRecordPerPart(nRecord,nThreads);	
	if(nThreads==1){
		countUnique(0,nRecord);
	}else{
		int begin=0;
		int end=0;
		for(int i=0;i<nThreads-1;i++){
			begin = i*rowsPerThread;
			end = (i+1)*(rowsPerThread);
			threads[i]=thread(countUnique,begin,end);
		}
		threads[nThreads-1]=thread(countUnique,end,nRecord);
		for(int i=0;i<nThreads;i++){
			if(threads[i].joinable()){
				threads[i].join();
			}
		}
	}
	// count multiple
	if(nThreads==1){
		countMultiple(0,nRecord);
	}else{
		int begin=0;
		int end=0;
		for(int i=0;i<nThreads-1;i++){
			begin = i*rowsPerThread;
			end = (i+1)*(rowsPerThread);
			threads[i]=thread(countMultiple,begin,end);
		}
		threads[nThreads-1]=thread(countMultiple,end,nRecord);
		for(int i=0;i<nThreads;i++){
			if(threads[i].joinable()){
				threads[i].join();
			}
		}
	}
}

void writeFinalFile(const std::string & path){
	ofstream output_handle(path);
	if(!output_handle.is_open()){
		throw "Can not open file length";
	}
	for(map<string,std::vector<float>>::iterator record = abnVectors.begin(); record!=abnVectors.end(); record++){
		output_handle<<record->first;
		vector<float> &abn= record->second;
		for(int i=0;i<abn.size();i++){
			output_handle<<"\t"<<abn[i];
		}
		output_handle<<endl;
	}
	output_handle.close();
}

int main(int argc, char const *argv[])
{	
	/* code */
	string fileSam=argv[1];
	string fileResult=argv[2];
	int nThreads=atoi(argv[3]);
	int minLength=atoi(argv[4]);
	int readLength=atoi(argv[5]);
	time_t start = time(0);
	parser(fileSam,minLength,0.9,nThreads);
	preProcessData(readLength,minLength,nThreads);
	initAbnVector();
	countAbn(nThreads);
	writeFinalFile(fileResult);
	double seconds_since_start = difftime( time(0), start);
	cout<<"Elapsed time:"<<seconds_since_start<<"(s)"<<endl;
	cout<<"Reads: "<<readIDs.size()<<endl;
	int length=0;
	for(REFERENCE_ITER ref=refsLength.begin();ref!=refsLength.end();ref++){
		length+=ref->second;
	}
	cout<<"Scaffold lengths: "<<length<<endl;
	return 0;
}