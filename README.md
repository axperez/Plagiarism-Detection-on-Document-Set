# Plagiarism-Detection-on-Document-Set

Implementation of finding plagiarism by given a set of documents. A plagiarism extracts/copies (or includes large quotations) other people’s work with modifications as his/her own work.

The program consists of three parts: shingling, minhashing, and locality sensitive hashing. 

The output shows the following:
- input document-term table and random permutations,
- minhashing table,
- the parameter (b and r) and the order of applying combinations of AND or OR constructions with the result probabilities at least (0.2, 0.8, 0.997, 0.003) sensitive, and
- all pairs of plagiarism
