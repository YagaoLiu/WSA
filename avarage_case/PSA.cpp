#include <iostream>
#include <vecter>
#include <cmath>
#include <map>
#include <numeric>
#include <bitset>

#include "radixsort.h"

using namespace std;

PropertySuffixArray::PropertySuffixArray( PropertyString const& S, string& alphabet){
	int length = S.length();
	double t = floor( log(length)/log(sigma) );
	map<char,int> char_to_int;
	for( int i = 0; i < alphabet.size(); i++ ){
		char_to_int[alphabet[i]] = i+1;
	}
	vector<int> integer_text ( length+t );
	for ( int i = 0; i < length; i++ ){
		integer_text[i] = char_to_int[S[i]];
	}
	bool next_stage = false;
	int level = 1;
	vector<int> sign = signature( integer_text, length, t, alphabet.size()+1 );
	vector<int> sort_sign ( sign );
	vector<int> index ( length );
	vector<int> rank ( length );
	iota ( index.begin(), index.end(), 0 );
	radixsort ( sort_sign, index, length );
	int r = 0;
	rank[index[0]] = r;
	for ( int i = 1; i < n; i++ ){
		if ( sort_sign[i] == sort_sign[i-1] ){
			rank[index[i]] = r;
		}
		else{
			rank[index[i]] = ++r;
		}
	}
	for ( int i = 0; i < log(length)/log(2); i++ ){
		t = t * (i+1);
		iota ( index.begin(), index.end(), 0 );
		sort ( index, index+length, pair_compare( sign, rank, t ) );
		vector<int> next_rank (n);
		r = 0;
		next_rank[index[0]] = r;
		for ( int i = 1; i < n; i++ ){
			if ( rank[index[i]] == rank[index[i-1]] && sign[index[i+t]] == sign[index[i+t-1]] ){
				next_rank[index[i]] = r;
			}
			else{
				next_rank[index[i]] = ++r;
			}
		}
		rank = next_rank;
	}
	PSA = index;
}

vector<int>& PropertySuffixArray::signature( vector<int>& text, int n, int t, int sigma ){
	vector<int> sign (n);
	for ( int j = 1; j < t; j++ ){
		sign[0] += integer_text[j-1] * pow(sigma, t-j);
	}
	for ( int i = 1; i < n; i++ ){
		sign[i] = ( sign[i-1] - integer_text[i-1] * pow(sigma, t-1) ) * sigma + integer_text[i+t-1];
	}
	for ( int i = 0; i < n; i++ ){
		if ( S.pi[i] < t ){
			sign[i] = sign[i]&( pow(sigma, t) - pow(sigma, t-S.pi[i]) );
		}
	}
	return sign;
}

