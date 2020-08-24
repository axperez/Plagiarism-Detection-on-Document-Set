// ============================================================
//      P1.hpp
// ------------------------------------------------------------
// Author :     Axel Perez
// Date :       6/30/20
// ============================================================

// include files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <ctime>       
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>

#include "VectorIntHash.hpp"

// constant declarations
int const SHINGLE_LENGTH = 9;
int const SIGNATURES = 320;
float const THRESHOLD = 0.8;
int const NUM_BANDS = 40;
std::string const FILE_PATH = "/home/ax/School/COEN-281-SUM20/P1/data/";

// function prototype
bool input_docs(std::vector<std::string> &docs, std::string path);
void shingling(std::vector<std::string> &docs, std::unordered_map<std::string, std::vector<int>> &shingles, int k);
std::vector<std::vector<int>> minHashing(std::unordered_map<std::string, std::vector<int>> &shingles, std::vector<std::vector<int>> &randPerms, int numDocs, int s);
std::map<int, std::unordered_set<int>> lsh(std::vector<std::vector<int>> &minHashTable, int bands);
void transpose2dVector(std::vector<std::vector<int>> &vec, std::vector<std::vector<int>> &transpVec, int numRows, int numCols);
void findPlagiarism(std::map<int, std::unordered_set<int>> candidatePairs, std::vector<std::vector<int>> &minHashTable, std::vector<std::string> docs, float threshold);
float jaccardSim(std::vector<int> v1, std::vector<int> v2);
double and_or(double x);
