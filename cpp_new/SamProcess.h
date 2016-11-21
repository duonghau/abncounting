#ifndef SAMPROCESS_H
#define SAMPROCESS_H
#include <thread>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include <iostream>
#include "AlignmentRecord.h"

typedef std::unordered_map<std::string,std::unordered_map<std::string,std::vector<AlignmentRecord> > > GROUPRECORDS;
typedef std::unordered_map<std::string,std::unordered_map<std::string,std::vector<AlignmentRecord> > >::iterator GROUPRECORD_ITER;
typedef std::unordered_map<std::string,std::unordered_map<int,std::vector<AlignmentRecord> > > GROUPCOUNTS;
typedef std::unordered_map<std::string,std::unordered_map<int,std::vector<AlignmentRecord> > >::iterator GROUPCOUNT_ITER;
typedef std::unordered_map<std::string,std::vector<AlignmentRecord> > SCAFFOLDS;
typedef std::unordered_map<std::string,std::vector<AlignmentRecord> >::iterator SCAFFOLD_ITER;
typedef std::vector<AlignmentRecord> RECORDS;
typedef std::vector<AlignmentRecord>::iterator RECORD_ITER;
typedef std::unordered_map<std::string, int> REFERENCES;
typedef std::unordered_map<std::string, int>::iterator REFERENCE_ITER;
typedef std::vector<std::string> READIDS;
// typedef std::unordered_map<std::string,std::unordered_map<int,int> > MAPCOUNTS;
// typedef std::unordered_map<std::string,std::unordered_map<int,int> >::iterator MAPCOUNT_ITER;

// Global variables
extern GROUPRECORDS records;
extern GROUPCOUNTS recordsTrans;
extern REFERENCES refsLength;
extern READIDS readIDs;
// extern MAPCOUNTS mapCounts;

// records
// refs
// readsID
// reads mapped
void parserRecords(const std::vector<std::string> & mapRecords,const int &start,const int &end, const int &minLength, const float & identity);
void parserHeader(const std::vector<std::string> & header,const int &start, const int &end);
void parser(const std::string & filePath,const float &minLength,const float &identity, const int &nThreads);
void ajustRecords(const int &start, const int &end, const int &readLength, const int &minLength);
void ajustRecord(AlignmentRecord & record1, AlignmentRecord & record2, const int &readLength, const int &minLength);
// void getMapCount(void);
void removeInvalidRecord(void);
int getRecordPerPart(const int &nRecord, const int &nThread);
void preProcessData(const int &readLength,const int &minLength, const int &nThreads);
void writeLengthFile(const std::string & path);
void transformData();
#endif
