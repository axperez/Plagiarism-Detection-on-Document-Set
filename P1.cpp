// =========================================================
//      P1.cpp
// 
// ---------------------------------------------------------
// Author:      Axel Perez
// Date:        6/30/20
// =========================================================

#include "P1.hpp"
using namespace std;

bool input_docs(vector<string> &docs, string path = FILE_PATH) {
    string input;
    FILE *file;

    while (cin >> input) {
        if (file = fopen((path + input).c_str(), "r")) {
            fclose(file);
            docs.push_back(input);
        }
        else
        {
            cout << "File " << input << " does not exist in " << FILE_PATH << " directory\n" << endl;
        }   
    }

    if (docs.size() < 2) {
        return false;
    }
    return true;
}

void shingling(vector<string> &docs, unordered_map<string, vector<int>> &shingles, int k = SHINGLE_LENGTH) {
    string text, processedText, tempShingle;
    char c;
    int i, j;
    bool first;
    unordered_set<char> punct {'!', '.', '?', '(', ')', ',', ':', '/', '\"', '-'};
    vector<int> zeroVector(docs.size(), 0);

    for (i = 0; i < docs.size(); i++) {
        ifstream MyReadFile(FILE_PATH + docs[i]);
        processedText = "";
        first = true;
        
        while(MyReadFile.get(c)) {
            if (isspace(c) || punct.count(c)) {
                if (first) {
                    first = false;
                    processedText += " ";
                }
            }
            else
            {
                first = true;
                processedText += tolower(c);
            }
        }
        //cout << "DEBUG: " << processedText << endl;
        for (j = 0; j < (processedText.size() - k + 1); j++) {
            tempShingle = processedText.substr(j, 9);
            //cout << "DEBUG" << tempShingle << endl;
            if (!shingles.count(tempShingle)) {
                shingles[tempShingle] = zeroVector;
            }
            shingles[tempShingle][i] = 1;
        }
    } 
    cout << "Total number of unique shingles in all docs: " << shingles.size() << endl;
}

vector<vector<int>> minHashing(unordered_map<string, vector<int>> &shingles, vector<vector<int>> &randPerms, int numDocs, int s = SIGNATURES) {
    vector<int> tempPerm;
    int i, j, perm, docIndex, shingleIndex = 0;
    //srand ( unsigned ( time(0) ) );
    srand ( unsigned ( 234 ) );
    vector<vector<int>> minHashTable(s, vector<int> (numDocs, numeric_limits<int>::max()));

    for (perm = 0; perm < s; perm++) {
        randPerms.push_back(tempPerm);
        // unordered_set some values:
        for (i=1; i<= shingles.size(); ++i) randPerms[perm].push_back(i);
        // using built-in random generator:
        random_shuffle ( randPerms[perm].begin(), randPerms[perm].end() );
    }
    cout << "a)\n" << "Shingles:\t\tRandom Permutations:";
    for (i = 0; i < s; i++) cout << "\t";
    cout << "Characteristic Matrix (Shingles x Documents):" << endl;

    for (auto const& shingle : shingles ) {
        cout << shingle.first << ":\t\t";
        for (perm = 0; perm < s; perm++) {
            cout << randPerms[perm][shingleIndex] << "\t";
        }
        cout << "\t\t";
        docIndex = 0;
        for (auto& doc : shingle.second) {
            cout << doc << " ";
            if (doc == 1) {
                for (perm = 0; perm < s; perm++) {
                    if (randPerms[perm][shingleIndex] < minHashTable[perm][docIndex]) {
                        minHashTable[perm][docIndex] = randPerms[perm][shingleIndex];
                    }
                }
            }
            docIndex++;
        }
        cout << "\n" << endl;
        shingleIndex++;
    }
    cout << "b)\nMinHash Table:" << endl;
    for (i = 0; i < s; i++) {
        for (j = 0; j < numDocs; j++) {
            cout << minHashTable[i][j] << "\t";
        }
        cout << endl;
    }

    return minHashTable;
}

