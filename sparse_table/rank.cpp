#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <algorithm>
#include <cmath>

#include "defs.h"

using namespace std;

int char2int ( char c )
{
	switch ( c )
	{
		case 'A':
			return 0;
		case 'C':
			return 1;
		case 'G':
			return 2;
		case 'T':
			return 3;
		default:
			return 4;
	}
}

bool same_suffix_factor ( int i1, int i2, int * ME, int cover,  vector<int> & rank, int * rSA )
{
	int i1_second = i1 + cover;
	int i2_second = i2 + cover;
	if ( rSA[i1] != rSA[i2] )
	{
		return false;
	}
	if ( ME[i1] < ME[i2] )
	{
		if ( ME[i1] < cover )
			return false;
		if ( ME[i1] < 2 * cover )
		{
			i1_second = i1 + ME[i1] - cover;
			i2_second = i2 + ME[i1] - cover;
		}
	}	
	else
	{
		if ( ME[i2] < cover )
		{
			if ( ME[i2] == ME[i1] )
				return true;
			else
				return false;
		}
		if ( ME[i2] < 2 * cover )
		{
			i1_second = i1 + ME[i2] - cover;
			i2_second = i2 + ME[i2] - cover;
		}
	}
	if ( rank[i1] == rank[i2] && rank[i1_second] == rank[i2_second] )
		return true;
	else
		return false;
}

struct suffix_compar 
{
	int * L;
	int cover;
	vector<int> & R;
	int * pR;
	int n;

	suffix_compar ( int * ME, int l, vector<int> & rank, int * rSA, int tn ):
		L(ME), cover(l), R(rank), pR (rSA), n(tn) {}
	bool operator() ( int i1, int i2 )
	{
		if ( pR[i1] != pR[i2] )		//if previous rank is defferent, no more comparison is needed.
			return pR[i1] < pR[i2];
#if 1
		bool i1_is_shorter;			//check which suffix factor is shorter.
		int i1_1st = i1;
		int i2_1st = i2;
		int i1_2nd = i1_1st + cover;
		int i2_2nd = i2_1st + cover;
		if ( L[i1_1st] < L[i2_1st] )
		{
			if ( L[i1_1st] < cover )
				return L[i1_1st] < L[i2_1st];
			if ( L[i1_1st] < 2 * cover )	//if the shorter factor is shorter than 2|cover|, the cover compar has overlap.
			{
				i1_2nd = i1_1st + L[i1_1st] - cover;
				i2_2nd = i2_1st + L[i1_1st] - cover;
			}
		}
		else
		{
			if ( L[i2_1st] < cover )
				return L[i1_1st] < L[i2_1st];
			i1_is_shorter = false;
			if ( L[i2_1st] < 2 * cover )
			{
				i2_2nd = i2_1st + L[i2_1st] - cover;
				i1_2nd = i1_1st + L[i2_1st] - cover;
			}
		}
		if ( R[i1_1st] == R[i2_1st] )	//if the first cover compar is a draw, do the second cover compar
		{
			if ( R[i1_2nd] == R[i2_2nd])	//still a draw, basic on the length
			{
				return L[i1_1st] < L[i2_1st];
			}
			else
			{
				return R[i1_2nd] < R[i2_2nd];
			}
		}
		else	//the first compar is not a draw
		{
			return  R[i1_1st] < R[i2_1st];
		}
#endif
	}
};

struct comparator 
{
	const vector <int> & value_vector;
	int cover;
	comparator( const vector <int>  & val_vec, int l ):
		value_vector(val_vec), cover(l) {}

	bool operator() ( int i1, int i2 )
	{
		if ( value_vector[i1] == value_vector[i2] )
			return value_vector[i1+cover/2] < value_vector[i2+cover/2];
		else
			return value_vector[i1] < value_vector[i2];
	}
};

struct firstSA
{
	vector <int> & v;
	firstSA ( vector <int> & text_rank ):
		v ( text_rank ) {}
	bool operator() ( int i1, int i2 )
	{
		return v[i1] < v[i2];
	}
};

void Rank_index ( string & sq, int n, int * ME, int * SA )
{
	vector <int> base_rank ( n );
	int level = 0;
	int i = 0 ,j = 0;
	for ( i = 0; i < n; i++ )
	{
		base_rank[i] = char2int ( sq[i] );
		level = max ( level, ME[i] );
	}
	level = floor ( log(level)/ log(2) );

	for ( i = 0; i < n; i++ )
	{
		SA[i] = i;
	}
	sort ( SA, SA+n, firstSA( base_rank ) );
	int * rSA = new int [n];
	rSA[SA[0]] = 0;
	for ( i = 1; i < n; i++ )
	{
		if ( base_rank[SA[i]] == base_rank[SA[i-1]] )
			rSA[SA[i]] = j;
		else
			rSA[SA[i]] = ++j;
	}
#if 0

		cout << "level: " << 1 << '\n';
		for ( i = 0; i < n; i++ )
			cout << setw(2) << i << ": " << setw(2) << SA[i] << ", " << setw(2) << rSA[SA[i]] << ", " << sq.substr( SA[i], ME[SA[i]] ) << '\n';

#endif
	for ( int k = 1; k <= level; k++ )
	{
		int l = pow (2, k);
		//compute the rank for text at k level
		vector < int > sort_for_rank ( n-l+1 );
		for ( i = 0; i < n-l+1; i++ )
		{
			sort_for_rank[i] = i;
		}
		sort ( sort_for_rank.begin(), sort_for_rank.end(), comparator(base_rank, l) );

		j = 0;
		vector < int > swap_rank ( n-l+1 );
		swap_rank[sort_for_rank[0]] = 0;
		for ( i = 1; i < n-l+1; i++ )
		{
			if ( base_rank[sort_for_rank[i-1]] == base_rank[sort_for_rank[i]] && base_rank[sort_for_rank[i-1]+l/2] == base_rank[sort_for_rank[i]+l/2] )
				swap_rank[sort_for_rank[i]] = j;
			else
				swap_rank[sort_for_rank[i]] = ++j;
		}
		base_rank.swap( swap_rank );
		vector<int>().swap(swap_rank);
		vector<int>().swap(sort_for_rank);

		//sort suffix array at k level
		suffix_compar sc( ME, l, base_rank, rSA, n );
		sort ( SA, SA+n, sc );
		j = 0;
		int * newrSA = new int [n];
		newrSA[SA[0]] = 0;
		for ( i = 1; i < n; i++ )
		{
			if ( same_suffix_factor ( SA[i-1], SA[i], ME, l, base_rank, rSA ) )
				newrSA[SA[i]] = j;
			else
				newrSA[SA[i]] = ++j;
		}
		for ( i = 0; i < n; i++ )
			rSA[i] = newrSA[i];
		delete[] newrSA;
#if 0 
		for ( i = 0; i < n; i++ )
			cout << setw(2) << i << ": " << setw(2) << SA[i] << ", " << setw(2) << rSA[SA[i]] << ", " << sq.substr( SA[i], ME[SA[i]] ) << '\n';

#endif
	}
	delete[] rSA;
}


