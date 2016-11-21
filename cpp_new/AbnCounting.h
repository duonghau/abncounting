#ifndef ABNCOUNTING_H
#define ABNCOUNTING_H
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <time.h>
#include <thread>
// #include <mutex>
#include "SamProcess.h"

void initAbnVector();
void countUnique(const int &start, const int &end);
void countMultiple(const int &start, const int &end);
void countAbn(const int &nThreads);
void addVector(const std::string &refName,const int &start, const int &end,const float & value);
std::vector<float> getHitRatio(const RECORDS &reads);
float sumFloat(const std::vector<float> &vec);
float getMeanAbn(const std::string &refName,const int &start, const int &end);
void writeFinalFile(const std::string & path);
int main(int argc, char const *argv[]);
#endif
