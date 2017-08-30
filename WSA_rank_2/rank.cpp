#include <iostream>
#include <string>
#include <utility>
#include <algorithm>

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

bool same_pair ( pair<int,int> &a, pair<int,int> &b )
{
	if ( a.first == b.first && a.second == b.second )
		return true;
	else
		return false;
}

bool same_suffix_factor ( int i1, int i2, int * ME, int cover,  vector<int> & rank )
{
	int i1_second = i1 + cover;
	int i2_second = i2 + cover;
	if ( ME[i1] < ME[i2] )
	{
		if ( ME[i1] < 2 * cover )
		{
			i1_second = i1 + ME[i1] - cover;
			i2_second = i2 + ME[i1] - cover;
		}
	}	
	else
	{
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
	int * SA;
	vector<int> & R;
	int * pR;
	int n;

	suffix_compar ( int * ME, int l, int * sa, vector<int> & rank, int * rSA, int tn ):
		L(ME), cover(l), SA(sa), R(rank), pR (rSA), n(tn) {}
	bool operator() ( int i1, int i2 )
	{
		if ( pR[i1] != pR[i2] )		//if previous rank is defferent, no more comparison is needed.
			return pR[i1] < pR[i2];
#if 1
		bool i1_is_shorter;			//check which suffix factor is shorter.
		int i1_1st = SA[i1];
		int i2_1st = SA[i2];
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
	const vector < pair<int,int> > & value_vector;
	comparator( const vector < pair<int,int> > & val_vec ):
		value_vector(val_vec){}

	bool operator() ( int i1, int i2 )
	{
		if ( value_vector[i1].first == value_vector[i2].first )
			return value_vector[i1].second < value_vector[i2].second;
		else
			return value_vector[i1].first < value_vector[i2].first;
	}
};

struct check
{
	int n;
	check ( int textn ):
		n ( textn ) {}
	bool operator() ( int i1, int i2 )
	{
		if ( i1 > n || i2 > n )
			cout << i1 << i2 << '\n';
		return i1 < i2;
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

void pa ( int * a, int n )
{
	for ( int i = 0; i < n; i++ )
		cout << a[i] << ' ';
	cout << endl;
}

void Rank_index ( string & sq, int n, int * ME, int * SA, int * iSA )
{
	vector <int> text_rank ( n );
	int level = 0;
	int i = 0 ,j = 0;
	for ( i = 0; i < n; i++ )
	{
		text_rank[i] = char2int ( sq[i] );
		level = max ( level, ME[i] );
	}
	level = floor ( log(level)/ log(2) );
//	cout << "level=" << level << endl;

	for ( i = 0; i < n; i++ )
	{
		SA[i] = i;
	}
	sort ( SA, SA+n, firstSA( text_rank ) );
	int * rSA = new int [n];
	rSA[0] = 0;
	for ( i = 1; i < n; i++ )
	{
		if ( text_rank[SA[i]] == text_rank[SA[i-1]] )
			rSA[i] = j;
		else
			rSA[i] = ++j;
	}
	for ( int k = 1; k <= level; k++ )
	{
		int l = pow (2, k);
		//compute the rank for text at k level
		vector < int > sort_for_rank ( n-l+1 );
		vector < pair<int,int> > pair_cover ( n-l+1 );
		for ( i = 0; i < n-l+1; i++ )
		{
			pair<int, int> p ( text_rank[i], text_rank[i+ l/2] );
			pair_cover[i] = p;
			sort_for_rank[i] = i;
		}
		sort ( sort_for_rank.begin(), sort_for_rank.end(), comparator(pair_cover) );

		text_rank.resize(n-l+1);
		text_rank[0] = 0;
		j = 0;
		for ( i = 1; i < n-l+1; i++ )
		{
			if ( same_pair ( pair_cover[sort_for_rank[i-1]], pair_cover[sort_for_rank[i]] ) )
				text_rank[sort_for_rank[i]] = j;
			else
				text_rank[sort_for_rank[i]] = ++j;
		}

		//sort suffix array at k level
		vector < int > sort_for_SA ( n );
		for ( i = 0; i < n; i++ )
		{
			sort_for_SA[i] = i;
		}
		suffix_compar sc( ME, l, SA, text_rank, rSA,n );
		sort ( sort_for_SA.begin(), sort_for_SA.end(), sc );
		vector<int> temp_SA ( SA, SA+n );
		for ( i = 0; i < n; i++ )
		{
			SA[i] = temp_SA[sort_for_SA[i]];
		}
		j = 0;
		rSA[0] = 0;
		for ( i = 1; i < n; i++ )
		{
			if ( same_suffix_factor ( SA[i-1], SA[i], ME, l, text_rank ) )
				rSA[i] = j;
			else
				rSA[i] = ++j;
		}
#if 0 //print & check
		cout << "level:" << k << endl;
		for ( i = 0; i < n-l+1; i++ )
		{
			cout << i <<"(" << pair_cover[i].first << "," << pair_cover[i].second << ")\t";
		}
		cout << endl;
		for ( i = 0; i < n-l+1; i++ )
			cout << text_rank[i] << '\t';
		cout << endl;
		for ( i = 0; i < n; i++ )
		{
			cout << sq.substr( SA[i], ME[SA[i]] ) << ":\t" << rSA[i] << "\tstarting:" << SA[i] << endl;
		}
#endif 
	}
	for ( i = 0; i < n; i++ )
	{
		iSA[SA[i]] = i;
	}
	delete[] rSA;
}


