// ============================================================
//      VectorIntHash.hpp
// ------------------------------------------------------------
// Author :     Axel Perez
// Date :       7/02/20
// ============================================================

// include files
#include <vector>
#include <map>
#include <unordered_set>
#include <cstdlib>
#include <cassert>

// constants
unsigned int const HASH_TABLE_SIZE = 1000000007;
//unsigned int const HASH_TABLE_SIZE = 2;

class VectorIntHash {
    public:
        std::size_t vectorSize;
        std::vector<std::vector<int>> hashTable;
        std::vector<int> randInts;
        std::map<int, std::unordered_set<int>> candidatePairs;

        VectorIntHash(std::size_t inputSize) {
            vectorSize = inputSize;
            hashTable.reserve(HASH_TABLE_SIZE);
            srand(vectorSize);

            for (int i = 0; i < vectorSize; i++) {
                randInts.push_back(rand() % HASH_TABLE_SIZE);
            }
        }

        unsigned int hash(std::vector<int> inputVector) {
            assert(inputVector.size() == vectorSize);
            int sum = 0;

            for (int i = 0; i < vectorSize; i++) {
                sum += (randInts[i] * inputVector[i]);
            }
            return sum % HASH_TABLE_SIZE;
        }

        void insert(std::vector<int> inputVector, int doc) {
            unsigned int hashCode = hash(inputVector);

            if (hashTable[hashCode].size() > 0) {
                for (int i = 0; i < hashTable[hashCode].size(); i++) {
                    if (candidatePairs[hashTable[hashCode][i]].count(doc) == 0) {
                        candidatePairs[doc].insert(hashTable[hashCode][i]);
                    }
                }
            }
            hashTable[hashCode].push_back(doc);
        }

        void clearHashTable() {
            hashTable.clear();
            hashTable.reserve(HASH_TABLE_SIZE);
        }
};