map<int, unordered_set<int>> lsh(vector<vector<int>> &minHashTable, int bands = NUM_BANDS) {
    int rows = minHashTable.size() / bands;
    int numDocs = minHashTable[0].size();
    VectorIntHash lsHash(rows);
    vector<vector<int>> transpMHT(numDocs, vector<int> (rows*bands));

    transpose2dVector(minHashTable, transpMHT, rows*bands, numDocs);

    for (int start = 0, end = rows; start < rows*bands; start += rows, end += rows) {
        for (int doc = 0; doc < numDocs; doc++) {
            lsHash.insert(vector<int>(transpMHT[doc].begin() + start, transpMHT[doc].begin() + end), doc);
        }
        lsHash.clearHashTable();
    }
    return lsHash.candidatePairs;
}

void transpose2dVector(vector<vector<int>> &vec, vector<vector<int>> &transpVec, int numRows, int numCols) {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            transpVec[j][i] = vec[i][j];
        }
    }
}

void findPlagiarism(map<int, unordered_set<int>> candidatePairs, vector<vector<int>> &minHashTable, vector<string> docs, float threshold = THRESHOLD) {
    int numDocs = minHashTable[0].size();
    int numSigs = minHashTable.size(), count = 0, flag = 0;
    float tempSim;
    vector<vector<int>> transpMHT(numDocs, vector<int> (numSigs));

    transpose2dVector(minHashTable, transpMHT, numSigs, numDocs);

    cout << "\nd)\nDocument Pairs that Plagiarized:" << endl;
    for (auto& pair : candidatePairs) {
        for (auto& elem : pair.second) {
            tempSim = jaccardSim(transpMHT[pair.first], transpMHT[elem]);
            count++;
            if (tempSim >= threshold) {
                flag = 1;
                cout << "\t" << docs[pair.first] << " and " << docs[elem] << " Jaccard Similarity: " << tempSim << endl;
            }
        }
    }
    if (!flag) {
        cout << "\tNone" << endl;
    }
    /*
    cout << "\nSize of Candidate Pairs: " << count << endl;
    for (int i = 0; i < numDocs; i++) {
        for (int j = 0; j < numDocs; j++) {
            cout << "Document " << docs[i] << " and Document " << docs[j] << " Jaccard Similarity: " << jaccardSim(transpMHT[i], transpMHT[j]) << endl;
        }
    }
    */
}

float jaccardSim(vector<int> v1, vector<int> v2) {
    int sameCount = 0;

    for (int i = 0; i < v1.size(); i++) {
        if (v1[i] == v2[i]) {
            sameCount++;
        }
    }
    return float(sameCount) / float(v1.size());
}

double and_or(double x) {
    return (1 - pow((1.0 - pow(x, double(SIGNATURES/NUM_BANDS))), double(NUM_BANDS)));
}

int main(void) {
    vector<string> docs;
    unordered_map<string, vector<int>> shingles;
    vector<vector<int>> randPerms, minHashTable;
    map<int, unordered_set<int>> candidatePairs;

    if (!input_docs(docs)) {
        cout << "Error: Less than two valid documents were provided\n" << endl;
        return 0;
    }

    shingling(docs, shingles);
    minHashTable = minHashing(shingles, randPerms, docs.size());
    candidatePairs = lsh(minHashTable);  
    cout << "\nc)\nParameters: b = " << NUM_BANDS << " , r = " << SIGNATURES/NUM_BANDS << endl;
    cout << "AND-OR construction: y = 1 - (1 - x^" << SIGNATURES/NUM_BANDS << ")^" << NUM_BANDS << endl;
    cout << "Resulting probabilities: (0.2, 0.8, " << and_or(0.8) << ", " << and_or(0.2) << ")" << endl;
    findPlagiarism(candidatePairs, minHashTable, docs);

    return 1;
}