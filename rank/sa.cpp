#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

struct suffix_compar 
{
	int * L;
	vector < vector<int>> &R;

	suffix_compar ( int * ME, vector< vector<int> > &rank ):
		L(ME), R(rank) {}
	bool operator() ( int i1, int i2 )
	{
		bool i1_is_shorter;
		int cover;
		int level;
		if ( L[i1] == 0 )
			return true;
		if ( L[i2] == 0 )
			return false;
		if ( L[i1] < L[i2] )
		{
			i1_is_shorter = true;
			level = floor ( log(L[i1])/log(2) );
			cover = pow ( 2, level);
		}
		else
		{
			i1_is_shorter = false;
			level = floor ( log(L[i2])/log(2) );
			cover = pow ( 2, level);
		}

		if ( R[level][i1] == R[level][i2] )
		{
			if ( R[level][i1+L[i1]-cover] == R[level][i2+L[i2]-cover])
				return i1_is_shorter;
			else
				return R[level][i1+L[i1]-cover] < R[level][i2+L[i2]-cover];
		}
		else
		{
			return  R[level][i1] < R[level][i2];
		}
	}
};

void SuffixArray ( int n, int * ME, vector< vector<int> > &rank, int * SA, int *iSA )
{
	vector < int > SA_vec ( n );
	for ( int i = 0; i < n; i++ )
	{
		SA_vec[i] = i;
	}
	
	sort ( SA_vec.begin(), SA_vec.end(), suffix_compar ( ME, rank ) );
	for ( int i = 0; i < n; i++ )
	{
		SA[i] = SA_vec[i];
	}
	for ( int i = 0; i < n; i++ )
	{
		iSA[SA[i]] = i;
	}
}

