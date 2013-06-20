#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <string.h>

// To calculate the minimum difference between 2 array
//
// Reference:
// Levenshtein demo
// http://www.let.rug.nl/kleiweg/lev/
// Minimum edit distance
// http://www.stanford.edu/class/cs124/lec/med.pdf
// Sequence Alignment
// http://math.mit.edu/classes/18.417/Slides/alignment.pdf
// Fast and Easy Levenshtein distance using a Trie
// http://stevehanov.ca/blog/index.php?id=114
//
// Sample usage:
// const char str1[] = "*intention";
// const char str2[] = "*execution";
// Levenshtein levenshtein;
// levenshtein.ComputeMinEdit(str1+1, strlen(str1)-1, str2+1, strlen(str2)-1);
// for(int i=0; i<levenshtein.align1.size(); ++i)
//     std::cout << str1[levenshtein.align1[i]+1];
// std::cout << std::endl;
// for(int i=0; i<levenshtein.align2.size(); ++i)
//     std::cout << str2[levenshtein.align2[i]+1];
//
// Result:
// inte*ntion
// *execution
class Levenshtein
{
public:
	// After calling ComputeMinEdit(), the resulting alignment of the 2 array will store here
	std::vector<int> align1, align2;

	template<typename T>
	void ComputeMinEdit(T* v1, size_t len1, T* v2, size_t len2)
	{
		// Initialize the matrix
		std::vector<Tuple> v;
		v.resize(len2 + 1);
		m.resize(len1 + 1, v);

		// Initialize the boundary for back trace
		for(int i=0; i<=len1; ++i)
			m[i][0].trace = 1;
		for(int j=0; j<=len2; ++j)
			m[0][j].trace = 2;

		// Action!
		recurse((int)len1, (int)len2, v1, v2);

		// Performs back trace
		int i = (int)len1;
		int j = (int)len2;
		while(i + j > 0) {
			if(m[i][j].trace == 1) {
				align1.push_back(i - 1);
				align2.push_back(-1);
				--i;
			}
			else if(m[i][j].trace == 2) {
				align1.push_back(-1);
				align2.push_back(j - 1);
				--j;
			}
			else if(m[i][j].trace == 3) {
				align1.push_back(i - 1);
				align2.push_back(j - 1);
				--i;
				--j;
			}
			else {
				assert(false);
			}
		}

		std::reverse(align1.begin(), align1.end());
		std::reverse(align2.begin(), align2.end());
	}

protected:
	struct Tuple
	{
		Tuple() : distance(-1), trace(-1) {}
		int distance, trace;
	};

	typedef std::vector<std::vector<Tuple> > Matrix;

	template<typename T>
	int recurse(int i, int j, T* v1, T* v2)
	{
		if(i == 0)
			return j;
		if(j == 0)
			return i;

		if(m[i][j].distance != -1)
			return m[i][j].distance;

		int d1 = recurse(i-1, j-0, v1, v2) + 1;
		int d2 = recurse(i-0, j-1, v1, v2) + 1;
		int d3 = recurse(i-1, j-1, v1, v2) + (v1[i-1] == v2[j-1] ? 0 : 2);

		int dMin = min(d1, d2, d3);
		int trace = -1;
		if(dMin == d3)
			trace = 3;
		else if(dMin == d1)
			trace = 1;
		else
			trace = 2;

		m[i][j].distance = dMin;
		m[i][j].trace = trace;

		return dMin;
	}

	int min(int a, int b) { return a < b ? a : b; }
	int min(int a, int b, int c) { return min(a, min(b, c)); }

	Matrix m;
};	// class Levenshtein

int main()
{
	const char str1[] = "*intention";
	const char str2[] = "*execution";

	Levenshtein levenshtein;
	levenshtein.ComputeMinEdit(str1+1, strlen(str1)-1, str2+1, strlen(str2)-1);

	for(int i=0; i<levenshtein.align1.size(); ++i)
		std::cout << str1[levenshtein.align1[i]+1];

	std::cout << std::endl;

	for(int i=0; i<levenshtein.align2.size(); ++i)
		std::cout << str2[levenshtein.align2[i]+1];

	return 0;
}
