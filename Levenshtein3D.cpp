#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <string.h>
#include <limits.h>

// To calculate the minimum difference between 3 array, suitable for doing 3 way merge
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
// const char str3[] = "*extinction";
// Levenshtein3D levenshtein;
// levenshtein.ComputeMinEdit(str1+1, strlen(str1)-1, str2+1, strlen(str2)-1, strlen(str3)-1);
// for(int i=0; i<levenshtein.align1.size(); ++i)
//     std::cout << str1[levenshtein.align1[i]+1];
// std::cout << std::endl;
// for(int i=0; i<levenshtein.align2.size(); ++i)
//     std::cout << str2[levenshtein.align2[i]+1];
// std::cout << std::endl;
// for(int i=0; i<levenshtein.align3.size(); ++i)
//     std::cout << str3[levenshtein.align3[i]+1];
//
// Result:
// in**t*en**tion
// **ex**e*cution
// **exti*nc*tion
class Levenshtein3D
{
public:
	// After calling ComputeMinEdit(), the resulting alignment of the 3 array will store here
	std::vector<int> align1, align2, align3;

	template<typename T>
	void ComputeMinEdit(T* v1, size_t len1, T* v2, size_t len2, T* v3, size_t len3)
	{
		// Initialize the matrix
		Array1D a1;
		a1.resize(len3 + 1);
		Array2D a2;
		a2.resize(len2 + 1, a1);
		m.resize(len1 + 1, a2);

		// Initialize the boundary for back trace
		for(size_t i=0; i<=len1; ++i)
			m[i][0][0].trace = 0;
		for(size_t j=0; j<=len2; ++j)
			m[0][j][0].trace = 1;
		for(size_t k=0; k<=len3; ++k)
			m[0][0][k].trace = 2;

		// Action!
		recurse((int)len1, (int)len2, (int)len3, v1, v2, v3);

		static const int step[7][3] = {
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 },
			{ 1, 1, 0 },
			{ 1, 0, 1 },
			{ 0, 1, 1 },
			{ 1, 1, 1 }
		};

		// Performs back trace
		int i = (int)len1;
		int j = (int)len2;
		int k = (int)len3;
		while(i + j + k > 0) {
			int t = m[i][j][k].trace;
			const int idx[4] = { -1, i-1, j-1, k-1 };

			align1.push_back(idx[step[t][0] * 1]);
			align2.push_back(idx[step[t][1] * 2]);
			align3.push_back(idx[step[t][2] * 3]);

			i -= step[t][0];
			j -= step[t][1];
			k -= step[t][2];
		}

		std::reverse(align1.begin(), align1.end());
		std::reverse(align2.begin(), align2.end());
		std::reverse(align3.begin(), align3.end());
	}

protected:
	struct Tuple
	{
		Tuple() : distance(-1), trace(-1) {}
		int distance, trace;
	};

	typedef std::vector<Tuple> Array1D;
	typedef std::vector<Array1D> Array2D;
	typedef std::vector<Array2D> Array3D;

	template<typename T>
	int recurse(int i, int j, int k, T* v1, T* v2, T* v3)
	{
		int& dist = m[i][j][k].distance;

		if(dist != -1)
			return dist;

		if(min(i, j, k) == 0) {
			int d = -1;
			if(i == 0) d = j + k;
			if(j == 0) d = i + k;
			if(k == 0) d = i + j;
			dist = d;
			return d;
		}

		bool bij = v1[i-1] == v2[j-1];
		bool bik = v1[i-1] == v3[k-1];
		bool bjk = v2[j-1] == v3[k-1];

		const int dVals[] = {
			recurse(i-1, j-0, k-0, v1, v2, v3) + 1,
			recurse(i-0, j-1, k-0, v1, v2, v3) + 1,
			recurse(i-0, j-0, k-1, v1, v2, v3) + 1,
			recurse(i-1, j-1, k-0, v1, v2, v3) + (bij ? 0 : 2),
			recurse(i-1, j-0, k-1, v1, v2, v3) + (bik ? 0 : 2),
			recurse(i-0, j-1, k-1, v1, v2, v3) + (bjk ? 0 : 2),
			recurse(i-1, j-1, k-1, v1, v2, v3) + ((bij && bjk) ? 0 : 4),
		};

		int dMin = INT_MAX;
		for(int it=0; it < sizeof(dVals)/sizeof(dVals[0]); ++it) {
			if(dVals[it] <= dMin) {
				dMin = dVals[it];
				m[i][j][k].trace = it;
			}
		}

		dist = dMin;
		return dMin;
	}

	int min(int a, int b) { return a < b ? a : b; }
	int min(int a, int b, int c) { return min(a, min(b, c)); }
	int max(int a, int b) { return a > b ? a : b; }
	int max(int a, int b, int c) { return max(a, max(b, c)); }

	Array3D m;
};	// class Levenshtein

int main()
{
	const char str1[] = "*intention";
	const char str2[] = "*execution";
	const char str3[] = "*extinction";

	Levenshtein3D levenshtein;
	levenshtein.ComputeMinEdit(str1+1, strlen(str1)-1, str2+1, strlen(str2)-1, str3+1, strlen(str3)-1);

	for(int i=0; i<levenshtein.align1.size(); ++i)
		std::cout << str1[levenshtein.align1[i]+1];

	std::cout << std::endl;
	for(int i=0; i<levenshtein.align2.size(); ++i)
		std::cout << str2[levenshtein.align2[i]+1];

	std::cout << std::endl;
	for(int i=0; i<levenshtein.align3.size(); ++i)
		std::cout << str3[levenshtein.align3[i]+1];

	return 0;
}
