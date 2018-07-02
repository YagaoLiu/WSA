#include <iostream>
#include <iomanip> 
#include <vector>
#include <cmath>
#include <map>
#include <numeric>
#include <bitset>
#include <algorithm>

#include "radixsort.h"
#include "PSA.h"

using namespace std;

bool PropertySuffixArray::pair_compare::operator() ( int i1, int i2 ){
	if ( r[i1] != r[i2] ){
		return r[i1] < r[i2];
	}
	else{
		return s[i1] < s[i2];
	}
}

PropertySuffixArray::PropertySuffixArray( PropertyString & S, string& alphabet){
	int length = S.length();
	vector<int> const& property = S.property();
	int sigma = 1;
	while ( sigma < (int) (alphabet.size()+1) ){
		sigma *= 2;
	}

	int max_pi = 0;
	for ( int i : property ){
		if ( i > max_pi ) max_pi = i;
	}	
	double t = floor( log(length)/log(sigma) );
	PSA.resize ( length );
	iota( PSA.begin(), PSA.end(), 0 );

	map<char,int> char_to_int;
	for( int i = 0; i < (int) alphabet.size(); i++ ){
		char_to_int[alphabet[i]] = i+1;
	}
	vector<int> integer_text ( length+t );
	for ( int i = 0; i < length; i++ ){
		integer_text[i] = char_to_int[S[i]];
	}
	for ( int i = 0; i < length; i++ ){
		cout << setw(3) << property[i]; 
	}
	cout << endl;	

	vector<int> sign = signature( integer_text, property, length, t, sigma );
	for ( int i = 0; i < length; i++ ){
		cout << setw(3) << sign[i];
	}
	cout << endl;
	vector<int> sort_sign ( sign );
	vector<int> index ( length );
	vector<int> rank ( length );
	iota ( index.begin(), index.end(), 0 );
	radixsort ( sort_sign, PSA, length );
	int r = 0;
	rank[index[0]] = r;
	for ( int i = 1; i < length; i++ ){
		if ( sort_sign[i] == sort_sign[i-1] ){
			rank[PSA[i]] = r;
		}
		else{
			rank[PSA[i]] = ++r;
		}
	}
	for ( int i = 0; i < length; i++ ){
		cout << setw(3) << PSA[i];
	}
	cout << endl;
	for ( int i = 0; i < length; i++ ){
		cout << setw(3) << rank[PSA[i]];
	}
	cout << endl;
	for ( int level = 0; level < log(length)/log(2); level++ ){
		int tt = t * (level+1);
		if( tt > max_pi ) break;
		for ( int i = 0; i < length; i++ ){
			int updater = pow( sigma, max(property[i]-tt, 0) ) -1;
			sign[i] = sign[i] && updater;
		}
		sort ( PSA.begin(), PSA.end(), pair_compare( sign, rank, property, tt ) );
		vector<int> next_rank (length);
		r = 0;
		next_rank[PSA[0]] = r;
		for ( int i = 1; i < length; i++ ){
			//compute rank
			if ( rank[PSA[i]] == rank[PSA[i-1]] && sign[PSA[i]] == sign[PSA[i-1]] ){
				next_rank[PSA[i]] = r;
			}
			else{
				next_rank[PSA[i]] = ++r;
			}
		}
		rank = next_rank;
		for ( int i = 0; i < length; i++ ){
		cout << setw(3) << PSA[i];
	}
	cout << endl;
	for ( int i = 0; i < length; i++ ){
		cout << setw(3) << rank[PSA[i]];
	}
	cout << endl;
	}
}

vector<int> PropertySuffixArray::signature( vector<int>& text, vector<int> const& property, int n, int t, int sigma ){
	vector<int> sign (n);
	sign[0] = 0;
	for ( int j = 0; j < t; j++ ){
		sign[0] += text[j] * pow(sigma, t-j-1);
	}
	for ( int i = 1; i < n; i++ ){
		sign[i] = ( sign[i-1] - text[i-1] * pow(sigma, t-1) ) * sigma + text[i+t-1];
	}
	for ( int i = 0; i < n; i++ ){
		if ( property[i] < t ){
			int remove = pow(sigma,t) - pow(sigma, t-property[i] );
			sign[i] = sign[i]&remove;
		}
	}
	return sign;
}

